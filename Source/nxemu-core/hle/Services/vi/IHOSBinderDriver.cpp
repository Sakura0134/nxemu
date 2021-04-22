#include <nxemu-core\hle\Services\vi\IHOSBinderDriver.h>
#include <nxemu-core\hle\Services\vi\ViParcel.h>
#include <nxemu-core\hle\Display\Nvidia\NvMultiFence.h>
#include <nxemu-core\Machine\SwitchSystem.h>
#include <nxemu-core\SystemGlobals.h>

class IGBPConnectResponseParcel :
    public ViParcel
{
public:
    IGBPConnectResponseParcel(uint32_t Width, uint32_t Height) :
        m_Data({ 0 })
    {
        m_Data.Width = Width;
        m_Data.Height = Height;
    }

protected:
    void SerializeData()
    {
        Write(&m_Data, sizeof(m_Data));
    }

private:
    struct stData
    {
        uint32_t Width;
        uint32_t Height;
        uint32_t TransformHint;
        uint32_t PendingBuffers;
        uint32_t Status;
    };

    stData m_Data;
};

class IGBPQueryResponseParcel :
    public ViParcel
{
public:
    IGBPQueryResponseParcel(uint32_t Data) :
        m_Data(Data)
    {
    }

protected:
    void SerializeData()
    {
        Write(&m_Data, sizeof(m_Data));
    }

private:
    uint32_t m_Data;
};

class IGBPQueryRequestParcel :
    public ViParcel
{
public:
    IGBPQueryRequestParcel(const CIPCRequest::RequestBuffer & Buffer) :
        ViParcel(Buffer),
        m_Type(BufferQueueQueryType_Width)
    {
        Deserialize();
    }

    void DeserializeData()
    {
        std::wstring token = ReadInterfaceToken();
        Read(&m_Type, sizeof(m_Type));
    }

    BufferQueueQueryType Type(void) const { return m_Type; }

private:
    BufferQueueQueryType m_Type;
};

class IGBPSetPreallocatedBufferRequestParcel :
    public ViParcel
{
public:
    IGBPSetPreallocatedBufferRequestParcel(const CIPCRequest::RequestBuffer & Buffer) :
        ViParcel(Buffer),
        m_Data({0}),
        m_Buffer({0})
    {
        Deserialize();
    }

    void DeserializeData()
    {
        std::wstring Token = ReadInterfaceToken();
        Read(&m_Data, sizeof(m_Data));
        Read(&m_Buffer, sizeof(m_Buffer));
    }

    struct stData
    {
        uint32_t Slot;
        PADDING_WORDS(1);
        uint32_t GraphicBufferLength;
        PADDING_WORDS(1);
    };

    const stData & Data(void) const { return m_Data; }
    const IGBPBuffer & Buffer(void) const { return m_Buffer; }

private:
    stData m_Data;
    IGBPBuffer m_Buffer;
};

class IGBPSetPreallocatedBufferResponseParcel :
    public ViParcel
{
protected:
    void SerializeData()
    {
        uint32_t Value = 0;
        Write(&Value, sizeof(Value));
    }
};

class IGBPDequeueBufferRequestParcel :
    public ViParcel
{
public:
    IGBPDequeueBufferRequestParcel(const CIPCRequest::RequestBuffer & Buffer) :
        ViParcel(Buffer),
        m_Data({0})
    {
        Deserialize();
    }
    void DeserializeData()
    {
        std::wstring Token = ReadInterfaceToken();
        Read(&m_Data, sizeof(m_Data));
    }

    struct stData
    {
        uint32_t PixelFormat;
        uint32_t Width;
        uint32_t Height;
        uint32_t FrameTimestamps;
        uint32_t Usage;
    };

    const stData & Data(void) const { return m_Data; }

private:
    stData m_Data;
};

class IGBPDequeueBufferResponseParcel :
    public ViParcel
{
public:
    IGBPDequeueBufferResponseParcel(uint32_t Slot, const NvMultiFence & MultiFence) :
        m_Slot(Slot),
        m_MultiFence(MultiFence)
    {
    }

protected:
    void SerializeData()
    {
        uint32_t Value0 = 0, Value1 = 1;
        Write(&m_Slot, sizeof(m_Slot));
        Write(&Value1, sizeof(Value1));
        WriteObject(&m_MultiFence, sizeof(m_MultiFence));
        Write(&Value0, sizeof(Value0));
    }
    uint32_t m_Slot;
    NvMultiFence m_MultiFence;
};

class IGBPRequestBufferRequestParcel :
    public ViParcel
{
public:
    IGBPRequestBufferRequestParcel(const CIPCRequest::RequestBuffer & Buffer) :
        ViParcel(Buffer),
        m_Slot(0)
    {
        Deserialize();
    }
    void DeserializeData()
    {
        std::wstring token = ReadInterfaceToken();
        Read(&m_Slot, sizeof(m_Slot));
    }

    const uint32_t & slot(void) const { return m_Slot; }

private:
    uint32_t m_Slot;
};

class IGBPRequestBufferResponseParcel :
    public ViParcel
{
public:
    IGBPRequestBufferResponseParcel(const IGBPBuffer & Buffer) :
        m_Buffer(Buffer)
    {
    }

protected:
    void SerializeData()
    {
        uint32_t Value0 = 0, Value1 = 1;

        Write(&Value1, sizeof(Value1));
        WriteObject(&m_Buffer, sizeof(m_Buffer));
        Write(&Value0, sizeof(Value0));
    }

    IGBPBuffer m_Buffer;
};

class IGBPQueueBufferRequestParcel :
    public ViParcel
{
    struct stData
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
    IGBPQueueBufferRequestParcel(const CIPCRequest::RequestBuffer & Buffer) :
        ViParcel(Buffer),
        m_Data({ 0 })
    {
        Deserialize();
    }

    void DeserializeData()
    {
        std::wstring token = ReadInterfaceToken();
        Read(&m_Data, sizeof(m_Data));
    }

    inline const struct stData Data(void) const { return m_Data; }

private:
    stData m_Data;
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
    struct stData
    {
        uint32_t Width;
        uint32_t Height;
        uint32_t TransformHint;
        uint32_t PendingBuffers;
        uint32_t Status;
    };

    stData m_Data;
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
    else if (transaction == TransactionId::SetPreallocatedBuffer)
    {
        IGBPSetPreallocatedBufferRequestParcel RequestParcel(ReadData);
        BufferQueue->SetPreallocatedBuffer(RequestParcel.Data().Slot, RequestParcel.Buffer());
        Request.WriteBuffer(IGBPSetPreallocatedBufferResponseParcel().Serialize());
    }
    else if (transaction == TransactionId::DequeueBuffer)
    {
        IGBPDequeueBufferRequestParcel RequestParcel(ReadData);
        NvMultiFence MultiFence;
        uint32_t Slot;
        if (BufferQueue->DequeueBuffer(RequestParcel.Data().Width, RequestParcel.Data().Height, Slot, MultiFence))
        {
            Request.WriteBuffer(IGBPDequeueBufferResponseParcel(Slot, MultiFence).Serialize());
        }
        else
        {
            CSystemThread* CurrentThread = m_System.SystemThread()->GetSystemThreadPtr();
            if (CurrentThread == nullptr)
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
                return RESULT_SUCCESS;
            }

            KernelObjectList EventObjects;
            EventObjects.push_back(BufferQueue->WaitEvent());
            CurrentThread->WaitEvent(EventObjects, -1);
            if (BufferQueue->DequeueBuffer(RequestParcel.Data().Width, RequestParcel.Data().Height, Slot, MultiFence))
            {
                Request.WriteBuffer(IGBPDequeueBufferResponseParcel(Slot, MultiFence).Serialize());
            }
            else
            {
                g_Notify->BreakPoint(__FILE__, __LINE__);
            }
        }
    }
    else if (transaction == TransactionId::RequestBuffer)
    {
        IGBPRequestBufferRequestParcel RequestParcel(ReadData);
        IGBPRequestBufferResponseParcel Response(BufferQueue->RequestBuffer(RequestParcel.slot()));
        Request.WriteBuffer(Response.Serialize());
    }
    else if (transaction == TransactionId::QueueBuffer)
    {
        IGBPQueueBufferRequestParcel RequestParcel(ReadData);
        BufferQueue->QueueBuffer(RequestParcel.Data().Slot, RequestParcel.Data().Transform,
            RequestParcel.Data().Crop, RequestParcel.Data().SwapInterval,
            RequestParcel.Data().MultiFence);

        Request.WriteBuffer(IGBPQueueBufferResponseParcel(1280, 720).Serialize());
    }
    else if (transaction == TransactionId::Query)
    {
        IGBPQueryRequestParcel RequestParcel(ReadData);
        uint32_t Value = BufferQueue ? BufferQueue->Query((BufferQueueQueryType)RequestParcel.Type()) : 0;
        Request.WriteBuffer(IGBPQueryResponseParcel(Value).Serialize());
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return RESULT_SUCCESS;
}