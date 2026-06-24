#include <iostream>
#include <fstream>
#include <cmath>
#include "../include/FrameBuffer.hpp"
#include "../include/ColorMatrix.hpp"
#include "../include/Engine.hpp"

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
    std::ofstream ofs("test_output.ppm");
    ofs << "P3\n" << fb.getWidth() << " " << fb.getHeight() << "\n255\n";
    uint8_t* dataPtr = fb.getData();
    for (size_t i = 0; i < fb.getWidth()*fb.getHeight()*fb.getChannels(); i+=fb.getChannels()) {
        ofs << static_cast<int>(dataPtr[i]) << " " << static_cast<int>(dataPtr[i+1]) << " " << static_cast<int>(dataPtr[i+2]) << "\n";
    }
    ofs.close();
    std::cout << "Test finished. Check the result...";
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
    std::cout << "Results: " << passed << " / " << total << "\n";
}

int main() {
    runAllTests();
    return 0;
}