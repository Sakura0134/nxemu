#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\SystemGlobals.h>
#include <lz4\lz4.h>
#include <Common\FileClass.h>
#include <Common\sha256.h>

typedef struct
{
    uint32_t Magic;
    uint32_t Version;
    uint32_t Reserved;
    struct 
    {
        unsigned TextCompressed : 1;
        unsigned RoDataCompressed : 1;
        unsigned DataCompressed : 1;
        unsigned TextCheck : 1;
        unsigned RoDataCheck : 1;
        unsigned DataCheck : 1;
        unsigned notused : 26;
    } Flags;
    struct 
    {
        uint32_t FileOffset;
        uint32_t MemoryOffset;
        uint32_t DecompressedSize;
    } Text;
    uint32_t ModuleOffset;
    struct 
    {
        uint32_t FileOffset;
        uint32_t MemoryOffset;
        uint32_t DecompressedSize;
    } rodata;
    uint32_t ModuleFileSize;
    struct 
    {
        uint32_t FileOffset;
        uint32_t MemoryOffset;
        uint32_t DecompressedSize;
    } data;
    uint32_t BssSize;
    uint8_t BuildId[0x20];
    uint32_t TextCompressedSize;
    uint32_t RodataCompressedSize;
    uint32_t DataCompressedSize;
    uint8_t Padding[0x1C];
    struct
    {
        uint32_t DataOffset;
        uint32_t Size;
    } api_info;
    struct
    {
        uint32_t DataOffset;
        uint32_t Size;
    } dynstr;
    struct
    {
        uint32_t DataOffset;
        uint32_t Size;
    } dynsym;
    uint8_t TextHash[0x20];
    uint8_t RodataHash[0x20];
    uint8_t DataHash[0x20];
} NSO_HEADER;

typedef struct
{
    uint32_t Magic;
    uint32_t DynamicOffset;
    uint32_t BssStartOffset;
    uint32_t BssEndOffset;
    uint32_t EhHdrStartOffset;
    uint32_t EhHdrEndOffset;
    uint32_t ModuleObjectOffset;
} MOD_HEADER;

bool IsNsoFile(const CPath & NsoFile)
{
    if (!NsoFile.Exists())
    {
        return false;
    }

    CFile ReadFile(NsoFile, CFileBase::modeRead);
    if (!ReadFile.IsOpen())
    {
        return false;
    }

    NSO_HEADER header;
    ReadFile.SeekToBegin();
    if (!ReadFile.Read(&header, sizeof(header)))
    {
        return false;
    }
    if (header.Magic != *((uint32_t *)(&"NSO0")))
    {
        return false;
    }
    return true;
}

bool LoadNsoSegement(CProcessMemory & ProcessMemory, uint64_t LoadAddress, CFile & NsoFile, uint32_t CompressSize, uint32_t UnCompressedSize, uint32_t SegmentSize, uint32_t SegmentOffset, bool Compressed, PageTable::MemoryPermission MemoryPermission, bool Check, const uint8_t * Hash, CProcessMemory::MemoryType memType)
{
    if (!Compressed)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }
    std::auto_ptr<uint8_t> CompressedData(new uint8_t[CompressSize]);

    if (NsoFile.Seek(SegmentOffset, CFile::begin) == -1)
    {
        return false;
    }
    if (!NsoFile.Read(CompressedData.get(), CompressSize))
    {
        return false;
    }
    uint8_t * Segment = ProcessMemory.MapMemory(LoadAddress, (uint32_t)PageTable::PageRoundUp(SegmentSize), MemoryPermission, memType);
    if (Segment == NULL)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }

    uint32_t bytes_uncompressed = LZ4_decompress_safe((const char *)CompressedData.get(),(char *)Segment, CompressSize, UnCompressedSize);
    if (bytes_uncompressed != UnCompressedSize)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }

    if (Check)
    {
        unsigned char digest[SHA256::DIGEST_SIZE];
        memset(digest, 0, SHA256::DIGEST_SIZE);

        SHA256 ctx = SHA256();
        ctx.init();
        ctx.update(Segment, bytes_uncompressed);
        ctx.final(digest);

        if (memcmp(Hash, digest, sizeof(digest) / sizeof(digest[0])) != 0)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return false;
        }
    }
    return true;
}

bool CSwitchSystem::LoadNsoFile(const CPath & NsoFile, uint64_t base_addr, uint64_t &end_addr)
{
    CFile ReadFile(NsoFile, CFileBase::modeRead);
    if (!ReadFile.IsOpen())
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }

    NSO_HEADER header;
    ReadFile.SeekToBegin();
    if (!ReadFile.Read(&header, sizeof(header)))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }

    if (!LoadNsoSegement(m_ProcessMemory, base_addr + header.Text.MemoryOffset, ReadFile, header.TextCompressedSize, header.Text.DecompressedSize, header.Text.DecompressedSize, header.Text.FileOffset, header.Flags.TextCompressed, PageTable::ReadExecute, header.Flags.TextCheck, header.TextHash, CProcessMemory::MemoryType_CodeStatic)) { return false; }
    if (!LoadNsoSegement(m_ProcessMemory, base_addr + header.rodata.MemoryOffset, ReadFile, header.RodataCompressedSize, header.rodata.DecompressedSize, header.rodata.DecompressedSize, header.rodata.FileOffset, header.Flags.RoDataCompressed, PageTable::Read, header.Flags.RoDataCheck, header.RodataHash, CProcessMemory::MemoryType_CodeMutable)) { return false; }
    if (!LoadNsoSegement(m_ProcessMemory, base_addr + header.data.MemoryOffset, ReadFile, header.DataCompressedSize, header.data.DecompressedSize, header.data.DecompressedSize + header.BssSize, header.data.FileOffset, header.Flags.DataCompressed, PageTable::ReadWrite, header.Flags.DataCheck, header.DataHash, CProcessMemory::MemoryType_CodeMutable)) { return false; }

    if (header.data.MemoryOffset < header.rodata.MemoryOffset || header.data.MemoryOffset < header.rodata.MemoryOffset)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }

    uint32_t Mod0Offset;
    if (!m_ProcessMemory.Read32(base_addr + header.Text.MemoryOffset + 4, Mod0Offset)) { g_Notify->BreakPoint(__FILE__, __LINE__); }

    if (Mod0Offset != 0)
    {
        MOD_HEADER mod_info;
        if (!m_ProcessMemory.ReadBytes(base_addr + Mod0Offset, (uint8_t *)&mod_info, sizeof(mod_info))) { g_Notify->BreakPoint(__FILE__, __LINE__); }
        if (mod_info.Magic != *((uint32_t *)(&"MOD0")))
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return false;
        }
        end_addr = base_addr + header.data.MemoryOffset + PageTable::PageRoundUp(header.data.DecompressedSize + header.BssSize);
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return true;
}
