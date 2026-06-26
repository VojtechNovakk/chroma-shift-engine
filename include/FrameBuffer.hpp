#pragma once

#include <cstddef>
#include <cstdint>
#include <algorithm>


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
    FrameBuffer(const FrameBuffer& other) : m_data(nullptr), m_w(other.m_w), m_h(other.m_h), m_channels(other.m_channels) {
        m_data = new uint8_t[m_w * m_h * m_channels];
        for (size_t i = 0; i < m_w * m_h * m_channels; ++i)
            m_data[i] = other.m_data[i];
    }
    FrameBuffer(FrameBuffer&& other) noexcept : m_data(nullptr), m_w(0), m_h(0), m_channels(0) {
        std::swap(m_w, other.m_w);
        std::swap(m_h, other.m_h);
        std::swap(m_channels, other.m_channels);
        std::swap(m_data, other.m_data);
    }
    FrameBuffer& operator=(FrameBuffer other) {
        std::swap(m_w, other.m_w);
        std::swap(m_h, other.m_h);
        std::swap(m_channels, other.m_channels);
        std::swap(m_data, other.m_data);

        return *this;
    }
    uint8_t* getData() { return m_data; };
    const uint8_t* getData() const { return m_data; };
    size_t getWidth() const { return m_w; }
    size_t getHeight() const { return m_h; }
    unsigned short int getChannels() const { return m_channels; }
};
