#pragma once

#include <algorithm>
#include "./FrameBuffer.hpp"
#include "./ColorMatrix.hpp"


void applyTransformation(FrameBuffer& fb, const ColorMatrix& cm) {
    uint8_t* data = fb.getData();
    const float* m = cm.getData();
    unsigned short int channels = fb.getChannels();
    
    for (size_t i = 0; i < fb.getWidth() * fb.getHeight() * channels;
            i += channels) {
        float r = data[i];
        float g = data[i+1];
        float b = data[i+2];
        float a = channels == 4 ? data[i+3] : 255.0;

        float newR = m[0]*r + m[1]*g + m[2]*b + m[3]*a + m[4];
        float newG = m[5]*r + m[6]*g + m[7]*b + m[8]*a + m[9];
        float newB = m[10]*r + m[11]*g + m[12]*b + m[13]*a + m[14];

        data[i] = static_cast<uint8_t>(std::clamp(newR, 0.0f, 255.0f));
        data[i+1] = static_cast<uint8_t>(std::clamp(newG, 0.0f, 255.0f));
        data[i+2] = static_cast<uint8_t>(std::clamp(newB, 0.0f, 255.0f));

        if (channels == 4) {
            float newA = m[15]*r + m[16]*g + m[17]*b + m[18]*a + m[19];
            data[i+3] = static_cast<uint8_t>(std::clamp(newA, 0.0f, 255.0f));
        }
    }
}