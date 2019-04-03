#pragma once
#include <nxemu-core\Machine\SwitchKeys.h>
#include <nxemu-core\FileSystem\PartitionFilesystem.h>
#include <nxemu-core\FileFormat\cnmt.h>
#include <Common\stdtypes.h>
#include <Common\FileClass.h>

class NCA;

class NSP
{
public:
	typedef std::vector<NCA *> NCAS;
	typedef std::map<CNMT::ContentRecordType, NCA *> NCAS_CONTENTTYPE;
	typedef std::map<uint64_t, NCAS_CONTENTTYPE> NCAS_TITLEID;

	NSP(CSwitchKeys & Keys, CFile & ReadFile, int64_t PartitionOffset, const CPartitionFilesystem::VirtualFile * file);
	~NSP();
		
	NCA * GetNCA(uint64_t title_id, CNMT::ContentRecordType type) const;
	uint64_t GetProgramTitleID(void) const;

	inline bool Valid(void) const { return m_Valid; }

private:
	NSP(void);                   // Disable default constructor
	NSP(const NSP&);             // Disable copy constructor
	NSP& operator=(const NSP&);  // Disable assignment

	typedef CPartitionFilesystem::VirtualFiles VirtualFiles;

	bool ReadTicketKeys(CSwitchKeys & Keys, CFile & ReadFile, int64_t PartitionOffset, const CPartitionFilesystem::VirtualFile * file);
	bool ReadNCAs(CSwitchKeys & Keys, CFile & ReadFile, int64_t PartitionOffset, const CPartitionFilesystem::VirtualFile * file);
	uint64_t GetFirstTitleID(void) const;
	static uint8_t ToHexNibble(char c1);

	CPartitionFilesystem * m_Files;
	NCAS_TITLEID m_NcasTitleId;
	NCAS m_Ncas;
	bool m_Valid;
};