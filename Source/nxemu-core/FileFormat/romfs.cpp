#include <nxemu-core\FileFormat\romfs.h>
#include <nxemu-core\SystemGlobals.h>

RomFS::RomFS(void)
{
}

RomFS::~RomFS()
{
}

bool RomFS::Load(CEncryptedFile * file, uint64_t offset)
{
    if (file == NULL)
    {
        return false;
    }
    if (file->Seek(offset, CFileBase::begin) < 0)
    {
        return false;
    }

    RomFSHeader header;
    if (!file->Read(&header, sizeof(header), 0))
    {
        return false;
    }
    if (header.HeaderSize != sizeof(header))
    {
        return false;
    }
    uint64_t DirOffset = header.DirectoryMeta.Offset + 4;
    uint64_t FileOffset = header.FileMeta.Offset;
    if (!ProcessDirectory(file, offset, DirOffset, FileOffset, header.DataOffset))
    {
        return false;
    }
    return true;
}

const RomFS::FILE * RomFS::GetFile(const char * name)
{
    if (name != NULL)
    {
        for (size_t i = 0, n = m_Files.size(); i < n; i++)
        {
            if (_stricmp(name, m_Files[i].name.c_str()) == 0)
            {
                return &m_Files[i];
            }
        }
    }
    return NULL;
}

bool RomFS::ProcessDirectory(CEncryptedFile * file, uint64_t offset, uint64_t DirOffset, uint64_t FileOffset, uint64_t DataOffset)
{
    if (file == NULL)
    {
        return false;
    }

    struct DirectoryEntry
    {
        uint32_t sibling;
        uint32_t child_dir;
        uint32_t child_file;
        uint32_t hash;
        uint32_t name_length;
    };

    for (;;)
    {
        DirectoryEntry entry;
        if (file->Seek(offset + DirOffset, CFileBase::begin) < 0)
        {
            return false;
        }
        if (!file->Read(&entry, sizeof(entry), DirOffset))
        {
            return false;
        }
        if (entry.name_length > 0)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (entry.child_file != ROMFS_ENTRY_EMPTY)
        {
            if (!ProcessFile(file, offset, FileOffset, entry.child_file, DataOffset))
            {
                return false;
            }
        }
        if (entry.child_dir != ROMFS_ENTRY_EMPTY)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (entry.sibling == ROMFS_ENTRY_EMPTY)
        {
            break;
        }
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return true;
}

bool RomFS::ProcessFile(CEncryptedFile * file, uint64_t offset, uint64_t FileOffset, uint64_t ThisFileOffset, uint64_t DataOffset)
{
    struct FileEntry 
    {
        uint32_t parent;
        uint32_t sibling;
        uint64_t offset;
        uint64_t size;
        uint32_t hash;
        uint32_t name_length;
    };

    for (;;)
    {
        FileEntry entry;
        if (file->Seek(offset + FileOffset + ThisFileOffset, CFileBase::begin) < 0)
        {
            return false;
        }
        if (!file->Read(&entry, sizeof(entry), FileOffset + ThisFileOffset))
        {
            return false;
        }
        std::string FileName(entry.name_length, '\0');
        if (entry.name_length > 0)
        {
            if (file->Seek(offset + FileOffset + ThisFileOffset + sizeof(FileEntry), CFileBase::begin) < 0)
            {
                return false;
            }
            if (!file->Read(&FileName[0], (uint32_t)FileName.size(), FileOffset + ThisFileOffset + sizeof(FileEntry)))
            {
                return false;
            }
        }

        FILE FileDetails;
        FileDetails.name = FileName;
        FileDetails.offset = entry.offset + DataOffset;
        FileDetails.size = entry.size;

        m_Files.push_back(FileDetails);

        if (entry.sibling == ROMFS_ENTRY_EMPTY)
        {
            break;
        }
        ThisFileOffset = entry.sibling;
    }
    return true;
}
