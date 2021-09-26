#include "OpenGLImage.h"
#include "Textures\Texture.h"
#include "Textures\Decoder.h"
#include "OpenGLRenderer.h"
#include "VideoNotification.h"
#include <Common\Maths.h>
#include <algorithm>

uint64_t OpenGLImage::m_CompatibleViewTable[SurfacePixelFormat_MaxPixelFormat][2];

OpenGLImage::OpenGLImage() :
    m_Type(OpenGLImageType_e1D),
    m_Format(SurfacePixelFormat_Invalid),
    m_Size(1, 1, 1),
    m_Block(0, 0, 0),
    m_NumSamples(1),
    m_LayerStride(0),
    m_TileWidthSpacing(0),
    m_Flags(ImageFlag_CpuModified),
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
    m_LayerStride(Image.m_LayerStride),
    m_TileWidthSpacing(Image.m_TileWidthSpacing),
    m_Flags(Image.m_Flags),
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

OpenGLImage::OpenGLImage(const TextureTICEntry & TICEntry ) :
    m_Type(OpenGLImageType_e1D),
    m_Format(SurfacePixelFormat_Invalid),
    m_Block(0, 0, 0),
    m_Size(1, 1, 1),
    m_NumSamples(1),
    m_LayerStride(0),
    m_TileWidthSpacing(0),
    m_Flags(ImageFlag_CpuModified),
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
    m_Format = TICEntry.PixelFormat();
    m_NumSamples = MsaaModeSamples(TICEntry.MsaaMode);
    m_Resources.Levels(TICEntry.MaxMipLevel + 1);
    if (TICEntry.IsPitchLinear())
    {
        m_Block.Width(TICEntry.Pitch());
    }
    else if (TICEntry.IsBlockLinear())
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    m_TileWidthSpacing = TICEntry.TileWidthSpacing;
    if (TICEntry.TextureType != TextureType_2D && TICEntry.TextureType != TextureType_2DNoMipmap)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }

    switch (TICEntry.TextureType)
    {
    case TextureType_2DNoMipmap:
        if (TICEntry.Depth() != 1)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        m_Type = TICEntry.IsPitchLinear() ? OpenGLImageType_Linear : OpenGLImageType_e2D;
        m_Size.Width(TICEntry.Width());
        m_Size.Height(TICEntry.Height());
        m_Resources.Layers(TICEntry.BaseLayer() + 1);
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        break;
    }
    if (m_Type != OpenGLImageType_Linear)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    SetOpenGLFormat();
}

OpenGLImage::OpenGLImage(const CMaxwell3D::tyRenderTarget & RenderTarget, uint32_t Samples) :
    m_Type(OpenGLImageType_e1D),
    m_Format(SurfacePixelFormatFromRenderTargetFormat(RenderTarget.Format)),
    m_Size(1, 1, 1),
    m_Block(0, 0, 0),
    m_NumSamples(Samples),
    m_LayerStride(0),
    m_TileWidthSpacing(0),
    m_Flags(ImageFlag_CpuModified),
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
    m_LayerStride = RenderTarget.LayerStride * 4;
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

OpenGLImageViewType OpenGLImage::ImageViewType(void) const
{
    switch (m_Type)
    {
    case OpenGLImageType_e2D: return m_Resources.Layers() > 1 ? OpenGLImageViewType_e2DArray : OpenGLImageViewType_e2D;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return OpenGLImageViewType_e1D;
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
    case OpenGLImageType_Linear:
        m_Texture.Create(GL_TEXTURE_2D_ARRAY);
        m_Texture.TextureStorage3D(NumLevels, m_GLInternalFormat, m_Size.Width(), m_Size.Height(), m_Resources.Layers());
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
        break;
    }
}

bool OpenGLImage::IsViewCompatible(SurfacePixelFormat Format, bool BrokenViews) const 
{
    if (BrokenViews)
    {
        return m_Format == Format;
    }
    return ((m_CompatibleViewTable[m_Format][Format / 64] >> (Format % 64)) & 1) != 0;
}

OpenGLBufferImageList OpenGLImage::UnswizzleImage(CVideoMemory & VideoMemory, uint64_t GpuAddr,  uint8_t * Output, size_t OutputSize) const
{
    uint32_t GuestSize = GuestSizeBytes();
    uint32_t BPBLog2 = BytesPerBlockLog2(SurfacePixelFormatBytesPerBlock(m_Format));

    if (m_Type == OpenGLImageType_Linear)
    {
        VideoMemory.ReadBuffer(GpuAddr, Output, GuestSize);

        if (((m_Block.Width() >> BPBLog2) << BPBLog2 != m_Block.Width()))
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        OpenGLBufferImageList Images(1);
        OpenGLBufferImage & Image = Images[0];
        Image.BufferOffset(0);
        Image.BufferSize(GuestSize);
        Image.BufferRowLength(m_Block.Width() >> BPBLog2);
        Image.BufferImageHeight(m_Size.Height());
        Image.ImageBaseLevel(0);
        Image.ImageBaseLayer(0);
        Image.ImageNumLayers(1);
        Image.ImageOffsetX(0);
        Image.ImageOffsetY(0);
        Image.ImageOffsetZ(0);
        Image.ImageExtent(m_Size);
        return Images;
    }
    std::unique_ptr<uint8_t> InputData(new uint8_t[GuestSize]);
    VideoMemory.ReadBuffer(GpuAddr, InputData.get(), GuestSize);

    LevelInfo Info = MakeLevelInfo();
    int32_t NumLayers = m_Resources.Layers();
    int32_t NumLevels = m_Resources.Levels();
    OpenGLExtent2D TileSize(SurfaceDefaultBlockWidth(m_Format), SurfaceDefaultBlockHeight(m_Format));
    OpenGLExtent2D Extent2D = GobSize(BPBLog2, m_Block.Height(), m_TileWidthSpacing);
    if (NumLevels >= MAX_MIP_LEVELS)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        OpenGLBufferImageList list;
        return list;
    }
    uint32_t LevelSizes[MAX_MIP_LEVELS];
    uint32_t LayerSize = 0;
    for (int32_t i = 0; i < NumLevels; i++)
    {
        LevelSizes[i] = CalculateLevelSize(Info, i);
        LayerSize += LevelSizes[i];
    }
    uint32_t LayerStride = AlignLayerSize(LayerSize, m_Size, Info.Block, TileSize.Height(), m_TileWidthSpacing);
    uint32_t GuestOffset = 0;
    uint64_t HostOffset = 0;

    OpenGLBufferImageList Images(NumLevels);
    for (int32_t i = 0; i < NumLevels; i++)
    {
        OpenGLBufferImage & Image = Images[i];
        OpenGLExtent3D LevelSize = AdjustMipSize(m_Size, i);
        uint32_t NumBlocksPerLayer = NumBlocks(LevelSize, TileSize);
        uint64_t HostBytesPerLayer = NumBlocksPerLayer << BPBLog2;

        uint32_t RowLengthMod = LevelSize.Width() % TileSize.Width();
        uint32_t RowLength = LevelSize.Width() - RowLengthMod;
        if (RowLengthMod != 0) { RowLength = RowLength + TileSize.Width(); }

        uint32_t ImageHeightMod = LevelSize.Height() % TileSize.Height();
        uint32_t ImageHeight = LevelSize.Height() - ImageHeightMod;
        if (ImageHeightMod != 0) { ImageHeight = ImageHeight + TileSize.Height(); }

        Image.BufferOffset(HostOffset);
        Image.BufferSize(HostBytesPerLayer * NumLayers);
        Image.BufferRowLength(RowLength);
        Image.BufferImageHeight(ImageHeight);
        Image.ImageBaseLevel(i);
        Image.ImageBaseLayer(0);
        Image.ImageNumLayers(m_Resources.Layers());
        Image.ImageExtent(LevelSize);
        Image.ImageOffsetX(0);
        Image.ImageOffsetY(0);
        Image.ImageOffsetZ(0);

        OpenGLExtent3D NumTiles = AdjustTileSize(LevelSize, TileSize);
        OpenGLExtent3D Block = AdjustMipBlockSize(NumTiles, Info.Block, i);
        uint32_t StrideAlignment = IsSmallerThanGobSize(NumTiles, Extent2D, Block.Depth()) ? Texture_GOB_SizeXShift - BPBLog2 : Extent2D.Width();
        uint32_t GuestLayerOffset = 0;

        for (int32_t Layer = 0; Layer < m_Resources.Layers(); Layer++)
        {
            uint32_t Offset = GuestOffset + GuestLayerOffset;
            UnswizzleTexture(Output + HostOffset, OutputSize - HostOffset, InputData.get() + Offset, GuestSize - Offset, 1U << BPBLog2, NumTiles.Width(), NumTiles.Height(), NumTiles.Depth(), Block.Height(), Block.Depth(), StrideAlignment);
            GuestLayerOffset += LayerStride;
            HostOffset += HostBytesPerLayer;
        }
        GuestOffset += LevelSizes[i];
    }
    return Images;
}

void OpenGLImage::UploadMemory(OpenGLStagingBuffer & Buffer, uint32_t BufferOffset, const OpenGLBufferImage * Images, size_t NoOfImages)
{
    Buffer.Buffer()->BindBuffer(GL_PIXEL_UNPACK_BUFFER);
    glFlushMappedBufferRange(GL_PIXEL_UNPACK_BUFFER, BufferOffset, UnswizzledSizeBytes());
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    uint32_t CurrentRowLength = 0xFFFFFFFF;
    uint32_t CurrentImageHeight = 0xFFFFFFFF;
    for (size_t i = 0; i < NoOfImages; i++)
    {
        const OpenGLBufferImage & Image = Images[i];
        if (CurrentRowLength != Image.BufferRowLength())
        {
            CurrentRowLength = Image.BufferRowLength();
            glPixelStorei(GL_UNPACK_ROW_LENGTH, CurrentRowLength);
        }
        if (CurrentImageHeight != Image.BufferImageHeight())
        {
            CurrentImageHeight = Image.BufferImageHeight();
            glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, CurrentImageHeight);
        }
        CopyBufferToImage(Image, BufferOffset);
    }
}

void OpenGLImage::CopyBufferToImage(const OpenGLBufferImage & Image, uint32_t BufferOffset) 
{
    bool IsCompressed = m_GLFormat == GL_NONE;
    const void * Offset = (const void*)(Image.BufferOffset() + BufferOffset);

    switch (m_Type)
    {
    case OpenGLImageType_e2D:
    case OpenGLImageType_Linear:
        if (IsCompressed) 
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        } 
        else
        {
            m_Texture.TextureSubImage3D(Image.ImageBaseLevel(), Image.ImageOffsetX(), Image.ImageOffsetY(), Image.ImageBaseLayer(), Image.ImageExtent().Width(), Image.ImageExtent().Height(), Image.ImageNumLayers(), m_GLFormat, m_GLType, Offset);
        }
        break;
    default:
        g_Notify->BreakPoint(__FILE__, __LINE__);
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
        uint32_t BlockHeight = SurfaceDefaultBlockHeight(m_Format);
        return m_Block.Width() * ((m_Size.Height() + BlockHeight - 1) / BlockHeight);
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

uint32_t OpenGLImage::NumBlocksPerLayer(const OpenGLExtent2D & TileSize) const
{
    uint32_t Blocks = 0;
    for (int32_t i = 0, n = m_Resources.Levels(); i < n; i++)
    {
        Blocks += NumBlocks(AdjustMipSize(m_Size, i), TileSize);
    }
    return Blocks;
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
    case SurfacePixelFormat_B5G6R5_UNORM:
        m_GLInternalFormat = GL_RGB565;
        m_GLFormat = GL_RGB;
        m_GLType = GL_UNSIGNED_SHORT_5_6_5_REV;
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

OpenGLExtent3D OpenGLImage::AdjustTileSize(const OpenGLExtent3D & Size, const OpenGLExtent2D & TileSize)
{
    uint32_t Width = (Size.Width() + TileSize.Width() - 1) / TileSize.Width();
    uint32_t Height = (Size.Height() + TileSize.Height() - 1) / TileSize.Height();
    return OpenGLExtent3D(Width, Height, Size.Depth());
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

uint32_t OpenGLImage::NumBlocks(const OpenGLExtent3D & Size, const OpenGLExtent2D & TileSize)
{
    OpenGLExtent3D Block = AdjustTileSize(Size, TileSize);
    return Block.Width() * Block.Height() * Block.Depth();
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

uint32_t OpenGLImage::AlignLayerSize(uint32_t SizeBytes, const OpenGLExtent3D & Size, OpenGLExtent3D Block, uint32_t TileSizeY, uint32_t TileWidthSpacing)
{
    if (TileWidthSpacing > 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return 0;
    }
    uint32_t AlignedHeightMod = Size.Height() % TileSizeY;
    uint32_t AlignedHeight = Size.Height() - AlignedHeightMod;
    if (AlignedHeightMod != 0) { AlignedHeight += TileSizeY; }

    while (Block.Height() != 0 && AlignedHeight <= (1U << (Block.Height() - 1)) * Texture_GOB_SizeY)
    {
        Block.Height(Block.Height() - 1);
    }
    while (Block.Depth() != 0 && Size.Depth() <= (1U << (Block.Depth() - 1)))
    {
        Block.Depth(Block.Depth() - 1);
    }
    uint32_t BlockShift = Texture_GOB_SizeShift + Block.Height() + Block.Depth();
    uint32_t NumBlocks = SizeBytes >> BlockShift;
    if (SizeBytes != NumBlocks << BlockShift)
    {
        return (NumBlocks + 1) << BlockShift;
    }
    return SizeBytes;
}

uint32_t OpenGLImage::AdjustMipSize(uint32_t size, uint32_t level)
{
    return std::max<uint32_t>(size >> level, 1);
}

OpenGLExtent3D OpenGLImage::AdjustMipBlockSize(OpenGLExtent3D NumTiles, OpenGLExtent3D BlockSize, uint32_t Level)
{
    uint32_t Width = BlockSize.Width();
    uint32_t Height = BlockSize.Height();
    uint32_t Depth = BlockSize.Depth();
    for (uint32_t i = 0, n = Level + 1; i < n; i++)
    {
        while (Width > 0 && NumTiles.Width() <= (1U << (Width - 1)) * Texture_GOB_SizeX) 
        {
            Width -= 1;
        }
    }

    for (uint32_t i = 0, n = Level + 1; i < n; i++)
    {
        while (Height > 0 && NumTiles.Height() <= (1U << (Height - 1)) * Texture_GOB_SizeY) 
        {
            Height -= 1;
        }
    }

    for (uint32_t i = 0, n = Level + 1; i < n; i++)
    {
        while (Depth > 0 && NumTiles.Depth() <= (1U << (Depth - 1)) * Texture_GOB_SizeZ) 
        {
            Depth -= 1;
        }
    }
    return OpenGLExtent3D(Width, Height, Depth);
}

OpenGLExtent3D OpenGLImage::AdjustMipSize(OpenGLExtent3D size, int32_t level)
{
    return OpenGLExtent3D(AdjustMipSize(size.Width(), level), AdjustMipSize(size.Height(), level), AdjustMipSize(size.Depth(), level));
}

OpenGLImageView * OpenGLImage::ImageView(OpenGLTexturePtr * NullTextures, uint32_t NumNullTextures, uint64_t GPUAddr, bool /*IsClear*/)
{
    OpenGLSubresourceBase Base;
    if (m_Type == OpenGLImageType_Linear)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return nullptr;
    }
    else if (!FindBase(GPUAddr, Base))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return nullptr;
    }
    int32_t Layers = m_Type == OpenGLImageType_e3D ? m_Size.Depth() : m_Resources.Layers();
    OpenGLSubresourceRange Range(Base,OpenGLSubresourceExtent(1, Layers));
    for (size_t i = 0, n = m_ImageViews.size(); i < n; i++)
    {
        OpenGLImageView * ImageView = m_ImageViews[i].Get();
        if (ImageView->Type() == ImageViewType() && ImageView->Format() == m_Format)
        {
            return ImageView;
        }
    }
    OpenGLImageViewPtr ImageViewPtr(new OpenGLImageView(ImageViewType(), m_Format, OpenGLSubresourceRange()));
    ImageViewPtr->Create(NullTextures, NumNullTextures, this);
    m_ImageViews.push_back(ImageViewPtr);
    return ImageViewPtr.Get();
}

OpenGLImageView * OpenGLImage::ImageView(OpenGLTexturePtr * NullTextures, uint32_t NumNullTextures, const TextureTICEntry & config, int32_t base_layer)
{
    if (m_ImageViews.size() > 0)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    m_ImageViews.emplace_back(new OpenGLImageView(config, base_layer));
    OpenGLImageViewPtr & ImageViewPtr = m_ImageViews[m_ImageViews.size() - 1];
    ImageViewPtr->Create(NullTextures, NumNullTextures, this);
    return ImageViewPtr.Get();
}

uint32_t OpenGLImage::MapSizeBytes(void) const
{
    if (IsFlagSet(ImageFlag_Converted))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    return UnswizzledSizeBytes();
}

uint32_t OpenGLImage::UnswizzledSizeBytes(void) const
{
    if (m_Type == OpenGLImageType_Buffer || m_NumSamples > 1)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return 0;
    }
    if (m_Type == OpenGLImageType_Linear)
    {
        uint32_t BlockHeight = SurfaceDefaultBlockHeight(m_Format);
        return m_Block.Width() * ((m_Size.Height() + BlockHeight - 1) / BlockHeight);
    }
    OpenGLExtent2D TileSize(SurfaceDefaultBlockWidth(m_Format), SurfaceDefaultBlockHeight(m_Format));
    return NumBlocksPerLayer(TileSize) * m_Resources.Layers() * SurfacePixelFormatBytesPerBlock(m_Format);
}

bool OpenGLImage::FindBase(uint64_t Addr, OpenGLSubresourceBase & Subresource) const
{
    if (Addr < m_GpuAddr)
    {
        return false;
    }

    uint32_t GuestSize = GuestSizeBytes();
    uint32_t Diff = (uint32_t)(Addr - m_GpuAddr);
    if (Diff > GuestSize)
    {
        return false;
    }
    if (m_Type == OpenGLImageType_e3D)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return false;
    }
    uint32_t MipOffset = m_LayerStride == 0 ? Diff : Diff % m_LayerStride;
    for (uint32_t i = 0, n = m_Resources.Levels(); i < n; i++)
    {
        if (m_MipLevelOffsets[i] != MipOffset)
        {
            continue;
        }
        Subresource.Level((int32_t)i);
        Subresource.Layer((int32_t)(m_LayerStride == 0 ? 0 : Diff / m_LayerStride));
        return true;
    }
    return false;
}

bool OpenGLImage::IsFlagSet(ImageFlags Flag) const
{
    return (m_Flags & Flag) != 0;
}

void OpenGLImage::UpdateFlags(uint32_t Add, uint32_t Remove)
{
    m_Flags &= ~Remove;
    m_Flags |= Add;
}

void OpenGLImage::Track(void)
{
    if (IsFlagSet(ImageFlag_Tracked))
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    UpdateFlags(ImageFlag_Tracked, 0);
    if (m_Renderer != nullptr)
    {
        m_Renderer->TrackRasterizerMemory(m_CpuAddr, GuestSizeBytes(), true);
    }
    else
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

void OpenGLImage::UploadImageContents(CVideoMemory & VideoMemory, OpenGLStagingBuffer & Buffer, uint32_t BufferOffset) 
{
    uint8_t * Map = Buffer.Map() + BufferOffset;
    uint32_t MapSize = Buffer.Size() - BufferOffset;

    if (IsFlagSet(ImageFlag_Converted) || m_Type == OpenGLImageType_Buffer) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    } 
    else 
    {
        OpenGLBufferImageList Images = UnswizzleImage(VideoMemory, m_GpuAddr, Map, MapSize);
        UploadMemory(Buffer, BufferOffset, Images.data(), Images.size());
    }
}

void OpenGLImage::EnableCompatibleView(SurfacePixelFormat FormatA, SurfacePixelFormat FormatB) 
{
    m_CompatibleViewTable[FormatA][FormatB / 64] |= 1ull << (FormatB % 64);
    m_CompatibleViewTable[FormatB][FormatA / 64] |= 1ull << (FormatA % 64);
}

void OpenGLImage::EnableCompatibleRange(SurfacePixelFormat * Formats, uint32_t NoOfFormats)
{
    for (uint32_t a = 0; a < NoOfFormats; a++)
    {
        for (uint32_t b = a; b < NoOfFormats; b++) 
        {
            EnableCompatibleView(Formats[a], Formats[b]);
        }
    }
}

void OpenGLImage::InitCompatibleViewTable(void)
{
    SurfacePixelFormat VIEW_CLASS_128_BITS[] = 
    {
        SurfacePixelFormat_R32G32B32A32_FLOAT,
        SurfacePixelFormat_R32G32B32A32_UINT,
        SurfacePixelFormat_R32G32B32A32_SINT,
    };

    SurfacePixelFormat VIEW_CLASS_96_BITS[] =
    {
        SurfacePixelFormat_R32G32B32_FLOAT,
    };

    SurfacePixelFormat VIEW_CLASS_64_BITS[] =
    {
        SurfacePixelFormat_R32G32_FLOAT,       SurfacePixelFormat_R32G32_UINT,
        SurfacePixelFormat_R32G32_SINT,        SurfacePixelFormat_R16G16B16A16_FLOAT,
        SurfacePixelFormat_R16G16B16A16_UNORM, SurfacePixelFormat_R16G16B16A16_SNORM,
        SurfacePixelFormat_R16G16B16A16_UINT,  SurfacePixelFormat_R16G16B16A16_SINT,
    };

    SurfacePixelFormat VIEW_CLASS_32_BITS[] =
    {
        SurfacePixelFormat_R16G16_FLOAT,     SurfacePixelFormat_B10G11R11_FLOAT,
        SurfacePixelFormat_R32_FLOAT,        SurfacePixelFormat_A2B10G10R10_UNORM,
        SurfacePixelFormat_R16G16_UINT,      SurfacePixelFormat_R32_UINT,
        SurfacePixelFormat_R16G16_SINT,      SurfacePixelFormat_R32_SINT,
        SurfacePixelFormat_A8B8G8R8_UNORM,   SurfacePixelFormat_R16G16_UNORM,
        SurfacePixelFormat_A8B8G8R8_SNORM,   SurfacePixelFormat_R16G16_SNORM,
        SurfacePixelFormat_A8B8G8R8_SRGB,    SurfacePixelFormat_E5B9G9R9_FLOAT,
        SurfacePixelFormat_B8G8R8A8_UNORM,   SurfacePixelFormat_B8G8R8A8_SRGB,
        SurfacePixelFormat_A8B8G8R8_UINT,    SurfacePixelFormat_A8B8G8R8_SINT,
        SurfacePixelFormat_A2B10G10R10_UINT,
    };

    SurfacePixelFormat VIEW_CLASS_16_BITS[] = 
    {
        SurfacePixelFormat_R16_FLOAT,  SurfacePixelFormat_R8G8_UINT,  SurfacePixelFormat_R16_UINT,
        SurfacePixelFormat_R16_SINT,   SurfacePixelFormat_R8G8_UNORM, SurfacePixelFormat_R16_UNORM,
        SurfacePixelFormat_R8G8_SNORM, SurfacePixelFormat_R16_SNORM,  SurfacePixelFormat_R8G8_SINT,
    };

    SurfacePixelFormat VIEW_CLASS_8_BITS[] = 
    {
        SurfacePixelFormat_R8_UINT,
        SurfacePixelFormat_R8_UNORM,
        SurfacePixelFormat_R8_SINT,
        SurfacePixelFormat_R8_SNORM,
    };

    SurfacePixelFormat VIEW_CLASS_RGTC1_RED[] = 
    {
        SurfacePixelFormat_BC4_UNORM,
        SurfacePixelFormat_BC4_SNORM,
    };

    SurfacePixelFormat VIEW_CLASS_RGTC2_RG[] = 
    {
        SurfacePixelFormat_BC5_UNORM,
        SurfacePixelFormat_BC5_SNORM,
    };

    SurfacePixelFormat VIEW_CLASS_BPTC_UNORM[] = 
    {
        SurfacePixelFormat_BC7_UNORM,
        SurfacePixelFormat_BC7_SRGB,
    };

    SurfacePixelFormat VIEW_CLASS_BPTC_FLOAT[] = 
    {
        SurfacePixelFormat_BC6H_SFLOAT,
        SurfacePixelFormat_BC6H_UFLOAT,
    };

    SurfacePixelFormat VIEW_CLASS_ASTC_4x4_RGBA[] = 
    {
        SurfacePixelFormat_ASTC_2D_4X4_UNORM,
        SurfacePixelFormat_ASTC_2D_4X4_SRGB,
    };

    SurfacePixelFormat VIEW_CLASS_ASTC_5x4_RGBA[] = 
    {
        SurfacePixelFormat_ASTC_2D_5X4_UNORM,
        SurfacePixelFormat_ASTC_2D_5X4_SRGB,
    };

    SurfacePixelFormat VIEW_CLASS_ASTC_5x5_RGBA[] = 
    {
        SurfacePixelFormat_ASTC_2D_5X5_UNORM,
        SurfacePixelFormat_ASTC_2D_5X5_SRGB,
    };

    SurfacePixelFormat VIEW_CLASS_ASTC_6x5_RGBA[] = 
    {
        SurfacePixelFormat_ASTC_2D_6X5_UNORM,
        SurfacePixelFormat_ASTC_2D_6X5_SRGB,
    };

    SurfacePixelFormat VIEW_CLASS_ASTC_6x6_RGBA[] = 
    {
        SurfacePixelFormat_ASTC_2D_6X6_UNORM,
        SurfacePixelFormat_ASTC_2D_6X6_SRGB,
    };

    SurfacePixelFormat VIEW_CLASS_ASTC_8x5_RGBA[] = 
    {
        SurfacePixelFormat_ASTC_2D_8X5_UNORM,
        SurfacePixelFormat_ASTC_2D_8X5_SRGB,
    };

    SurfacePixelFormat VIEW_CLASS_ASTC_8x8_RGBA[] =
    {
        SurfacePixelFormat_ASTC_2D_8X8_UNORM,
        SurfacePixelFormat_ASTC_2D_8X8_SRGB,
    };

    SurfacePixelFormat VIEW_CLASS_ASTC_10x8_RGBA[] = 
    {
        SurfacePixelFormat_ASTC_2D_10X8_UNORM,
        SurfacePixelFormat_ASTC_2D_10X8_SRGB,
    };

    SurfacePixelFormat VIEW_CLASS_ASTC_10x10_RGBA[] = 
    {
        SurfacePixelFormat_ASTC_2D_10X10_UNORM,
        SurfacePixelFormat_ASTC_2D_10X10_SRGB,
    };

    SurfacePixelFormat VIEW_CLASS_ASTC_12x12_RGBA[] =
    {
        SurfacePixelFormat_ASTC_2D_12X12_UNORM,
        SurfacePixelFormat_ASTC_2D_12X12_SRGB,
    };

    for (uint32_t i = 0; i < SurfacePixelFormat_MaxPixelFormat; i++) 
    {
        EnableCompatibleView((SurfacePixelFormat)i, (SurfacePixelFormat)i);
    }
    EnableCompatibleRange(VIEW_CLASS_128_BITS, sizeof(VIEW_CLASS_128_BITS) / sizeof(VIEW_CLASS_128_BITS[0]));
    EnableCompatibleRange(VIEW_CLASS_96_BITS, sizeof(VIEW_CLASS_96_BITS) / sizeof(VIEW_CLASS_96_BITS[0]));
    EnableCompatibleRange(VIEW_CLASS_64_BITS, sizeof(VIEW_CLASS_64_BITS) / sizeof(VIEW_CLASS_64_BITS[0]));
    EnableCompatibleRange(VIEW_CLASS_32_BITS, sizeof(VIEW_CLASS_32_BITS) / sizeof(VIEW_CLASS_32_BITS[0]));
    EnableCompatibleRange(VIEW_CLASS_16_BITS, sizeof(VIEW_CLASS_16_BITS) / sizeof(VIEW_CLASS_16_BITS[0]));
    EnableCompatibleRange(VIEW_CLASS_8_BITS, sizeof(VIEW_CLASS_8_BITS) / sizeof(VIEW_CLASS_8_BITS[0]));
    EnableCompatibleRange(VIEW_CLASS_RGTC1_RED, sizeof(VIEW_CLASS_RGTC1_RED) / sizeof(VIEW_CLASS_RGTC1_RED[0]));
    EnableCompatibleRange(VIEW_CLASS_RGTC2_RG, sizeof(VIEW_CLASS_RGTC2_RG) / sizeof(VIEW_CLASS_RGTC2_RG[0]));
    EnableCompatibleRange(VIEW_CLASS_BPTC_UNORM, sizeof(VIEW_CLASS_BPTC_UNORM) / sizeof(VIEW_CLASS_BPTC_UNORM[0]));
    EnableCompatibleRange(VIEW_CLASS_BPTC_FLOAT, sizeof(VIEW_CLASS_BPTC_FLOAT) / sizeof(VIEW_CLASS_BPTC_FLOAT[0]));
    EnableCompatibleRange(VIEW_CLASS_ASTC_4x4_RGBA, sizeof(VIEW_CLASS_ASTC_4x4_RGBA) / sizeof(VIEW_CLASS_ASTC_4x4_RGBA[0]));
    EnableCompatibleRange(VIEW_CLASS_ASTC_5x4_RGBA, sizeof(VIEW_CLASS_ASTC_5x4_RGBA) / sizeof(VIEW_CLASS_ASTC_5x4_RGBA[0]));
    EnableCompatibleRange(VIEW_CLASS_ASTC_5x5_RGBA, sizeof(VIEW_CLASS_ASTC_5x5_RGBA) / sizeof(VIEW_CLASS_ASTC_5x5_RGBA[0]));
    EnableCompatibleRange(VIEW_CLASS_ASTC_6x5_RGBA, sizeof(VIEW_CLASS_ASTC_6x5_RGBA) / sizeof(VIEW_CLASS_ASTC_6x5_RGBA[0]));
    EnableCompatibleRange(VIEW_CLASS_ASTC_6x6_RGBA, sizeof(VIEW_CLASS_ASTC_6x6_RGBA) / sizeof(VIEW_CLASS_ASTC_6x6_RGBA[0]));
    EnableCompatibleRange(VIEW_CLASS_ASTC_8x5_RGBA, sizeof(VIEW_CLASS_ASTC_8x5_RGBA) / sizeof(VIEW_CLASS_ASTC_8x5_RGBA[0]));
    EnableCompatibleRange(VIEW_CLASS_ASTC_8x8_RGBA, sizeof(VIEW_CLASS_ASTC_8x8_RGBA) / sizeof(VIEW_CLASS_ASTC_8x8_RGBA[0]));
    EnableCompatibleRange(VIEW_CLASS_ASTC_10x8_RGBA, sizeof(VIEW_CLASS_ASTC_10x8_RGBA) / sizeof(VIEW_CLASS_ASTC_10x8_RGBA[0]));
    EnableCompatibleRange(VIEW_CLASS_ASTC_10x10_RGBA, sizeof(VIEW_CLASS_ASTC_10x10_RGBA) / sizeof(VIEW_CLASS_ASTC_10x10_RGBA[0]));
    EnableCompatibleRange(VIEW_CLASS_ASTC_12x12_RGBA, sizeof(VIEW_CLASS_ASTC_12x12_RGBA) / sizeof(VIEW_CLASS_ASTC_12x12_RGBA[0]));
}
