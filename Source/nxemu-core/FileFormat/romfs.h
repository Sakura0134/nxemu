#pragma once
#include <nxemu-core\Machine\SwitchKeys.h>
#include <nxemu-core\FileSystem\EncryptedFile.h>
#include <Common\FileClass.h>
#include <string>
#include <stdint.h>

class RomFS
{
public:
    typedef struct
    {
        std::string name;
        uint64_t offset;
        uint64_t size;
    } FILE;
    
    RomFS(void);
	~RomFS();

	bool Load(CEncryptedFile * file, uint64_t offset);
    const FILE * GetFile(const char * name);

private:
    RomFS(const RomFS&);            // Disable copy constructor
    RomFS& operator=(const RomFS&); // Disable assignment

    enum : uint32_t
    {
        ROMFS_ENTRY_EMPTY = 0xFFFFFFFF
    };

    struct TableLocation
    {
        uint64_t Offset;
        uint64_t Size;
    };

    struct RomFSHeader
    {
        uint64_t HeaderSize;
        TableLocation DirectoryHash;
        TableLocation DirectoryMeta;
        TableLocation FileHash;
        TableLocation FileMeta;
        uint64_t DataOffset;
    };
    
    bool ProcessDirectory(CEncryptedFile * file, uint64_t offset, uint64_t DirOffset, uint64_t FileOffset, uint64_t DataOffset);
    bool ProcessFile(CEncryptedFile * file, uint64_t offset, uint64_t FileOffset, uint64_t ThisFileOffset, uint64_t DataOffset);

    std::vector<FILE> m_Files;
};
