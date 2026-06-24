#pragma once

#include <cstddef>
#include <cstdint>


class FrameBuffer {
private:
    uint8_t* m_data;
    size_t m_w;
    size_t m_h;
    unsigned short int m_channels;
public:
    FrameBuffer(uint8_t* data, size_t w, size_t h, unsigned short int channels) :
        m_data(data), m_w(w), m_h(h), m_channels(channels) {};
    ~FrameBuffer() { delete[] m_data; }
    uint8_t* getData() { return m_data; };
    uint8_t* getData() const { return m_data; };
    size_t getWidth() const { return m_w; }
    size_t getHeight() const { return m_h; }
    unsigned short int getChannels() const { return m_channels; }
};
