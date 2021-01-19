#include <nxemu-core\hle\Display\Nvidia\NvEvents.h>
#include <nxemu-core\hle\Kernel\KEvent.h>
#include <nxemu-core\SystemGlobals.h>

CNvEvents::CNvEvents()
{
}

CNvEvents::~CNvEvents()
{
}


CKernelObjectPtr CNvEvents::GetEvent(uint32_t /*EventId*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return CKernelObjectPtr(nullptr);
}
