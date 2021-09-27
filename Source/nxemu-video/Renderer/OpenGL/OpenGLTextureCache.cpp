#include "OpenGLTextureCache.h"
#include "OpenGLImageView.h"
#include "OpenGLStateTracker.h"
#include "OpenGLDevice.h"
#include "Engine/Maxwell3D.h"
#include "Textures/Texture.h"
#include "OpenGLTypes.h"
#include "Video.h"
#include "VideoNotification.h"

OpenGLTextureCache::OpenGLTextureCache(OpenGLRenderer & Renderer, CVideo & Video) :
    m_Renderer(Renderer),
    m_Maxwell3D(Video.Maxwell3D()),
    m_VideoMemory(Video.VideoMemory()),
    m_GraphicsImageTable(Video.VideoMemory()), 
    m_GraphicsSamplerTable(Video.VideoMemory()), 
    m_UploadBuffers(GL_MAP_WRITE_BIT, GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT),
    m_RenderWidth(0),
    m_RenderHeight(0),
    m_HasBrokenTextureViewFormats(false)
{
    memset(m_DrawBuffers, 0, sizeof(m_DrawBuffers));
}

bool OpenGLTextureCache::Init(const OpenGLDevice & Device)
{
    m_HasBrokenTextureViewFormats = Device.HasBrokenTextureViewFormats();

    GLint NULL_SWIZZLE[] = { GL_ZERO, GL_ZERO, GL_ZERO, GL_ZERO };
    OpenGLTexturePtr & NullImage1DArray = m_NullImages[OpenGLImageViewType_e1DArray];
    NullImage1DArray.Reset(new OpenGLTexture);
    NullImage1DArray->Create(GL_TEXTURE_1D_ARRAY);
    NullImage1DArray->TextureStorage2D(1, GL_R8, 1, 1);
    NullImage1DArray->TextureParameteriv(GL_TEXTURE_SWIZZLE_RGBA, NULL_SWIZZLE);

    OpenGLTexturePtr & NullImageCubeArray = m_NullImages[OpenGLImageViewType_CubeArray];
    NullImageCubeArray.Reset(new OpenGLTexture);
    NullImageCubeArray->Create(GL_TEXTURE_CUBE_MAP_ARRAY);
    NullImageCubeArray->TextureStorage3D(1, GL_R8, 1, 1, 6);
    NullImageCubeArray->TextureParameteriv(GL_TEXTURE_SWIZZLE_RGBA, NULL_SWIZZLE);

    OpenGLTexturePtr & NullImage3D = m_NullImages[OpenGLImageViewType_e3D];    
    NullImage3D.Reset(new OpenGLTexture);
    NullImage3D->Create(GL_TEXTURE_3D);
    NullImage3D->TextureStorage3D(1, GL_R8, 1, 1, 1);
    NullImage3D->TextureParameteriv(GL_TEXTURE_SWIZZLE_RGBA, NULL_SWIZZLE);

    OpenGLTexturePtr & NullImageRect = m_NullImages[OpenGLImageViewType_Rect];
    NullImageRect.Reset(new OpenGLTexture);
    NullImageRect->Create(GL_TEXTURE_RECTANGLE);
    NullImageRect->TextureStorage2D(1, GL_R8, 1, 1);
    NullImageRect->TextureParameteriv(GL_TEXTURE_SWIZZLE_RGBA, NULL_SWIZZLE);

    OpenGLTexturePtr & NullImageView1D = m_NullImages[OpenGLImageViewType_e1D];
    NullImageView1D.Reset(new OpenGLTexture);
    NullImageView1D->Generate();
    NullImageView1D->TextureView(GL_TEXTURE_1D, NullImage1DArray, GL_R8, 0, 1, 0, 1);
    NullImageView1D->TextureParameteriv(GL_TEXTURE_SWIZZLE_RGBA, NULL_SWIZZLE);

    OpenGLTexturePtr & NullImageView2D = m_NullImages[OpenGLImageViewType_e2D];
    NullImageView2D.Reset(new OpenGLTexture);
    NullImageView2D->Generate();
    NullImageView2D->TextureView(GL_TEXTURE_2D, NullImageCubeArray, GL_R8, 0, 1, 0, 1);
    NullImageView2D->TextureParameteriv(GL_TEXTURE_SWIZZLE_RGBA, NULL_SWIZZLE);

    OpenGLTexturePtr & NullImageView2DArray = m_NullImages[OpenGLImageViewType_e2DArray];
    NullImageView2DArray.Reset(new OpenGLTexture);
    NullImageView2DArray->Generate();
    NullImageView2DArray->TextureView(GL_TEXTURE_2D_ARRAY, NullImageCubeArray, GL_R8, 0, 1, 0, 1);
    NullImageView2DArray->TextureParameteriv(GL_TEXTURE_SWIZZLE_RGBA, NULL_SWIZZLE);

    OpenGLTexturePtr & NullImageViewCube = m_NullImages[OpenGLImageViewType_Cube];
    NullImageViewCube.Reset(new OpenGLTexture);
    NullImageViewCube->Generate();
    NullImageViewCube->TextureView(GL_TEXTURE_CUBE_MAP, NullImageCubeArray, GL_R8, 0, 1, 0, 6);
    NullImageViewCube->TextureParameteriv(GL_TEXTURE_SWIZZLE_RGBA, NULL_SWIZZLE);
    return true;
}

void OpenGLTextureCache::FillGraphicsImageViews(const uint32_t * Indices, uint32_t IndiceSize, OpenGLImageViewPtr * ImageViews, uint32_t ImageViewSize)
{
    if (IndiceSize > ImageViewSize)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }

    for (uint32_t i = 0, n = IndiceSize; i < n; i++)
    {
        if (i > m_GraphicsImageTable.Limit())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            break;
        }
        TextureTICEntry Descriptor;
        bool IsNew = m_GraphicsImageTable.Read(Indices[i], Descriptor);
        OpenGLImageViewPtr & ImageView = ImageViews[i];
        if (IsNew)
        {
            uint64_t CpuAddr;
            if (Descriptor.Address() != 0 && Descriptor.Address() <= (1ull << 48) && m_VideoMemory.GpuToCpuAddress(Descriptor.Address(), CpuAddr))
            {
                TextureEntryOpenGLImages::const_iterator itr = m_TextureImageViews.find(Descriptor);
                if (itr == m_TextureImageViews.end()) 
                {
                    OpenGLImage ImageInfo(Descriptor);
                    uint64_t ImageAddr = Descriptor.Address() - (Descriptor.BaseLayer() * ImageInfo.LayerStride());
                    OpenGLImage * Image = GetImage(ImageInfo, ImageAddr);
                    if (Image != nullptr)
                    {
                        OpenGLSubresourceBase Base;
                        if (!Image->FindBase(Descriptor.Address(), Base) || Base.Level() != 0)
                        {
                            g_Notify->BreakPoint(__FILE__, __LINE__);
                        }

                        ImageView.Reset(Image->ImageView(m_NullImages, sizeof(m_NullImages) / sizeof(m_NullImages[0]), Descriptor, Base.Layer()));
                        if (ImageView.Get() != nullptr)
                        {
                            m_TextureImageViews.emplace(Descriptor, ImageView.Get());
                        }
                        else
                        {
                            g_Notify->BreakPoint(__FILE__, __LINE__);
                            ImageView.Reset(nullptr);
                        } 
                    }
                    else
                    {
                        g_Notify->BreakPoint(__FILE__, __LINE__);
                        ImageView.Reset(nullptr);
                    }
                } 
                else 
                {
                    ImageView.Reset(itr->second.Get());
                }
            }
            else
            {
                ImageView.Reset(nullptr);
            }
        }
        if (ImageView.Get() != nullptr)
        {
            PrepareImage(ImageView->Image(), false, false);
        }
    }
}

OpenGLSamplerPtr OpenGLTextureCache::GetGraphicsSampler(uint32_t Index)
{
    if (Index > m_GraphicsSamplerTable.Limit())
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return nullptr;
    }
    TextureTSCEntry Descriptor;
    bool IsNew = m_GraphicsSamplerTable.Read(Index, Descriptor);
    OpenGLSamplerPtr & Ptr = m_GraphicsSamplers[Index];
    if (IsNew) 
    {
        bool NullConfig = true;
        for (uint32_t i = 0, n = sizeof(Descriptor.Value) / sizeof(Descriptor.Value[0]); i < n; i++)
        {
            if (Descriptor.Value[i] == 0)
            {
                continue;
            }
            NullConfig = false;
            break;
        }
        if (!NullConfig)
        {
            std::pair<TextureEntryOpenGLSamplers::iterator, bool> Res = m_TextureSamplers.try_emplace(Descriptor);
            if (Res.second)
            {
                m_Samplers.emplace_back(new OpenGLSampler(Descriptor));
                Res.first->second = m_Samplers[m_Samplers.size() - 1];
            }
            Ptr = Res.first->second;
        }
        else 
        {
            Ptr = nullptr;
        }
    }
    return Ptr;
}

void OpenGLTextureCache::SynchronizeGraphicsDescriptors()
{
    bool Linked = m_Maxwell3D.Regs().SamplerIndex == CMaxwell3D::SamplerIndex_ViaHeaderIndex;
    uint32_t TicLimit = m_Maxwell3D.Regs().Tic.Limit;
    uint32_t TscLimit = Linked ? TicLimit : m_Maxwell3D.Regs().Tsc.Limit;
    if (m_GraphicsSamplerTable.Synchornize(m_Maxwell3D.Regs().Tsc.Address(), TscLimit))
    {
        m_GraphicsSamplers.resize(TscLimit + 1);
    }
    if (m_GraphicsImageTable.Synchornize(m_Maxwell3D.Regs().Tic.Address(), TicLimit))
    {
        m_GraphicsImageViews.resize(TicLimit + 1);
    }
}

void OpenGLTextureCache::UpdateRenderTargets(bool IsClear)
{
    CStateTracker & StateTracker = m_Maxwell3D.StateTracker();
    if (!StateTracker.Flag(OpenGLDirtyFlag_RenderTargets))
    {
        return;
    }
    StateTracker.FlagClear(OpenGLDirtyFlag_RenderTargets);

    bool Force = StateTracker.Flag(OpenGLDirtyFlag_RenderTargetControl);
    StateTracker.FlagClear(OpenGLDirtyFlag_RenderTargetControl);

    for (uint8_t i = 0, n = sizeof(m_ColorBuffer) / sizeof(m_ColorBuffer[0]); i < n; i++)
    {
        OpenGLImageViewPtr & ColorBuffer = m_ColorBuffer[i];
        if (StateTracker.Flag(OpenGLDirtyFlag_ColorBuffer0 + i) || Force)
        {
            StateTracker.FlagClear(OpenGLDirtyFlag_ColorBuffer0 + i);
            OpenGLImageViewPtr NewColorBuffer = FindColorBuffer(i, IsClear);
            if (ColorBuffer != NewColorBuffer)
            {
                if (ColorBuffer.Get() != nullptr && ColorBuffer->IsFlagSet(ImageViewFlag_PreemtiveDownload))
                {
                    g_Notify->BreakPoint(__FILE__, __LINE__);
                }
                ColorBuffer = NewColorBuffer;
            }
        }
        if (ColorBuffer.Get() != nullptr)
        {
            PrepareImage(ColorBuffer->Image(), true, IsClear && IsFullClear(ColorBuffer));
        }
    }
    if (StateTracker.Flag(OpenGLDirtyFlag_ZetaBuffer) || Force)
    {
        StateTracker.FlagClear(OpenGLDirtyFlag_ZetaBuffer);
        m_DepthBuffer = FindDepthBuffer(IsClear);
    }
    if (m_DepthBuffer.Get() != nullptr)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }

    for (uint32_t index = 0; index < NumRenderTargets; ++index)
    {
        m_DrawBuffers[index] = (uint8_t)(m_Maxwell3D.Regs().RTControl.Map(index));
    }
    m_RenderWidth = m_Maxwell3D.Regs().RenderArea.Width;
    m_RenderHeight = m_Maxwell3D.Regs().RenderArea.Height;
}

OpenGLFramebufferPtr OpenGLTextureCache::GetFramebuffer()
{
    uint64_t FramebufferID = (uint64_t)(m_DepthBuffer.Get());
    for (uint32_t i = 0; i < NumRenderTargets; i++)
    {
        FramebufferID ^= (uint64_t)(m_ColorBuffer[i].Get());
        FramebufferID ^= m_DrawBuffers[i];
    }
    FramebufferID ^= m_RenderWidth;
    FramebufferID ^= m_RenderHeight;

    OpenGLFramebuffers::iterator itr = m_Framebuffers.find(FramebufferID);
    if (itr != m_Framebuffers.end())
    {
        return itr->second.Get();
    }

    OpenGLFramebufferPtr Framebuffer(new OpenGLFramebuffer(m_ColorBuffer, NumRenderTargets, m_DepthBuffer.Get(), m_DrawBuffers, NumRenderTargets, m_RenderWidth, m_RenderHeight));
    m_Framebuffers.emplace(FramebufferID, Framebuffer);
    return Framebuffer;
}

void OpenGLTextureCache::RefreshContents(OpenGLImage & Image)
{
    if (!Image.IsFlagSet(ImageFlag_CpuModified))
    {
        return;
    }
    Image.UpdateFlags(0, ImageFlag_CpuModified);
    Image.Track();
    if (Image.NumSamples() > 1)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    OpenGLStagingBuffer Buffer = m_UploadBuffers.RequestMap(Image.MapSizeBytes(), true);
    Image.UploadImageContents(m_VideoMemory, Buffer, 0);
    glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

OpenGLImage * OpenGLTextureCache::GetImage(const OpenGLImage & Info, uint64_t GpuAddr, uint32_t Options)
{
    uint64_t CpuAddr;
    if (!m_VideoMemory.GpuToCpuAddress(GpuAddr, CpuAddr) || CpuAddr == 0)
    {
        g_Notify->BreakPoint(__FILE__,__LINE__);
        return nullptr;
    }
    uint32_t Size = Info.GuestSizeBytes();
    for (uint64_t Page = CpuAddr >> PAGE_BITS,  PageEnd = (CpuAddr + Size - 1) >> PAGE_BITS; Page <= PageEnd; Page++ )
    {
        PageTables::iterator itr = m_PageTable.find(Page);
        if (itr == m_PageTable.end())
        {
            continue;
        }

        OpenGLImagePtrList & Images = itr->second;
        for (size_t i = 0, n = Images.size(); i < n; i++)
        {
            OpenGLImage * Image = Images[i].Get();
            if (!Image->Overlaps(CpuAddr, Size))
            {
                continue;
            }
            if (Info.Type() == OpenGLImageType_Linear || Image->Type() == OpenGLImageType_Linear)
            {
                g_Notify->BreakPoint(__FILE__,__LINE__);
            }
            else if (Image->IsSubresource(Info, GpuAddr, Options, m_HasBrokenTextureViewFormats))
            {
                return Image;
            }
        }
    }

    OpenGLImagePtr Image(new OpenGLImage(Info));
    Image->Create(GpuAddr, CpuAddr, &m_Renderer);
    m_Images.emplace(std::pair<uint64_t, OpenGLImagePtr>(Image->GpuAddr(), Image));
    RefreshContents(Image);
    RegisterImage(Image);
    return Image.Get();
}

OpenGLImageViewPtr OpenGLTextureCache::FindColorBuffer(size_t Index, bool IsClear)
{
    const CMaxwell3D::Registers & Regs = m_Maxwell3D.Regs();
    if (Index >= Regs.RTControl.Count)
    {
        return nullptr;
    }
    const CMaxwell3D::tyRenderTarget & RenderTarget = Regs.RenderTarget[Index];
    if (RenderTarget.Address() == 0 || RenderTarget.Format == RenderTargetFormat_NONE)
    {
        return nullptr;
    }
    uint64_t GPUAddr = RenderTarget.Address();
    OpenGLImage * Image = GetImage(OpenGLImage(Regs.RenderTarget[Index], MsaaModeSamples(Regs.MultisampleMode)), GPUAddr, IsClear ? OpenGLOptions_Samples : 0);
    if (Image == nullptr)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return nullptr;
    }
    return Image->ImageView(m_NullImages, sizeof(m_NullImages) / sizeof(m_NullImages[0]), GPUAddr, IsClear);
}

OpenGLImageViewPtr OpenGLTextureCache::FindDepthBuffer(bool /*IsClear*/)
{
    const CMaxwell3D::Registers & Regs = m_Maxwell3D.Regs();
    if (!Regs.ZetaEnable)
    {
        return nullptr;
    }
    const uint64_t GpuAddr = Regs.Zeta.Address();
    if (GpuAddr == 0)
    {
        return nullptr;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return nullptr;
}

void OpenGLTextureCache::RegisterImage(OpenGLImagePtr & Image)
{
    if (Image->IsFlagSet(ImageFlag_Registered))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    Image->UpdateFlags(0, ImageFlag_Registered);

    for (uint64_t Page = Image->CpuAddr() >> PAGE_BITS, PageEnd = (Image->CpuAddr() + Image->GuestSizeBytes() - 1) >> PAGE_BITS; Page <= PageEnd; Page++)
    {
        m_PageTable[Page].push_back(Image);
    }
}

void OpenGLTextureCache::PrepareImage(OpenGLImagePtr & Image, bool IsModification, bool Invalidate)
{
    if (Invalidate)
    {
        Image->UpdateFlags(0, ImageFlag_CpuModified | ImageFlag_GpuModified);
        if (!Image->IsFlagSet(ImageFlag_Tracked))
        {
            Image->Track();
        }
    }
    else
    {
        RefreshContents(Image);
    }
    if (IsModification)
    {
        Image->UpdateFlags(ImageFlag_GpuModified, 0);
    }
}

bool OpenGLTextureCache::IsFullClear(OpenGLImageViewPtr & ImageView)
{
    if (ImageView.Get() == nullptr)
    {
        return true;
    }
    OpenGLImagePtr Image = ImageView->Image();
    const OpenGLExtent3D & Size = ImageView->Size();
    const CMaxwell3D::Registers & Regs = m_Maxwell3D.Regs();
    const CMaxwell3D::tyScissorTest & Scissor = Regs.ScissorTest[0];
    if (Image->Resources().Levels() > 1 || Image->Resources().Layers() > 1)
    {
        return false;
    }
    if (Regs.ClearFlags.Scissor == 0)
    {
        return true;
    }
    return Scissor.MinX == 0 && Scissor.MinY == 0 && Scissor.MaxX >= Size.Width() && Scissor.MaxY >= Size.Height();
}

void OpenGLTextureCache::WriteMemory(uint64_t /*CpuAddr*/, uint64_t /*Size*/)
{
}