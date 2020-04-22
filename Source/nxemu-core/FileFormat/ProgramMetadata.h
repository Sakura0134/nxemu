#pragma once
#include <Common\stdtypes.h>
#include <nxemu-core\FileSystem\PartitionFilesystem.h>

class CProgramMetadata
{
public:
    CProgramMetadata(uint64_t offset, CEncryptedFile &EncryptedFile, const CPartitionFilesystem::VirtualFile * npdm);

    inline uint8_t GetMainThreadPriority(void) const { return m_Header.main_thread_priority; }
    inline bool Is64bit(void) const { return ((Flags *)(&m_Header.flags))->Is64BitInstructions != 0; }

private:
    CProgramMetadata(void);                                // Disable default constructor
    CProgramMetadata(const CProgramMetadata&);             // Disable copy constructor
    CProgramMetadata& operator=(const CProgramMetadata&);  // Disable assignment

    struct Header
    {
        uint8_t magic[4];
        uint8_t reserved[8];
        uint8_t flags;
        uint8_t reserved1;
        uint8_t main_thread_priority;
        uint8_t main_thread_cpu;
        uint8_t reserved2[8];
        uint32_t process_category;
        uint32_t main_stack_size;
        uint8_t application_name[0x10];
        uint8_t reserved3[0x40];
        uint32_t aci_offset;
        uint32_t aci_size;
        uint32_t acid_offset;
        uint32_t acid_size;
    };

    struct Flags
    {
        unsigned Is64BitInstructions : 1;
        unsigned AddressSpaceType : 3;
    };

    Header m_Header;
};