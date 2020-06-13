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
    typedef std::vector<uint8_t> REQUEST_DATA;

    CIPCRequest(CSwitchSystem & System, uint64_t RequestAddress, CService * Service);

    inline IPC_COMMAND_TYPE CommandType(void) const { return (IPC_COMMAND_TYPE)m_cmd.CommandType;  }
    inline const REQUEST_DATA & RequestData() const { return m_RequestData; }

    static const char * CommandTypeName(IPC_COMMAND_TYPE Id);

	bool WriteResponse(ResultCode call_result);

private:
    CIPCRequest(void);
    CIPCRequest(const CIPCRequest&);
    CIPCRequest& operator=(const CIPCRequest&);

    static uint32_t GetPadSize16(uint32_t address);

	CSwitchSystem & m_System;
    CSystemThreadMemory & m_ThreadMemory;
    uint64_t m_RequestAddress;
    IpcMessageCmd m_cmd;
    IpcDomainMessage m_DomainMessage;
    REQUEST_DATA m_RequestData;
    CKernelObjectPtr m_Service;
    bool m_IsDomainRequest;
    bool m_valid;
};
