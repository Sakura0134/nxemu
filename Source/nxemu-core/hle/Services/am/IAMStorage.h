#pragma once
#include <nxemu-core\hle\Kernel\Service.h>
#include <vector>

class IStorageAccessor;

class IAMStorage :
    public CService
{
	friend IStorageAccessor;

public:
	enum Method
	{
		Open = 0,
        OpenTransferStorage = 1
	};
    ~IAMStorage();

    static CKernelObjectPtr CreateInstance(CSwitchSystem & System, uint8_t * Data, uint64_t Size);

    //__interface IService
    bool Connect(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "IStorage"; }

private:
	IAMStorage(void);
	IAMStorage(const IAMStorage&);
	IAMStorage& operator=(const IAMStorage&);

	IAMStorage(CSwitchSystem & System, uint8_t * Data, uint64_t Size);

    std::vector<uint8_t> m_Data;
};