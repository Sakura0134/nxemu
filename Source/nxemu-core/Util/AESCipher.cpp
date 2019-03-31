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

void CAESCipher::SetIV(const uint8_t * iv, size_t iv_len)
{
	if (m_valid && m_CipherDec != NULL)
	{
		mbedtls_cipher_set_iv((mbedtls_cipher_context_t*)m_CipherDec, iv, iv_len);
	}
}
