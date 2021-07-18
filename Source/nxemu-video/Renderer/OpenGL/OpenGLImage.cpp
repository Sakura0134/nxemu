#include "OpenGLImage.h"
#include "Textures/Texture.h"
#include "Textures/Decoder.h"
#include "OpenGLRenderer.h"
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

OpenGLImage::OpenGLImage(const CMaxwell3D::tyRenderTarget & RenderTarget, uint32_t Samples) :
    m_Type(OpenGLImageType_e1D),
    m_Format(SurfacePixelFormatFromRenderTargetFormat(RenderTarget.Format)),
    m_Size(1, 1, 1),
    m_Block(0, 0, 0),
    m_NumSamples(Samples),
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

OpenGLBufferImageList OpenGLImage::UnswizzleImage(CVideoMemory & VideoMemory, uint64_t GpuAddr,  uint8_t * Output, size_t OutputSize) const
{
    uint32_t GuestSize = GuestSizeBytes();
    uint32_t BPBLog2 = BytesPerBlockLog2(SurfacePixelFormatBytesPerBlock(m_Format));

    if (m_Type == OpenGLImageType_Linear)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        OpenGLBufferImageList Images;
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
    if (m_Type == OpenGLImageType_Buffer || m_Type == OpenGLImageType_Linear || m_NumSamples > 1)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return 0;
    }
    OpenGLExtent2D TileSize(SurfaceDefaultBlockWidth(m_Format), SurfaceDefaultBlockHeight(m_Format));
    return NumBlocksPerLayer(TileSize) * m_Resources.Layers() * SurfacePixelFormatBytesPerBlock(m_Format);
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

