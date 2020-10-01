#pragma once
#include <nxemu-core\hle\Kernel\Service.h>

class IAMStorage;

class IStorageAccessor :
	public CService
{
	friend IAMStorage;

public:
	enum
	{
		Method_GetSize = 0,
        Method_Write = 10,
        Method_Read = 11,
	};

	//__interface IService
	bool Connect(void);
	void Close(void);
	ResultCode CallMethod(CIPCRequest & Request);
	const char * Name(void) const { return "IStorageAccessor"; }

private:
	IStorageAccessor(void);
	IStorageAccessor(const IStorageAccessor&);
	IStorageAccessor& operator=(const IStorageAccessor&);

	IStorageAccessor(CSwitchSystem & System, IAMStorage * storage);

	void ProcessGetSize(CIPCRequest & Request);

    CKernelObjectPtr m_StorageService;
	IAMStorage * m_Storage;
};