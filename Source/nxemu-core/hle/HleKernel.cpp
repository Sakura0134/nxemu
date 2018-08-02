#include <nxemu-core\hle\HleKernel.h>
#include <nxemu-core\SystemGlobals.h>
#include <nxemu-core\Trace.h>
#include <Common\StdString.h>

CHleKernel::CHleKernel(CSwitchSystem & System) :
    m_System(System)
{
}

CHleKernel::~CHleKernel()
{
    for (size_t i = 0, n = m_SystemThreads.size(); i < n; i++)
    {
        delete m_SystemThreads[(int)i];
    }
    m_SystemThreads.clear();
}