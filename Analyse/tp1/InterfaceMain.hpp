#ifndef INTERFACEMAIN_HPP
#define INTERFACEMAIN_HPP

#include "Interface.hpp"
#include <opencv2/opencv.hpp>

class InterfaceMain : public Interface {
public:
    InterfaceMain(const cv::Mat& img);
    void show(int deg) override;
    void onMouse(int event, int x, int y) override;

private:
    cv::Rect buttons[3];
    cv::Mat m_img;
};

#endif // INTERFACEMAIN_HPP
