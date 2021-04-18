#include <nxemu-core\hle\Services\vi\IHOSBinderDriver.h>
#include <nxemu-core\hle\Services\vi\ViParcel.h>
#include <nxemu-core\hle\Display\Nvidia\NvMultiFence.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\SystemGlobals.h>

class IGBPConnectResponseParcel :
    public ViParcel
{
public:
    IGBPConnectResponseParcel(uint32_t Width, uint32_t Height)
    {
        memset(&m_Data, 0, sizeof(m_Data));
        m_Data.Width = Width;
        m_Data.Height = Height;
    }

protected:
    void SerializeData()
    {
        Write(&m_Data, sizeof(m_Data));
    }

private:
    struct Data
    {
        uint32_t Width;
        uint32_t Height;
        uint32_t TransformHint;
        uint32_t NumPendingBuffers;
        uint32_t Status;
    };

    Data m_Data;
};

class IGBPQueueBufferRequestParcel :
    public ViParcel
{
    struct Data
    {
        uint32_t Slot;
        PADDING_WORDS(3);
        uint32_t Timestamp;
        int32_t AutoTimestamp;
        CBufferQueue::Rectangle Crop;
        int32_t ScalingMode;
        BufferTransformFlags Transform;
        uint32_t StickyTransform;
        PADDING_WORDS(1);
        uint32_t SwapInterval;
        NvMultiFence MultiFence;
    };

public:
    IGBPQueueBufferRequestParcel(const std::vector<uint8_t> & Buffer) :
        ViParcel(Buffer)
    {
        Deserialize();
    }

    void DeserializeData()
    {
        std::wstring token = ReadInterfaceToken();
        Read(&m_Data, sizeof(m_Data));
    }

    inline const struct Data data(void) const { return m_Data; }

private:
    Data m_Data;
};

class IGBPQueueBufferResponseParcel :
    public ViParcel
{
public:
    IGBPQueueBufferResponseParcel(uint32_t Width, uint32_t Height)
    {
        memset(&m_Data, 0, sizeof(m_Data));
        m_Data.Width = Width;
        m_Data.Height = Height;
    }

protected:
    void SerializeData()
    {
        Write(&m_Data, sizeof(m_Data));
    }

private:
    struct Data
    {
        uint32_t Width;
        uint32_t Height;
        uint32_t TransformHint;
        uint32_t PendingBuffers;
        uint32_t Status;
    };

    Data m_Data;
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
    else if (transaction == TransactionId::QueueBuffer)
    {
        IGBPQueueBufferRequestParcel RequestParcel(ReadData);
        BufferQueue->QueueBuffer(RequestParcel.data().Slot, RequestParcel.data().Transform,
            RequestParcel.data().Crop, RequestParcel.data().SwapInterval,
            RequestParcel.data().MultiFence);

        Request.WriteBuffer(IGBPQueueBufferResponseParcel(1280, 720).Serialize());
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return RESULT_SUCCESS;
}