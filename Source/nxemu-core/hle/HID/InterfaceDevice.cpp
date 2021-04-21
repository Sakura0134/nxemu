#include <nxemu-core\hle\HID\InterfaceDevice.h>
#include <nxemu-core\SystemGlobals.h>

CInterfaceDevice::CInterfaceDevice(CHleKernel& Kernel) :
    m_Kernel(Kernel),
    m_SharedMem(nullptr)
{
}

CInterfaceDevice::~CInterfaceDevice()
{

}

CKernelObjectPtr CInterfaceDevice::GetSharedMemory()
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return nullptr;
}
