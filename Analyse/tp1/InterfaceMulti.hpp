#ifndef INTERFACEMULTI_HPP
#define INTERFACEMULTI_HPP

#include "Interface.hpp"
#include "InterfaceKernel.hpp"
#include <opencv2/opencv.hpp>

template<Type T>
class InterfaceMulti : public Interface {
public:
    InterfaceMulti(const cv::Mat& img);
    void show(int module) override;
    void onMouse(int event, int x, int y) override;

private:
    cv::Rect buttons[8];
    cv::Mat m_img;
    cv::Mat m_imgBase;
};

#endif // INTERFACEMULTI_HPP
