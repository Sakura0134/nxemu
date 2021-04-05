#pragma once
#include <stdint.h>
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

	bool Decrypt(const uint8_t * Source, uint8_t * Dest, uint32_t Size);
	bool Transcode(const uint8_t * Source, uint8_t * Dest, uint32_t Size);
	bool XTSTranscode(const uint8_t * Source, uint8_t * Dest, uint32_t Size, size_t SectorId, uint32_t SectorSize);
	void SetIV(const uint8_t * iv, size_t iv_len);

private:
	CAESCipher(void);                           // Disable default constructor
	CAESCipher(const CAESCipher&);              // Disable copy constructor
	CAESCipher& operator=(const CAESCipher&);   // Disable assignment
	
	static std::vector<uint8_t> CalculateNintendoTweak(size_t sector_id);

	void * m_CipherDec;
	bool m_valid;
};