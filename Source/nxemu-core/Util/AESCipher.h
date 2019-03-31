#pragma once
#include <Common\stdtypes.h>
#include <vector>

class CAESCipher
{
public:
	enum Mode 
	{
		CIPHER_AES_128_ECB = 2,
		CIPHER_AES_128_CTR = 11,
		CIPHER_AES_128_XTS = 70,
	};

	CAESCipher(const uint8_t * key, uint32_t key_bitlen, Mode mode);
	~CAESCipher();

	bool Transcode(const uint8_t * Source, uint8_t * Dest, uint32_t Size);
	void SetIV(const uint8_t * iv, size_t iv_len);

private:
	CAESCipher(void);                           // Disable default constructor
	CAESCipher(const CAESCipher&);              // Disable copy constructor
	CAESCipher& operator=(const CAESCipher&);   // Disable assignment

	void * m_CipherDec;
	bool m_valid;
};