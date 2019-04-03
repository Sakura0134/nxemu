#include <nxemu-core\FileFormat\ProgramMetadata.h>

CProgramMetadata::CProgramMetadata(uint64_t offset, CEncryptedFile &EncryptedFile, const CPartitionFilesystem::VirtualFile * npdm)
{
	EncryptedFile.Seek(offset + npdm->Offset, CFileBase::begin);
	EncryptedFile.Read(&m_Header, sizeof(m_Header), npdm->Offset);
}
