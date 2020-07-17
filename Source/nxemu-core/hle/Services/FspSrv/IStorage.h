#pragma once
#include <nxemu-core\hle\Kernel\Service.h>
#include <nxemu-core\FileSystem\EncryptedFile.h>

class IStorage :
    public CService
{
public:
    static CKernelObjectPtr CreateInstance(CSwitchSystem & System, CEncryptedFile * File, uint64_t Offset);

    //__interface IService
    bool Connect(void);
    ResultCode CallMethod(CIPCRequest & Request);
    const char * Name(void) const { return "IStorage"; }

private:
    IStorage(void);
    IStorage(const IStorage&);
    IStorage& operator=(const IStorage&);

	IStorage(CSwitchSystem & System, CEncryptedFile * File, uint64_t Offset);

	CEncryptedFile * m_File;
	uint64_t m_Offset;
};