#ifndef INTERFACEHYSTERESIS_HPP
#define INTERFACEHYSTERESIS_HPP

#include "Interface.hpp"
#include "InterfaceKernel.hpp"
#include <opencv2/opencv.hpp>

template<Type T>
class InterfaceHysteresis : public Interface {
public:
    InterfaceHysteresis(const cv::Mat& img, const cv::Mat& img2);
    void show(int param) override;
    void onMouse(int event, int x, int y) override;

private:
    cv::Rect buttons[3];
    cv::Mat m_img;
    cv::Mat m_imgBase;
    cv::Mat m_img2;
};

#endif // INTERFACEHYSTERESIS_HPP
