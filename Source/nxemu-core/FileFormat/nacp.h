#pragma once
#include <nxemu-core\FileSystem\EncryptedFile.h>
#include <stdint.h>
#include <string>

class NACP
{
    struct LanguageEntry
    {
        uint8_t application_name[0x200];
        uint8_t developer_name[0x100];
    };

    struct NACPInfo 
    {
        LanguageEntry language_entries[16];
        uint8_t isbn[0x25];
        uint8_t startup_user_account;
        uint8_t padding[2];
        uint32_t application_attribute;
        uint32_t supported_languages;
        uint32_t parental_control;
        bool screenshot_enabled;
        uint8_t video_capture_mode;
        bool data_loss_confirmation;
        uint8_t padding1;
        uint64_t title_id;
        uint8_t rating_age[0x20];
        uint8_t version_string[0x10];
        uint64_t dlc_base_title_id;
        uint64_t title_id_2;
        uint64_t normal_save_data_size;
        uint64_t journal_sava_data_size;
        uint8_t padding2[0x18];
        uint64_t product_code;
        uint64_t local_communication[0x8];
        uint8_t logo_type;
        uint8_t logo_handling;
        bool runtime_add_on_content_install;
        uint8_t padding4[0x5];
        uint64_t title_id_update;
        uint8_t bcat_passphrase[0x40];
        uint8_t padding5[0xEC0];
    };

public:
    enum Language : uint8_t 
    {
        AmericanEnglish = 0,
        BritishEnglish = 1,
        Japanese = 2,
        French = 3,
        German = 4,
        LatinAmericanSpanish = 5,
        Spanish = 6,
        Italian = 7,
        Dutch = 8,
        CanadianFrench = 9,
        Portugese = 10,
        Russian = 11,
        Korean = 12,
        Taiwanese = 13,
        Chinese = 14,

        Default = 255,
    };

    NACP(void);

    bool Load(CEncryptedFile * EncryptedFile, uint64_t Offset, uint64_t FileOffset, uint64_t FileSize);
    std::string GetApplicationName(Language language = Language::Default) const;

private:
    NACP(const NACP&);             // Disable copy constructor
    NACP& operator=(const NACP&);  // Disable assignment

    const LanguageEntry& GetLanguageEntry(Language language) const;
        
    NACPInfo m_Info;
};