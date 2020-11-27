#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Trace.h>
#include <lz4\lz4.h>
#include <Common\FileClass.h>
#include <Common\sha256.h>
#include <Common\StdString.h>

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

typedef struct {
    uint32_t name;
    uint8_t info;
    uint8_t other;
    uint16_t shndx;
    uint64_t value;
    uint64_t size;
} ELF_SYM;

bool LoadNsoSegement(CProcessMemory & ProcessMemory, uint64_t LoadAddress, CEncryptedFile &EncryptedFile, uint64_t FileOffset, uint32_t CompressSize, uint32_t UnCompressedSize, uint32_t SegmentSize, uint32_t SegmentOffset, uint32_t BssSize, bool Compressed, MemoryPermission MemoryPermission, bool Check, const uint8_t * Hash, MemoryType memType)
{
	if (!Compressed)
	{
		g_Notify->BreakPoint(__FILE__, __LINE__);
		return false;
	}
	std::auto_ptr<uint8_t> CompressedData(new uint8_t[CompressSize]);
	if (EncryptedFile.Seek(FileOffset + SegmentOffset, CFileBase::begin) < 0)
	{
		return false; 
	}
	if (EncryptedFile.Read(CompressedData.get(), CompressSize, SegmentOffset) != CompressSize)
	{
		return false; 
	}

	WriteTrace(TraceGameFile, TraceVerbose, "MapMemory: Address: 0x%I64X Size: 0x%08X (0x%08X) BssSize: 0x%08X (0x%08X) MemoryPermission: %s type: %X", LoadAddress, SegmentSize, (uint32_t)CPageTable::PageRoundUp(SegmentSize), BssSize, CPageTable::PageRoundUp(BssSize), CPageTable::MemoryPermissionName(MemoryPermission), memType);
	uint8_t * Segment = ProcessMemory.MapMemory(LoadAddress, (uint32_t)(CPageTable::PageRoundUp(SegmentSize) + CPageTable::PageRoundUp(BssSize)), MemoryPermission, memType);
	if (Segment == nullptr)
	{
		g_Notify->BreakPoint(__FILE__, __LINE__);
		return false;
	}

	uint32_t bytes_uncompressed = LZ4_decompress_safe((const char *)CompressedData.get(), (char *)Segment, CompressSize, UnCompressedSize);
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

bool CSwitchSystem::LoadNSOModule(uint64_t offset, CEncryptedFile &EncryptedFile, const CPartitionFilesystem::VirtualFile * file, uint64_t base_addr, uint64_t &end_addr)
{
	if (file == nullptr)
	{
		return false;
	}
	WriteTrace(TraceGameFile, TraceDebug, "Start (NsoFile: %s base_addr: 0x%I64X)", file->Name.c_str(), base_addr);
	g_Notify->DisplayMessage(0, stdstr_f("%s: %s", GS(MSG_LOADING), file->Name.c_str()).c_str());

	NSO_HEADER header;
	if (EncryptedFile.Seek(offset + file->Offset, CFileBase::begin) < 0) { return false; }
	if (EncryptedFile.Read(&header, sizeof(header), file->Offset) != sizeof(header)) { return false; }

	WriteTrace(TraceGameFile, TraceDebug, "Text Section - MemoryOffset: 0x%I64X DecompressedSize: 0x%I64X", header.Text.MemoryOffset, header.Text.DecompressedSize);
	WriteTrace(TraceGameFile, TraceDebug, "Read only data Section - MemoryOffset: 0x%I64X DecompressedSize: 0x%I64X", header.rodata.MemoryOffset, header.rodata.DecompressedSize);
	WriteTrace(TraceGameFile, TraceDebug, "Data Section - MemoryOffset: 0x%I64X DecompressedSize: 0x%I64X", header.data.MemoryOffset, header.data.DecompressedSize);

	if (!LoadNsoSegement(m_ProcessMemory, base_addr + header.Text.MemoryOffset, EncryptedFile, offset, header.TextCompressedSize, header.Text.DecompressedSize, header.Text.DecompressedSize, (uint32_t)(header.Text.FileOffset + file->Offset), 0, header.Flags.TextCompressed, MemoryPermission_ReadExecute, header.Flags.TextCheck, header.TextHash, MemoryType_CodeStatic)) { return false; }
	if (!LoadNsoSegement(m_ProcessMemory, base_addr + header.rodata.MemoryOffset, EncryptedFile, offset, header.RodataCompressedSize, header.rodata.DecompressedSize, header.rodata.DecompressedSize, (uint32_t)(header.rodata.FileOffset + file->Offset), 0, header.Flags.RoDataCompressed, MemoryPermission_Read, header.Flags.RoDataCheck, header.RodataHash, MemoryType_CodeMutable)) { return false; }
	if (header.data.MemoryOffset < header.rodata.MemoryOffset || header.data.MemoryOffset < header.rodata.MemoryOffset)
	{
		g_Notify->BreakPoint(__FILE__, __LINE__);
		return false;
	}

	uint32_t Mod0Offset;
	if (!m_ProcessMemory.Read32(base_addr + header.Text.MemoryOffset + 4, Mod0Offset)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
	WriteTrace(TraceGameFile, TraceDebug, "Mod0Offset Location: 0x%X Mod0Offset: 0x%X", base_addr + header.Text.MemoryOffset + 4, Mod0Offset);

	MOD_HEADER mod_info = { 0 };
	uint32_t BssSize = header.BssSize;
	if (Mod0Offset != 0)
	{
		if (!m_ProcessMemory.ReadBytes(base_addr + Mod0Offset, (uint8_t *)&mod_info, sizeof(mod_info))) { g_Notify->BreakPoint(__FILE__, __LINE__); }
		if (mod_info.Magic != *((uint32_t *)(&"MOD0")))
		{
			g_Notify->BreakPoint(__FILE__, __LINE__);
			return false;
		}
		WriteTrace(TraceGameFile, TraceDebug, "Mod0Offset: 0x%08X BssStartOffset: 0x%08X BssEndOffset: 0x%08X", Mod0Offset, mod_info.BssStartOffset, mod_info.BssEndOffset);
		BssSize = mod_info.BssEndOffset - mod_info.BssStartOffset;
		if (BssSize > header.BssSize)
		{
			g_Notify->BreakPoint(__FILE__, __LINE__);
			return false;
		}
	}
	WriteTrace(TraceGameFile, TraceDebug, "BssSize: 0x%I64X", BssSize);
	if (!LoadNsoSegement(m_ProcessMemory, base_addr + header.data.MemoryOffset, EncryptedFile, offset, header.DataCompressedSize, header.data.DecompressedSize, header.data.DecompressedSize, (uint32_t)(header.data.FileOffset + file->Offset), BssSize, header.Flags.DataCompressed, MemoryPermission_ReadWrite, header.Flags.DataCheck, header.DataHash, MemoryType_CodeMutable)) { return false; }

	if (Mod0Offset != 0)
	{
		enum ElfDynamicTag : uint64_t
		{
			DT_nullptr = 0,
			DT_PLTRELSZ = 2,
			DT_PLTGOT = 3,
			DT_HASH = 4,
			DT_STRTAB = 5,
			DT_SYMTAB = 6,
			DT_RELA = 7,
			DT_RELASZ = 8,
			DT_RELAENT = 9,
			DT_STRSZ = 10,
			DT_SYMENT = 11,
			DT_INIT = 12,
			DT_FINI = 13,
			DT_PLTREL = 20,
			DT_JMPREL = 23,
			DT_GNU_HASH = 0x6ffffef5,
			DT_RELACOUNT = 0x6ffffff9,
		};
		typedef std::map<ElfDynamicTag, uint64_t> ElfDynamicTags;

		ElfDynamicTags DynamicTags;

		for (uint64_t DynamicOffset = mod_info.DynamicOffset + base_addr + Mod0Offset;; DynamicOffset += 0x10)
		{
			uint64_t TagVal, Value;
			if (!m_ProcessMemory.Read64(DynamicOffset, TagVal)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
			if (!m_ProcessMemory.Read64(DynamicOffset + 8, Value)) { g_Notify->BreakPoint(__FILE__, __LINE__); }

			if ((ElfDynamicTag)TagVal == DT_nullptr)
			{
				break;
			}

			DynamicTags.insert(ElfDynamicTags::value_type((ElfDynamicTag)TagVal, Value));
		}

		ElfDynamicTags::const_iterator StrTblItr = DynamicTags.find(DT_STRTAB);
		ElfDynamicTags::const_iterator SymTblItr = DynamicTags.find(DT_SYMTAB);
		ElfDynamicTags::const_iterator SymEntSizeItr = DynamicTags.find(DT_SYMENT);

		if (StrTblItr != DynamicTags.end() && SymTblItr != DynamicTags.end() && SymEntSizeItr != DynamicTags.end())
		{
			uint64_t StrTblAddr = base_addr + StrTblItr->second;
			uint64_t SymTblAddr = base_addr + SymTblItr->second;
			uint64_t SymEntSize = SymEntSizeItr->second;

			for (; SymTblAddr < StrTblAddr; SymTblAddr += SymEntSize)
			{
				ELF_SYM symbol;
				if (!m_ProcessMemory.ReadBytes(SymTblAddr, (uint8_t *)&symbol, sizeof(symbol))) { g_Notify->BreakPoint(__FILE__, __LINE__); }

				if (symbol.value != 0)
				{
					std::string name;
					if (!m_ProcessMemory.ReadCString(StrTblAddr + symbol.name, name)) { g_Notify->BreakPoint(__FILE__, __LINE__); }
				}
			}
		}

		end_addr = base_addr + header.data.MemoryOffset + CPageTable::PageRoundUp(header.data.DecompressedSize) + CPageTable::PageRoundUp(BssSize);
	}
	else
	{
		g_Notify->BreakPoint(__FILE__, __LINE__);
	}
	WriteTrace(TraceGameFile, TraceDebug, "Done (Res: True end_addr: 0x%I64X)", end_addr);
	return true;
}
