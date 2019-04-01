#pragma once
#include <Common\stdtypes.h>
#include <nxemu-core\FileSystem\EncryptedFile.h>

class CPartitionFilesystem
{
public:
	typedef struct
	{
		std::string Name;
		uint64_t Offset;
		uint64_t Size;
	} VirtualFile;

	typedef std::vector<VirtualFile> VirtualFiles;

	CPartitionFilesystem(CEncryptedFile EncryptedFile, uint64_t Offset, uint64_t StartAddress, uint64_t Size);

	const VirtualFile * GetFile(const char * Name) const;
	inline uint64_t Offset(void) const { return m_Offset; }
	inline const VirtualFiles & GetFiles(void) const { return m_Files; }
	inline CEncryptedFile & EncryptedFile() { return m_EncryptedFile; }
	inline bool Valid() const { return m_Valid; }

private:
	CPartitionFilesystem(void);                                   // Disable default constructor
	CPartitionFilesystem(const CPartitionFilesystem&);            // Disable copy constructor
	CPartitionFilesystem& operator=(const CPartitionFilesystem&); // Disable assignment

	bool Load(int64_t Offset, uint64_t Size);

	CEncryptedFile m_EncryptedFile;
	VirtualFiles m_Files;
	uint64_t m_Offset;
	uint64_t m_StartAddress;
	uint64_t m_Size;
	bool m_Valid;
};