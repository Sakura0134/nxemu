#include "OpenGLImage.h"
#include "Textures/Texture.h"
#include "VideoNotification.h"
#include <Common/Maths.h>
#include <algorithm>

OpenGLImage::OpenGLImage() :
    m_Type(OpenGLImageType_e1D),
    m_Format(SurfacePixelFormat_Invalid),
    m_Size(1, 1, 1),
    m_Block(0, 0, 0),
    m_NumSamples(1),
    m_TileWidthSpacing(0),
    m_Renderer(nullptr),
    m_GpuAddr(0),
    m_CpuAddr(0),
    m_CpuAddrEnd(0),
    m_GLInternalFormat(GL_NONE),
    m_GLFormat(GL_NONE),
    m_GLType(GL_NONE),
    m_Ref(0)
{
    memset(m_MipLevelOffsets, 0, sizeof(m_MipLevelOffsets));
    SetOpenGLFormat();
}

OpenGLImage::OpenGLImage(const OpenGLImage & Image) :
    m_Type(Image.m_Type),
    m_Format(Image.m_Format),
    m_Size(Image.m_Size),
    m_Block(Image.m_Block),
    m_NumSamples(Image.m_NumSamples),
    m_TileWidthSpacing(Image.m_TileWidthSpacing),
    m_Renderer(nullptr),
    m_GpuAddr(0),
    m_CpuAddr(0),
    m_CpuAddrEnd(0),
    m_GLInternalFormat(Image.m_GLInternalFormat),
    m_GLFormat(Image.m_GLFormat),
    m_GLType(Image.m_GLType),
    m_Ref(0)
{
    memset(m_MipLevelOffsets, 0, sizeof(m_MipLevelOffsets));
    SetOpenGLFormat();
}

OpenGLImage::OpenGLImage(const CMaxwell3D::tyRenderTarget & RenderTarget, uint32_t Samples) :
    m_Type(OpenGLImageType_e1D),
    m_Format(SurfacePixelFormatFromRenderTargetFormat(RenderTarget.Format)),
    m_Size(1, 1, 1),
    m_Block(0, 0, 0),
    m_NumSamples(Samples),
    m_TileWidthSpacing(0),
    m_Renderer(nullptr),
    m_GpuAddr(0),
    m_CpuAddr(0),
    m_CpuAddrEnd(0),
    m_GLInternalFormat(GL_NONE),
    m_GLFormat(GL_NONE),
    m_GLType(GL_NONE),
    m_Ref(0)
{
    memset(m_MipLevelOffsets, 0, sizeof(m_MipLevelOffsets));
    if (RenderTarget.TileMode.IsPitchLinear)
    {
        if (RenderTarget.TileMode.Is3D != 0)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        m_Type = OpenGLImageType_Linear;
        m_Block.Width(RenderTarget.Width);
        m_Size = OpenGLExtent3D(m_Block.Width() / SurfacePixelFormatBytesPerBlock(m_Format), RenderTarget.Height, 1);
        return;
    }
    m_Size.Width(RenderTarget.Width);
    m_Size.Height(RenderTarget.Height);
    m_Block = OpenGLExtent3D(RenderTarget.TileMode.BlockWidth, RenderTarget.TileMode.BlockHeight, RenderTarget.TileMode.BlockDepth);
    if (RenderTarget.TileMode.Is3D)
    {
        m_Type = OpenGLImageType_e3D;
        m_Size.Depth(RenderTarget.Depth);
    }
    else
    {
        m_Type = OpenGLImageType_e2D;
        m_Resources.Layers(RenderTarget.Depth);
    }
    SetOpenGLFormat();
}

OpenGLImage::~OpenGLImage()
{
}

uint32_t OpenGLImage::LayerSize(void) const
{
    if (m_Type == OpenGLImageType_Linear)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    LevelInfo Info = MakeLevelInfo();
    uint32_t Size = 0;
    for (uint32_t i = 0, n = m_Resources.Levels(); i < n; i++)
    {
        Size += CalculateLevelSize(Info, i);
    }
    return Size;
}

void OpenGLImage::Create(uint64_t GpuAddr, uint64_t CpuAddr, OpenGLRenderer * Renderer)
{
    uint32_t GuestSize = GuestSizeBytes();
    m_Renderer = Renderer,
    m_GpuAddr = GpuAddr;
    m_CpuAddr = CpuAddr;
    m_CpuAddrEnd = CpuAddr + GuestSize;

    LevelInfo Levels = MakeLevelInfo();
    uint32_t Offset = 0;
    for (int32_t i = 0; i < m_Resources.Levels(); i++)
    {
        m_MipLevelOffsets[i] = Offset;
        Offset += CalculateLevelSize(Levels, i);
    }
    GLsizei NumLevels = std::min(m_Resources.Levels(), (int32_t)(32 - clz32(m_Size.Width())));
    switch (m_Type)
    {
    case OpenGLImageType_e2D:
        if (m_NumSamples > 1)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        m_Texture.Create(GL_TEXTURE_2D_ARRAY);
        m_Texture.TextureStorage3D(NumLevels, m_GLInternalFormat, m_Size.Width(), m_Size.Height(), m_Resources.Layers());
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        break;
    }
}

uint32_t OpenGLImage::GuestSizeBytes(void) const
{
    if (m_Type == OpenGLImageType_Buffer)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return 0;
    }
    if (m_Type == OpenGLImageType_Linear)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return 0;
    }
    if (m_Resources.Layers() > 1)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return 0;
    }
    return LayerSize();
}

OpenGLImage::LevelInfo OpenGLImage::MakeLevelInfo(void) const
{
    int SamplesX = 1, SamplesY = 1;
    switch (m_NumSamples)
    {
    case 1:
        SamplesX = 1;
        SamplesY = 1;
        break;
    case 2:
        SamplesX = 2;
        SamplesY = 1;
        break;
    case 4:
        SamplesX = 2;
        SamplesY = 2;
        break;
    case 8:
        SamplesX = 4;
        SamplesY = 2;
        break;
    case 16:
        SamplesX = 4;
        SamplesY = 4;
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }

    LevelInfo Info;
    Info.Size = OpenGLExtent3D(m_Size.Width() * SamplesX, m_Size.Height() * SamplesY, m_Size.Depth());
    Info.Block = m_Block;
    Info.TileSize = OpenGLExtent2D(SurfaceDefaultBlockWidth(m_Format), SurfaceDefaultBlockHeight(m_Format));
    Info.BytesPerBlockLog2 = BytesPerBlockLog2(SurfacePixelFormatBytesPerBlock(m_Format));
    Info.TileWidthSpacing = m_TileWidthSpacing;
    return Info;
}

void OpenGLImage::SetOpenGLFormat(void)
{
    if ((m_Format == SurfacePixelFormat_BC4_UNORM ||  m_Format == SurfacePixelFormat_BC5_UNORM) && m_Type == OpenGLImageType_e3D)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }

    switch (m_Format)
    {
    case SurfacePixelFormat_A8B8G8R8_UNORM:
        m_GLInternalFormat = GL_RGBA8;
        m_GLFormat = GL_RGBA;
        m_GLType = GL_UNSIGNED_INT_8_8_8_8_REV;
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

uint32_t OpenGLImage::AdjustTileSize(uint32_t Shift, uint32_t UnitFactor, uint32_t Dimension)
{
    if (Shift == 0)
    {
        return 0;
    }
    uint32_t X = UnitFactor << (Shift - 1);
    if (X >= Dimension)
    {
        while (--Shift)
        {
            X >>= 1;
            if (X < Dimension)
            {
                break;
            }
        }
    }
    return Shift;
}

OpenGLExtent3D OpenGLImage::LevelTiles(const LevelInfo & Info, uint32_t Level)
{
    OpenGLExtent3D Bocks = NumLevelBlocks(Info, Level), Shift = TileShift(Info, Level);
    OpenGLExtent2D Gobs = NumGobs(Info, Level);
    uint32_t Width = ((Gobs.Width() + (1 << Shift.Width()) - 1) >> Shift.Width());
    uint32_t Height = ((Gobs.Height() + (1 << Shift.Height()) - 1) >> Shift.Height());
    uint32_t Depth = ((Bocks.Depth() + (1 << Shift.Depth()) - 1) >> Shift.Depth());
    return OpenGLExtent3D(Width, Height, Depth);
}

OpenGLExtent3D OpenGLImage::NumLevelBlocks(const LevelInfo & Info, uint32_t Level)
{
    uint32_t Width = ((std::max<uint32_t>(Info.Size.Width() >> Level, 1) + Info.TileSize.Width() - 1) / Info.TileSize.Width()) << Info.BytesPerBlockLog2;
    uint32_t Height = (std::max<uint32_t>(Info.Size.Height() >> Level, 1) + Info.TileSize.Height() - 1) / Info.TileSize.Height();
    return OpenGLExtent3D(Width, Height, std::max<uint32_t>(Info.Size.Depth() >> Level, 1));
}

OpenGLExtent3D OpenGLImage::TileShift(const LevelInfo & Info, uint32_t Level)
{
    OpenGLExtent3D blocks = NumLevelBlocks(Info, Level);
    return OpenGLExtent3D(AdjustTileSize(Info.Block.Width(), Texture_GOB_SizeX, blocks.Width()), AdjustTileSize(Info.Block.Height(), Texture_GOB_SizeY, blocks.Height()), AdjustTileSize(Info.Block.Depth(), Texture_GOB_SizeZ, blocks.Depth()));
}

OpenGLExtent2D OpenGLImage::NumGobs(const LevelInfo & Info, uint32_t Level)
{
    OpenGLExtent3D Blocks = NumLevelBlocks(Info, Level);
    uint32_t GobWidth = ((Blocks.Width() + (1 << Texture_GOB_SizeXShift) - 1) >> Texture_GOB_SizeXShift);
    uint32_t GobHeight = ((Blocks.Height() + (1 << Texture_GOB_SizeYShift) - 1) >> Texture_GOB_SizeYShift);
    OpenGLExtent2D Gobs(GobWidth, GobHeight);
    OpenGLExtent2D Gob = GobSize(Info.BytesPerBlockLog2, Info.Block.Height(), Info.TileWidthSpacing);
    bool IsSmall = IsSmallerThanGobSize(Blocks, Gob, Info.Block.Depth());
    uint32_t Align = IsSmall ? 0 : Info.TileWidthSpacing;
    uint32_t Width = (Gobs.Width() + ((1ULL << Align) - 1)) >> Align << Align;
    return OpenGLExtent2D(Width, Gobs.Height());
}

uint32_t OpenGLImage::CalculateLevelSize(const LevelInfo & Info, uint32_t Level)
{
    OpenGLExtent3D Shift = TileShift(Info, Level);
    OpenGLExtent3D Tiles = LevelTiles(Info, Level);
    uint32_t NumTiles = Tiles.Width() * Tiles.Height() * Tiles.Depth();
    uint32_t ShiftValue = Texture_GOB_SizeShift + Shift.Width() + Shift.Height() + Shift.Depth();
    return NumTiles << ShiftValue;
}

bool OpenGLImage::IsSmallerThanGobSize(const OpenGLExtent3D & NumTiles, const OpenGLExtent2D & Gob, uint32_t BlockDepth)
{
    return NumTiles.Width() <= (1U << Gob.Width()) || NumTiles.Height() <= (1U << Gob.Height()) || NumTiles.Depth() < (1U << BlockDepth);
}

OpenGLExtent2D OpenGLImage::GobSize(uint32_t BytesPerBlockLog2, uint32_t Height, uint32_t TileWidthSpacing)
{
    return OpenGLExtent2D(Texture_GOB_SizeXShift - BytesPerBlockLog2 + TileWidthSpacing, Texture_GOB_SizeYShift + Height);
}

uint32_t OpenGLImage::BytesPerBlockLog2(uint32_t BytesPerBlock)
{
    return clz32(BytesPerBlock) ^ 0x1F;
}

