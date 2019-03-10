#pragma once

enum SwitchFileType
{
	sft_Unknown,
	sft_XCI,
};

SwitchFileType IdentifyFileType(const char * File);
