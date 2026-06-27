#pragma once

#include <optional>
#include <stdexcept>
#include <opencv2/opencv.hpp>
#include "./FrameBuffer.hpp"

class VideoReader {
private:
    cv::VideoCapture m_video;
    bool m_isFinished;
public:
    VideoReader(const std::string& name) : m_video(name), m_isFinished(false) {
        if (!m_video.isOpened())
            throw std::runtime_error("Canno open the file: " + name);
    }
    bool hasFrame() const {
        return !m_isFinished;
    }
    std::optional<FrameBuffer> getNextFrame() {
        if (!hasFrame())
            return std::nullopt;
        cv::Mat frame;
        m_video >> frame;
        if (frame.empty()) {
            m_isFinished = true;
            return std::nullopt;
        }
        size_t w = frame.cols;
        size_t h = frame.rows;
        unsigned short int channels = frame.channels();
        auto newData = new uint8_t[w*h*channels];
        const uint8_t* cvData = frame.data;
        if (channels == 3){
            for (size_t i = 0; i < w * h * channels; i += 3) {
                newData[i] = cvData[i + 2];
                newData[i + 1] = cvData[i + 1];
                newData[i + 2] = cvData[i];
            }
        }else {
            std::copy(cvData, cvData + (w * h * channels), newData);
        }
        return FrameBuffer(newData, w, h, channels);
    }
};