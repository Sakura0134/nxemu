#pragma once
#include <Common\stdtypes.h>

class CSwitchSystem;

class CDisplay
{
public:
    CDisplay(CSwitchSystem & System);
    ~CDisplay();

    bool Open(const char * name, uint32_t & DisplayId);

private:
    CDisplay(void);
    CDisplay(const CDisplay&);
    CDisplay& operator=(const CDisplay&);

    CSwitchSystem & m_System;
};