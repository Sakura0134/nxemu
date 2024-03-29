#pragma once
#include <nxemu-core\hle\Display\Layer.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <memory>

class CBufferQueue;

class CDisplayDevice
{
public:
    CDisplayDevice(uint32_t id, const char * Name);
    ~CDisplayDevice();

    bool CreateLayer(uint32_t id, std::shared_ptr<CBufferQueue> buffer_queue);
    CLayer * FindLayer(uint32_t id);

    inline uint32_t Id(void) const { return m_Id; }
    inline const char * Name(void) const { return m_Name.c_str(); }
    inline size_t Layers(void) const { return m_layers.size(); }
    inline CLayer & Layer(size_t index) const { return *m_layers[index]; }

private:
    std::vector<std::shared_ptr<CLayer>> m_layers;
    uint32_t m_Id;
    std::string m_Name;
};
