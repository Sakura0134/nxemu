#pragma once
#include <nxemu-core\Machine\SwitchKeys.h>
#include <nxemu-core\FileSystem\EncryptedFile.h>
#include <Common\stdtypes.h>
#include <Common\FileClass.h>

class CPartitionFilesystem;

class NCA
{
public:
	typedef std::vector<CPartitionFilesystem*> CPartitionFilesystems;

	NCA(void);
	~NCA();

	bool Load(CSwitchKeys & Keys, CFile & file, int64_t BaseOffset, int64_t FileOffset, uint64_t Size);
	inline const CPartitionFilesystems & Dirs(void) const { return m_dirs; }

private:
	NCA(const NCA&);            // Disable copy constructor
	NCA& operator=(const NCA&); // Disable assignment

	CPartitionFilesystems m_dirs;
};
