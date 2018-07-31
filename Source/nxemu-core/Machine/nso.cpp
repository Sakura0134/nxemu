#include <Common\path.h>
#include <Common\FileClass.h>

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
