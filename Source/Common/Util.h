#pragma once
#include <stdint.h>

class nxutil
{
public:
    static void Sleep(uint32_t timeout);

private:
    nxutil(void);
    nxutil(const nxutil&);
    nxutil& operator=(const nxutil&);
};
