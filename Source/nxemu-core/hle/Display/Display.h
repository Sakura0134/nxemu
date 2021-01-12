#pragma once
#include <nxemu-core\hle\Display\DisplayDevice.h>
#include <Common\stdtypes.h>
#include <memory>
#include <vector>

class CSwitchSystem;

class CDisplay
{
    typedef std::vector<std::shared_ptr<CDisplayDevice>> DisplayList;

public:
    CDisplay(CSwitchSystem & System);
    ~CDisplay();

    bool Open(const char * name, uint32_t & DisplayId);

private:
    CDisplay(void);
    CDisplay(const CDisplay&);
    CDisplay& operator=(const CDisplay&);

    CSwitchSystem & m_System;
    DisplayList m_DisplayList;
};