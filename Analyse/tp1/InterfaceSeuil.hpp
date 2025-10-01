#ifndef INTERFACESEUIL_HPP
#define INTERFACESEUIL_HPP

#include "Interface.hpp"
#include "InterfaceKernel.hpp"
#include <opencv2/opencv.hpp>

template<Type T>
class InterfaceSeuil : public Interface {
public:
    InterfaceSeuil(const cv::Mat& img, const cv::Mat& imgBase);
    void show(int param) override;
    void onMouse(int event, int x, int y) override;

private:
    cv::Rect buttons[3];
    cv::Mat m_img;
    cv::Mat m_imgBase;
    cv::Mat m_img2;
};

#endif // INTERFACESEUIL_HPP
