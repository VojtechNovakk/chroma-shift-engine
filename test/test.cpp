#include <iostream>
#include <fstream>
#include <cmath>
#include "../include/FrameBuffer.hpp"
#include "../include/ColorMatrix.hpp"
#include "../include/Engine.hpp"
#include "../include/VideoReader.hpp"

#define UNIT_TEST(condition, message) \
    do { \
        if(!(condition)) { \
            std::cout << " [FAIL] " << message << "(Row : " << __LINE__ << " )\n"; \
            return false; \
        }   \
    } while(0)

constexpr float EPSILON = 1e-9;

bool testFrameBufferConstructor() {
    size_t w = 100;
    size_t h = 50;
    unsigned short int ch = 3;

    uint8_t* mockData = new uint8_t[w*h*ch];
    for (size_t i = 0; i < w*h*ch; i += ch) {
        mockData[i] = 255;
        mockData[i+1] = 0;
        mockData[i+2] = 0;
    }
    FrameBuffer fb(mockData, w, h, ch);

    UNIT_TEST(fb.getWidth() == w, "Width mismatch");
    UNIT_TEST(fb.getHeight() == h, "Height mismatch");
    UNIT_TEST(fb.getChannels() == ch, "Channels mismatch");
    UNIT_TEST(fb.getData() != nullptr, "Data pointer is null");

    std::ofstream ofs("test_output.ppm");
    ofs << "P3\n" << fb.getWidth() << " " << fb.getHeight() << "\n255\n";
    uint8_t* dataPtr = fb.getData();
    for (size_t i = 0; i < fb.getWidth()*fb.getHeight()*fb.getChannels(); i+=fb.getChannels()) {
        ofs << static_cast<int>(dataPtr[i]) << " " << static_cast<int>(dataPtr[i+1]) << " " << static_cast<int>(dataPtr[i+2]) << "\n";
    }
    ofs.close();
    std::cout << "Test finished. Check the result...";

    FrameBuffer fb_copy(fb);
    UNIT_TEST(fb_copy.getWidth() == w, "Width mismatch");
    UNIT_TEST(fb_copy.getHeight() == h, "Height mismatch");
    UNIT_TEST(fb_copy.getChannels() == ch, "Channels mismatch");
    UNIT_TEST(fb_copy.getData() != nullptr, "Data pointer is null");

    fb_copy = fb;
    UNIT_TEST(fb_copy.getWidth() == w, "Width mismatch");
    UNIT_TEST(fb_copy.getHeight() == h, "Height mismatch");
    UNIT_TEST(fb_copy.getChannels() == ch, "Channels mismatch");
    UNIT_TEST(fb_copy.getData() != nullptr, "Data pointer is null");
    return true;
}

bool testColorMatrix() {
    float matrix[20] = {
        0, 0, 0, 0, 0,
        0, 0, 1, 1, 0,
        1, 1, 1, 1, 1,
        1, 1, 1, 1, 1
    };
    ColorMatrix cm(matrix);
    UNIT_TEST(std::abs(cm.getData()[0] - 0) < EPSILON, "Values dont match!");
    UNIT_TEST(std::abs(cm.getData()[10] - 1) < EPSILON, "Values dont match!");
    size_t w = 100;
    size_t h = 50;
    unsigned short int ch = 3;

    uint8_t* mockData = new uint8_t[w*h*ch];
    for (size_t i = 0; i < w*h*ch; i += ch) {
        mockData[i] = 255;
        mockData[i+1] = 0;
        mockData[i+2] = 0;
    }
    FrameBuffer fb(mockData, w, h, ch);
    applyTransformation(fb, cm);
    std::ofstream ofs("test_output2.ppm");
    ofs << "P3\n" << fb.getWidth() << " " << fb.getHeight() << "\n255\n";
    uint8_t* dataPtr = fb.getData();
    for (size_t i = 0; i < fb.getWidth()*fb.getHeight()*fb.getChannels(); i+=fb.getChannels()) {
        ofs << static_cast<int>(dataPtr[i]) << " " << static_cast<int>(dataPtr[i+1]) << " " << static_cast<int>(dataPtr[i+2]) << "\n";
    }
    ofs.close();
    std::cout << "Test finished. Check the result...";
    return true;
}

bool testVideoReader() {
    std::string filename = "output.mp4";
    int fourcc = cv::VideoWriter::fourcc('M','P','4','V');
    double fps = 30.0;
    cv::Size frameSize(640, 480);
    bool isColor = true;

    cv::VideoWriter writer(filename, fourcc, fps, frameSize, isColor);
    if (!writer.isOpened())
        throw std::runtime_error("Cannot open the file!");
    cv::Mat frame(480, 640, CV_8UC3, cv::Scalar(255, 0, 0));
    for (size_t i = 0; i < 5; ++i)
        writer.write(frame);

    writer.release();

    VideoReader video("output.mp4");
    int frameCount = 0;
    while (std::optional<FrameBuffer> optFrame = video.getNextFrame()) {
        FrameBuffer& currentFrame = *optFrame;
        ++frameCount;

        const uint8_t* data = currentFrame.getData();
        UNIT_TEST(data[0] < 50, "Red channel should be 0!");
        UNIT_TEST(data[1] < 50, "Green channel should be 0!");
        UNIT_TEST(data[2] > 200, "Blue channel should be 255!");

        float m_data[20] = {
            1, 1, 1, 1, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0
        };
        ColorMatrix matrix(m_data);
        applyTransformation(currentFrame, matrix);
        UNIT_TEST(data[0] > 200, "Red channel should be 255 after transform");
        UNIT_TEST(data[1] < 50, "Green channel should be 0 after transform");
        UNIT_TEST(data[2] < 50, "Blue channel should be 0 after transform");
    }
    UNIT_TEST(frameCount == 5, "VideoReader should read exactly 5 frames");
    std::remove(filename.c_str());
    return true;
}

void runAllTests() {
    std::cout << "Running tests!\n";

    int passed = 0;
    int total = 0;
    auto runTest = [&passed, &total](bool (*testFunc)(), const std::string& name) {
        total++;
        std::cout << "Runs: " << name << " ... ";
        if (testFunc()) {
            std::cout << "OK\n";
            passed++;
        }
    };
    runTest(testFrameBufferConstructor, "Init Frame Buffer");
    runTest(testColorMatrix, "Color Matrix and Transformation");
    runTest(testVideoReader, "Video Reader");
    std::cout << "Results: " << passed << " / " << total << "\n";
}

int main() {
    runAllTests();
    return 0;
}