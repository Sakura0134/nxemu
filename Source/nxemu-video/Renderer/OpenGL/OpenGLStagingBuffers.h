#pragma once
#include "OpenGLStagingBuffer.h"
#include <glad/glad.h>
#include <stdint.h>
#include <vector>

class OpenGLStagingBuffers 
{
public:
    OpenGLStagingBuffers(GLenum StorageFlags, GLenum MapFlags);
    ~OpenGLStagingBuffers();

    OpenGLStagingBuffer RequestMap(uint32_t RequestedSize, bool InsertFence);
    size_t RequestBuffer(uint32_t RequestedSize);
    bool FindBuffer(uint32_t RequestedSize, size_t & BufferIndex);

private:
    OpenGLStagingBuffers(void);
    OpenGLStagingBuffers(const OpenGLStagingBuffers&);
    OpenGLStagingBuffers& operator=(const OpenGLStagingBuffers&);

    std::vector<OpenGLStagingBuffer> m_Buffers;
    GLenum m_StorageFlags;
    GLenum m_MapFlags;
};
