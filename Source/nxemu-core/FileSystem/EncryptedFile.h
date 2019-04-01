#pragma once
#include <Common\FileClass.h>
#include <vector>

class CEncryptedFile
{
public:
	CEncryptedFile(CFile & file);
	CEncryptedFile(const CEncryptedFile&);

	void SetEncrypted(const uint8_t * Key, size_t KeyLen, const std::vector<uint8_t>& IV, size_t BaseOffset);
	uint64_t GetLength() const;
	uint64_t Seek(int64_t lOff, CFileBase::SeekPosition nFrom);
	uint32_t Read(void* lpBuf, uint32_t nCount, size_t offset);

private:
	typedef std::vector<uint8_t> KeyData;

	CEncryptedFile(void);                             // Disable default constructor
	CEncryptedFile& operator=(const CEncryptedFile&); // Disable assignment

	void UpdateIV(size_t offset);

	CFile & m_file;
	bool m_Encrypted;
	KeyData m_Key;
	std::vector<uint8_t> m_IV;
	size_t m_BaseOffset;
};