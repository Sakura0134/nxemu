#pragma once
#include "OpenGLResource.h"
#include "EmulatorWindow.h"
#include "Framebuffer.h"
#include "util/Rectangle.h"
#include <glad/glad.h>
#include <vector>

class OpenGLStateTracker;
class OpenGLTextureCache;
class OpenGLProgramManager;
class OpenGLDevice;

class OpenGLScreen
{
public:
    OpenGLScreen(ISwitchSystem & SwitchSystem, OpenGLStateTracker & StateTracker, OpenGLTextureCache & TextureCache, OpenGLProgramManager & ProgramManager, OpenGLDevice & Device);

    bool Init();
    void Draw(const EmulatorWindow & EmulatorWindow);
    void PrepareRendertarget(const CFramebuffer& Framebuffer);

private:
    OpenGLScreen();
    OpenGLScreen(const OpenGLScreen&);
    OpenGLScreen& operator=(const OpenGLScreen&);

    void ConfigureFramebufferTexture(const CFramebuffer & Framebuffer);

    ISwitchSystem & m_SwitchSystem;
    OpenGLStateTracker & m_StateTracker;
    OpenGLTextureCache & m_TextureCache;
    OpenGLProgramManager & m_ProgramManager;
    OpenGLDevice & m_Device;
    OpenGLProgram m_VertexProgram;
    OpenGLProgram m_FragmentProgram;
    OpenGLBuffer m_VertexBuffer;
    OpenGLSampler m_PresentSampler;
    GLuint64EXT m_VertexBufferAddress;
    std::vector<uint8_t> m_FramebufferData;
    TransformFlags m_FramebufferTransformFlags;
    CRectangle<int> m_FramebufferCropRect;
    OpenGLTexturePtr m_DisplayTexture;
    OpenGLTexturePtr m_Texture;
    OpenGLPipeline m_Pipeline;
    const CRectangle<float> m_DisplayTexCoords;
    bool m_DisplaySRGB;
    GPUPixelFormat m_PixelFormat;
    GLenum m_GLType;
    GLenum m_GLFormat;
    GLsizei m_Width;
    GLsizei m_Height;
};
