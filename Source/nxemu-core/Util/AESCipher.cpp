#include <nxemu-core\Util\AESCipher.h>
#include "mbedtls/cipher.h"
#include <algorithm>

CAESCipher::CAESCipher(const uint8_t * key, uint32_t key_bitlen, Mode mode) :
	m_CipherDec(NULL),
	m_valid(false)
{
	m_CipherDec = (void *)(new mbedtls_cipher_context_t);
	mbedtls_cipher_init((mbedtls_cipher_context_t*)m_CipherDec);
	if (mbedtls_cipher_setup((mbedtls_cipher_context_t*)m_CipherDec, mbedtls_cipher_info_from_type((const mbedtls_cipher_type_t)mode)))
	{
		return;
	}

	if (mbedtls_cipher_setkey((mbedtls_cipher_context_t*)m_CipherDec, key, key_bitlen * 8, MBEDTLS_DECRYPT))
	{
		return;
	}
	m_valid = true;
}

CAESCipher::~CAESCipher()
{
	if (m_CipherDec)
	{
		mbedtls_cipher_free((mbedtls_cipher_context_t*)m_CipherDec);
		delete (mbedtls_cipher_context_t*)m_CipherDec;
		m_CipherDec = NULL;
	}
}

bool CAESCipher::Decrypt(const uint8_t * Source, uint8_t * Dest, uint32_t Size)
{
	if (!m_valid)
	{
		return false;
	}
	bool SrcEqualsDst = Source == Dest;
	std::auto_ptr<uint8_t> temp_dest(NULL);
	if (SrcEqualsDst)
	{
		temp_dest.reset(new uint8_t[Size]);
		Dest = temp_dest.get();
		if (Dest == NULL)
		{
			return false;
		}
	}

	size_t OutLen = 0;
	mbedtls_cipher_reset((mbedtls_cipher_context_t*)m_CipherDec);

	if (mbedtls_cipher_get_cipher_mode((mbedtls_cipher_context_t*)m_CipherDec) == MBEDTLS_MODE_XTS)
	{
		mbedtls_cipher_update((mbedtls_cipher_context_t*)m_CipherDec, (const uint8_t *)Source, Size, (uint8_t *)Dest, &OutLen);
	}
	else
	{
		unsigned int blk_size = mbedtls_cipher_get_block_size((mbedtls_cipher_context_t*)m_CipherDec);
		for (int32_t offset = 0; (uint32_t)offset < Size; offset += blk_size)
		{
			int32_t len = ((uint32_t)(Size - offset) > blk_size) ? blk_size : (uint32_t)(Size - offset);
			mbedtls_cipher_update((mbedtls_cipher_context_t*)m_CipherDec, (const uint8_t *)Source + offset, len, (uint8_t *)Dest + offset, &OutLen);
		}
	}
	mbedtls_cipher_finish((mbedtls_cipher_context_t*)m_CipherDec, NULL, NULL);

	if (SrcEqualsDst)
	{
		memcpy((void*)Source, Dest, Size);
	}
	return true;
}

bool CAESCipher::Transcode(const uint8_t * Source, uint8_t * Dest, uint32_t Size)
{
	if (!m_valid)
	{
		return false;
	}
	mbedtls_cipher_reset((mbedtls_cipher_context_t*)m_CipherDec);

	std::size_t written = 0;
	if (mbedtls_cipher_get_cipher_mode((mbedtls_cipher_context_t*)m_CipherDec) == MBEDTLS_MODE_XTS)
	{
		mbedtls_cipher_update((mbedtls_cipher_context_t*)m_CipherDec, Source, Size, Dest, &written);
		if (written != Size)
		{
			mbedtls_cipher_finish((mbedtls_cipher_context_t*)m_CipherDec, nullptr, nullptr);
			return false;
		}
	}
	else 
	{
		uint32_t block_size = mbedtls_cipher_get_block_size((mbedtls_cipher_context_t*)m_CipherDec);
		for (uint32_t offset = 0; offset < Size; offset += block_size)
		{
			uint32_t length = std::min<uint32_t>(block_size, Size - offset);
			if (length < block_size)
			{
				std::vector<uint8_t> block(block_size);
				std::memcpy(block.data(), Source + offset, length);
				mbedtls_cipher_update((mbedtls_cipher_context_t*)m_CipherDec, block.data(), (uint32_t)block.size(), block.data(), &written);
				std::memcpy(Dest + offset, block.data(), length);
				if (written == block.size())
				{
					written = length;
				}
			}
			else
			{
				mbedtls_cipher_update((mbedtls_cipher_context_t*)m_CipherDec, Source + offset, length, Dest + offset, &written);
			}
			if (written != length) 
			{
				mbedtls_cipher_finish((mbedtls_cipher_context_t*)m_CipherDec, nullptr, nullptr);
				return false;
			}
		}
	}
	mbedtls_cipher_finish((mbedtls_cipher_context_t*)m_CipherDec, nullptr, nullptr);
	return true;
}

bool CAESCipher::XTSTranscode(const uint8_t * Source, uint8_t * Dest, uint32_t Size, size_t SectorId, uint32_t SectorSize)
{
	for (std::size_t i = 0; i < Size; i += SectorSize) 
	{
		std::vector<uint8_t> iv = CalculateNintendoTweak(SectorId++);
		SetIV(iv.data(), iv.size());
		if (!Transcode(Source + i, Dest + i, SectorSize))
		{
			return false;
		}
	}
	return true;
}

void CAESCipher::SetIV(const uint8_t * iv, size_t iv_len)
{
	if (m_valid && m_CipherDec != NULL)
	{
		mbedtls_cipher_set_iv((mbedtls_cipher_context_t*)m_CipherDec, iv, iv_len);
	}
}

std::vector<uint8_t> CAESCipher::CalculateNintendoTweak(size_t sector_id)
{
	std::vector<uint8_t> out(0x10);
	for (uint32_t i = 0xF; i <= 0xF; i--)
	{
		out[i] = sector_id & 0xFF;
		sector_id >>= 8;
	}
	return out;
}
