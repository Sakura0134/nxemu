#pragma once
#include <nxemu-core\Machine\SwitchKeys.h>
#include <nxemu-core\FileSystem\PartitionFilesystem.h>

class NSP
{
public:
	NSP(CSwitchKeys & Keys, CFile & ReadFile, int64_t PartitionOffset, const CPartitionFilesystem::VirtualFile * file);
	~NSP();
		
	inline bool Valid(void) const { return m_Valid; }

private:
	NSP(void);                   // Disable default constructor
	NSP(const NSP&);             // Disable copy constructor
	NSP& operator=(const NSP&);  // Disable assignment
	
	bool m_Valid;
};