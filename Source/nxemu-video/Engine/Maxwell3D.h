#pragma once
#include <stdint.h>

__interface ISwitchSystem;
class CVideoMemory;

class CMaxwell3D
{
public:
    enum Method : uint32_t
    {
    };

    CMaxwell3D(ISwitchSystem & SwitchSystem, CVideoMemory & VideoMemory);
    ~CMaxwell3D();

    void CallMethod(Method Method, uint32_t Argument, bool Last);
private:
    CMaxwell3D();
    CMaxwell3D(const CMaxwell3D&);
    CMaxwell3D& operator=(const CMaxwell3D&);
    ISwitchSystem & m_SwitchSystem;
    CVideoMemory & m_VideoMemory;
};
