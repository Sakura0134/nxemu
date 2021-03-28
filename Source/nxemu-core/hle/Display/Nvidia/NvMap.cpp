#include <nxemu-core\hle\Display\Nvidia\NvMap.h>
#include <nxemu-core\SystemGlobals.h>

CNvMap::CNvMap(void) :
	m_NextNvHandle(1)
{
}

CNvMap::~CNvMap()
{
}

nvResult CNvMap::Ioctl(nvIoctl Ioctl, const CIPCRequest::RequestBuffer & InData, CIPCRequest::RequestBuffer & OutData)
{
    if (Ioctl.Group == nvIoctl::NvMapCustomMagic)
    {
        switch (Ioctl.Cmd)
        {
        case NVMAP_IOC_CREATE:
            Create(InData, OutData);
            break;
        case NVMAP_IOC_ALLOC:
            Alloc(InData, OutData);
            break;
        default:
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return nvResult_Success;
}

void CNvMap::Create(const CIPCRequest::RequestBuffer & InData, CIPCRequest::RequestBuffer & OutData)
{
    struct NvMapCreate
    {
        uint32_t Size;
        uint32_t Handle;
    };

    if (InData.size() < sizeof(NvMapCreate))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    OutData.resize(sizeof(NvMapCreate));
    memcpy(OutData.data(), InData.data(), InData.size());
    NvMapCreate & Args = *((NvMapCreate *)OutData.data());
    uint32_t Size = (Args.Size + (PageSize - 1)) & ~(PageSize - 1);

    uint32_t Handle = m_NextNvHandle++;
    m_Handles.emplace(Handle, Size);
    Args.Handle = Handle;
}

void CNvMap::Alloc(const CIPCRequest::RequestBuffer & InData, CIPCRequest::RequestBuffer & OutData)
{
    struct NvMapAlloc
    {
        uint32_t Handle;
        uint32_t HeapMask;
        uint32_t Flags;
        uint32_t Align;
        uint64_t Kind;
        uint64_t Address;
    };

    if (InData.size() < sizeof(NvMapAlloc))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    OutData.resize(sizeof(NvMapAlloc));
    memcpy(OutData.data(), InData.data(), InData.size());
    NvMapAlloc & Args = *((NvMapAlloc *)OutData.data());
    NvMapHandleMap::iterator itr = m_Handles.find(Args.Handle);
    if (itr == m_Handles.end())
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    NvMapHandle & Map = itr->second;
    if ((Args.Align & (Args.Align - 1)) != 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    if (Args.Align < PageSize)
    {
        Args.Align = PageSize;
    }

    if (!Map.Allocated())
    {
        uint32_t Size = (uint32_t)CPageTable::PageRoundUp(Map.Size());
        if (Args.Address == 0 || Map.Address() != 0)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        Map.Allocate(Args.Address, Size, Args.Align, (uint32_t)Args.Kind);
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

NvMapHandle::NvMapHandle(uint32_t Size) :
    m_Address(0),
    m_Size(Size),
    m_Allocated(false)
{
}

void NvMapHandle::Allocate(int64_t Address, uint32_t Size, uint32_t Align, uint32_t Kind)
{
    if (m_Allocated)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    m_Allocated = true;
    m_Address = Address;
    m_Size = Size;
    m_Align = Align;
    m_Kind = Kind;
}
