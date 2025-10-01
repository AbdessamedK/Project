#ifndef INTERFACEKERNEL_HPP
#define INTERFACEKERNEL_HPP

#include "Interface.hpp"
#include <opencv2/opencv.hpp>

enum Type {
    KIRSH,
    SOBEL,
    PREWITT
};

template<Type T>
class InterfaceKernel : public Interface {
public:
    InterfaceKernel(const cv::Mat& img);
    void show(int deg) override;
    void onMouse(int event, int x, int y) override;

private:
    cv::Mat m_img;
    cv::Mat m_imgBase;
    cv::Rect buttons[6];
};

#endif // INTERFACEKERNEL_HPP
