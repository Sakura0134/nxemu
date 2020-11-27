#pragma once
#include <Common\stdtypes.h>
#include <nxemu-core\hle\Memory\SystemThreadMemory.h>
#include <nxemu-core\hle\Kernel\ResultCode.h>
#include <nxemu-core\hle\Kernel\KernelObject.h>
#include <vector>
#include <memory>

__interface IService;
class CSwitchSystem;

class CIPCRequest
{
public:
    enum IpcMagic : uint64_t
    {
        SFCI = 'S' << 0 | 'F' << 8 | 'C' << 16 | 'I' << 24,
        SFCO = 'S' << 0 | 'F' << 8 | 'C' << 16 | 'O' << 24,
    };

    enum IPC_COMMAND_TYPE
    {
        Command_Invalid = 0,
        Command_LegacyRequest = 1,
        Command_Close = 2,
        Command_LegacyControl = 3,
        Command_Request = 4,
        Command_Control = 5,
        Command_RequestWithContext = 6,
        Command_ControlWithContext = 7
    };

    typedef struct
    {
        unsigned CommandType : 16;
        unsigned num_buf_x_descriptors : 4;
        unsigned num_buf_a_descriptors : 4;
        unsigned num_buf_b_descriptors : 4;
        unsigned num_buf_w_descriptors : 4;
        unsigned data_size : 10;
        unsigned buf_c_descriptor_flags : 4;
        unsigned reserved : 17;
        unsigned enable_handle_descriptor : 1;
    } IpcMessageCmd;

    typedef struct
    {
        unsigned SendCurrentPID : 1;
        unsigned NumberOfHandlesToCopy : 4;
        unsigned NumberOfHandlesToMove : 4;
        unsigned : 23;
    } IpcHandleDesc;

    typedef struct
    {
        uint8_t Command;
        uint8_t InputObjectCount;
        uint16_t DataLength;
        uint32_t ObjectID;
        uint32_t Padding;
        uint32_t Token;
    } IpcDomainMessage;

    typedef struct
    {
        uint64_t Magic;
        uint64_t Command;
    } IpcRequestHeader;

    typedef struct
    {
        uint64_t Magic;
        uint64_t Result;
    } IpcResponseHeader;

    typedef struct
    {
        uint32_t Size31_0;
        uint32_t Address31_0;
        struct
        {
            unsigned Flags : 2;
            unsigned Address38_36 : 3;
            unsigned : 19;
            unsigned Size35_32 : 4;
            unsigned Address35_32 : 4;
        } p;
    } IpcBuffDescPack;
    
    typedef struct
    {
        uint32_t Address31_0;
        struct
        {
            unsigned Address47_32 : 16;
            unsigned Size : 16;
        } p;
    } IpcRecvBuffDescPack;

    typedef struct
    {
        uint64_t Address;
        uint64_t Size;
        uint32_t Flags;
    } IpcBuffDesc;

    typedef struct
    {
        uint64_t Address;
        uint64_t Size;
    } IpcRecvBuffDesc;

    typedef std::vector<uint8_t> REQUEST_DATA;
    typedef std::vector<uint8_t> RequestBuffer;
    typedef std::vector<uint32_t> HandleList;
    typedef std::vector<IpcBuffDesc> IpcBuffDescList;
    typedef std::vector<IpcRecvBuffDesc> IpcRecvBuffDescList;

    CIPCRequest(CSwitchSystem & System, uint64_t RequestAddress, CService * Service);

    inline IPC_COMMAND_TYPE CommandType(void) const { return (IPC_COMMAND_TYPE)m_cmd.CommandType;  }
    inline const IpcRequestHeader & RequestHeader(void) const { return m_RequestHeader; }
    inline const REQUEST_DATA & RequestData() const { return m_RequestData; }
    inline const IpcDomainMessage & DomainMessage() const { return m_DomainMessage; }
    inline const IpcRecvBuffDescList & RecvBuffList() const { return m_RecvBuffList; }
    inline const IpcBuffDescList & ReceiveBuff() const { return m_ReceiveBuff; }
    inline bool IsDomainRequest(void) const { return m_IsDomainRequest; }
	inline REQUEST_DATA & ResponseData() { return m_ResponseData; }
	inline CSwitchSystem & SwitchSystem() { return m_System; }

    static const char * CommandTypeName(IPC_COMMAND_TYPE Id);

	void AddResponseHandlesToCopy(uint32_t handle);
	void AddResponseHandlesToMove(uint32_t handle);
	void AddObjectID(uint32_t ObjectId);
	bool WriteResponse(ResultCode call_result);
	void SetDomainRequest(bool DomainRequest);
	void MakeObject(CService * Service);
	uint64_t GetWriteBufferSize(uint32_t BufferIndex = 0);
    bool WriteBuffer(const RequestBuffer & Buffer, uint32_t BufferIndex = 0) const;
    bool WriteBuffer(const uint8_t * buffer, uint32_t size, uint32_t BufferIndex = 0) const;

private:
    CIPCRequest(void);
    CIPCRequest(const CIPCRequest&);
    CIPCRequest& operator=(const CIPCRequest&);

    void ReadBuffList(uint64_t & read_addr, IpcBuffDescList & list, uint32_t size);
    void ReadRecvBuffList(uint64_t & read_addr, IpcRecvBuffDescList & list, uint32_t size);
    static uint32_t GetPadSize16(uint32_t address);

	CSwitchSystem & m_System;
    CSystemThreadMemory & m_ThreadMemory;
    uint64_t m_RequestAddress;
    uint64_t m_PId;
    IpcMessageCmd m_cmd;
    IpcHandleDesc m_HandleDesc;
    IpcRequestHeader m_RequestHeader;
    IpcDomainMessage m_DomainMessage;
    HandleList m_RequestHandlesToCopy;
    HandleList m_ResponseHandlesToMove, m_ResponseHandlesToCopy;
	HandleList m_ObjectIds;
    IpcBuffDescList m_ReceiveBuff;
    IpcRecvBuffDescList m_RecvBuffList;
    REQUEST_DATA m_RequestData, m_ResponseData;
    CKernelObjectPtr m_Service;
    bool m_IsDomainRequest;
    bool m_valid;
};
