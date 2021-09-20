#pragma once

class ShaderSamplerEntry 
{
public:
    ShaderSamplerEntry();
private:
    ShaderSamplerEntry & operator=(const ShaderSamplerEntry & );

};
typedef std::vector<ShaderSamplerEntry> ShaderSamplerEntryList;
