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
		GetSize = 0,
		Write = 10,
		Read = 11,
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

    CKernelObjectPtr m_StorageService;
	IAMStorage * m_Storage;
};