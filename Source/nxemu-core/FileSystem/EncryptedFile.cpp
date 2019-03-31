#include <nxemu-core\FileSystem\EncryptedFile.h>
#include <nxemu-core\Util\AESCipher.h>
#include <nxemu-core\SystemGlobals.h>

CEncryptedFile::CEncryptedFile(CFile & file) :
	m_file(file),
	m_Encrypted(false),
	m_Key({}),
	m_IV({}),
	m_BaseOffset(0)
{
}

CEncryptedFile::CEncryptedFile(const CEncryptedFile& rhs) :
	m_file(rhs.m_file),
	m_Encrypted(rhs.m_Encrypted),
	m_Key(rhs.m_Key),
	m_IV(rhs.m_IV),
	m_BaseOffset(rhs.m_BaseOffset)
{
}

uint64_t CEncryptedFile::GetLength() const
{
	return m_file.GetLength();
}

uint64_t CEncryptedFile::Seek(int64_t lOff, CFileBase::SeekPosition nFrom)
{
	return m_file.Seek(lOff, nFrom);
}

uint32_t CEncryptedFile::Read(void* lpBuf, uint32_t nCount, size_t offset)
{
	if (!m_Encrypted)
	{
		return m_file.Read(lpBuf, nCount);
	}

	CAESCipher Cipher(m_Key.data(), (uint32_t)m_Key.size(), CAESCipher::CIPHER_AES_128_CTR);
	const auto sector_offset = offset & 0xF;
	if (sector_offset != 0)
	{
		std::vector<uint8_t> data(0x10);
		size_t read = 0x10 - sector_offset;
		UpdateIV(m_BaseOffset + offset - sector_offset);
		Cipher.SetIV(m_IV.data(), m_IV.size());
		m_file.Seek(sector_offset * -1, CFileBase::current);
		uint32_t length = m_file.Read(data.data(), (uint32_t)data.size());
		if (!Cipher.Transcode(data.data(), data.data(), (uint32_t)data.size()))
		{
			g_Notify->BreakPoint(__FILE__, __LINE__);
			return 0;
		}
		if (length + sector_offset < 0x10) 
		{
			g_Notify->BreakPoint(__FILE__, __LINE__);
			return 0;
		}
		if (nCount <= read)
		{
			std::memcpy(lpBuf, data.data() + sector_offset, nCount);
			return nCount;
		}
		std::memcpy(lpBuf, data.data() + sector_offset, read);
		return Read((uint8_t*)lpBuf + read, (uint32_t)(nCount - read), (uint32_t)(offset + read)) + (uint32_t)read;
	}

	UpdateIV(m_BaseOffset + offset);
	Cipher.SetIV(m_IV.data(), m_IV.size());
	uint32_t length = m_file.Read(lpBuf, nCount);
	if (!Cipher.Transcode((const uint8_t*)lpBuf, (uint8_t*)lpBuf, nCount))
	{
		g_Notify->BreakPoint(__FILE__, __LINE__);
		return 0;
	}
	return length;
}

void CEncryptedFile::UpdateIV(size_t offset)
{
	offset >>= 4;
	for (uint32_t i = 0; i < 8; i++)
	{
		m_IV[16 - i - 1] = offset & 0xFF;
		offset >>= 8;
	}
}
