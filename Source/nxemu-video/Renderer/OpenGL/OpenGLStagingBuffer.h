#pragma once
#include "OpenGLResource.h"
#include <stdint.h>

class OpenGLStagingBuffer 
{
public:
    OpenGLStagingBuffer(OpenGLBufferPtr & Buffer, uint8_t * Map, uint32_t Size, OpenGLSyncPtr & Sync);
    OpenGLStagingBuffer(uint32_t Size, GLenum StorageFlags, GLenum MapFlags);
    OpenGLStagingBuffer(const OpenGLStagingBuffer &);
    ~OpenGLStagingBuffer();

    OpenGLSyncPtr & Sync() { return m_Sync; }
    OpenGLBufferPtr & Buffer() { return m_Buffer; }
    uint8_t * Map() const { return m_Map; }
    uint32_t Size() const { return m_Size; }

private:
    OpenGLStagingBuffer(void);
    OpenGLStagingBuffer & operator=(const OpenGLStagingBuffer &);

    OpenGLBufferPtr m_Buffer;
    OpenGLSyncPtr m_Sync;
    uint8_t * m_Map;
    uint32_t m_Size;
};
