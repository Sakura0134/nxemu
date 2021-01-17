#include <nxemu-core\hle\Services\vi\IHOSBinderDriver.h>
#include <nxemu-core\hle\Services\vi\ViParcel.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\SystemGlobals.h>

class IGBPConnectResponseParcel : 
    public ViParcel 
{
public:
    IGBPConnectResponseParcel(uint32_t width, uint32_t height)  :
        m_data({ 0 })
    {
        m_data.width = width;
        m_data.height = height;
    }

protected:
    void SerializeData()
    {
        Write(&m_data, sizeof(m_data));
    }

private:
    struct Data 
    {
        uint32_t width;
        uint32_t height;
        uint32_t transform_hint;
        uint32_t num_pending_buffers;
        uint32_t status;
    };

    Data m_data;
};


CKernelObjectPtr IHOSBinderDriver::CreateInstance(CSwitchSystem & System)
{
    return CKernelObjectPtr(new IHOSBinderDriver(System));
}

IHOSBinderDriver::IHOSBinderDriver(CSwitchSystem & System) :
    CService(System)
{
}

IHOSBinderDriver::~IHOSBinderDriver()
{
}

bool IHOSBinderDriver::Connect(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return false;
}

ResultCode IHOSBinderDriver::CallMethod(CIPCRequest & Request)
{
    switch (Request.RequestHeader().Command)
    {
    case Method_TransactParcel:
        return ViTransactParcel(Request);
    case Method_AdjustRefcount:
        ViAdjustRefcount();
        break;
    case Method_GetNativeHandle:
        ViGetNativeHandle(Request);
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        break;
    }
    return RESULT_SUCCESS;
}

void IHOSBinderDriver::ViAdjustRefcount(void)
{
    //stubbed
}

void IHOSBinderDriver::ViGetNativeHandle(CIPCRequest & Request)
{
    const CIPCRequest::REQUEST_DATA & RequestData = Request.RequestData();
    if (RequestData.size() < 0x8)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    uint32_t Id = *((uint32_t *)&RequestData[0]);
    CDisplay & Display = m_System.HleKernel().GetDisplay();
    CBufferQueue * BufferQueue = Display.FindBufferQueue(Id);
    if (BufferQueue == nullptr)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    CHleKernel & HleKernel = Request.SwitchSystem().HleKernel();
    uint32_t handle = HleKernel.AddKernelObject(BufferQueue->WaitEvent());
    Request.AddResponseHandlesToCopy(handle);
}
ResultCode IHOSBinderDriver::ViTransactParcel(CIPCRequest & Request)
{
    const CIPCRequest::REQUEST_DATA & RequestData = Request.RequestData();
    if (RequestData.size() < 0x8)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return RESULT_SUCCESS;
    }
    CIPCRequest::RequestBuffer ReadData;
    if (!Request.ReadBuffer(ReadData))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return RESULT_SUCCESS;
    }

    if (ReadData.size() < 8)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    uint32_t DataSize = *(uint32_t *)&ReadData[0];
    uint32_t DataOffset = *(uint32_t *)&ReadData[4];

    if ((uint64_t)(DataOffset + DataSize) > ReadData.size() || ReadData.size() + DataOffset <= sizeof(uint32_t))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return RESULT_SUCCESS;
    }
    uint32_t StrSize = *(uint32_t *)&ReadData[DataOffset + 4];
    if (((StrSize << 1) + 8 + DataOffset) > ReadData.size())
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return RESULT_SUCCESS;
    }

    std::wstring InterfaceName;
    for (uint32_t i = 0; i < StrSize && ReadData[(i << 1) + 8 + DataOffset] != '\0'; i++)
    {
        InterfaceName += (wchar_t)ReadData[(i << 1) + 8 + DataOffset];
    }

    if (InterfaceName != L"android.gui.IGraphicBufferProducer")
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return RESULT_SUCCESS;
    }
    enum TransactionId
    {
        RequestBuffer = 1,
        SetBufferCount = 2,
        DequeueBuffer = 3,
        DetachBuffer = 4,
        DetachNextBuffer = 5,
        AttachBuffer = 6,
        QueueBuffer = 7,
        CancelBuffer = 8,
        Query = 9,
        Connect = 10,
        Disconnect = 11,

        AllocateBuffers = 13,
        SetPreallocatedBuffer = 14
    };

    uint32_t Id = *((uint32_t *)&RequestData[0]);
    TransactionId transaction = (TransactionId)*((uint32_t *)&RequestData[4]);

    CDisplay & Display = m_System.HleKernel().GetDisplay();
    CBufferQueue * BufferQueue = Display.FindBufferQueue(Id);
    if (BufferQueue == nullptr)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return RESULT_SUCCESS;
    }
    if (transaction == TransactionId::Connect)
    {
        uint32_t UndockedWidth = 1280;
        uint32_t UndockedHeight = 720;

        Request.WriteBuffer(IGBPConnectResponseParcel(UndockedWidth, UndockedHeight).Serialize());
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return RESULT_SUCCESS;
}