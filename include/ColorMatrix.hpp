#pragma once

#include <algorithm>

class ColorMatrix {
private:
    float m_data[20];
public:
    ColorMatrix() : m_data{
        1, 0, 0, 0, 0,
        0, 1, 0, 0, 0,
        0, 0, 1, 0, 0,
        0, 0, 0, 1, 0
    } {}
    ColorMatrix(const float (&data)[20]) {
        std::copy(data, data + 20, m_data);
    }
    const float* getData() const {return m_data; }
};