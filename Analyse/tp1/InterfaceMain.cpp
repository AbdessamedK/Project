#include <opencv2/opencv.hpp>
#include <iostream>

#include "InterfaceMain.hpp"
#include "InterfaceKernel.hpp"
#include "kernel.hpp"
#include "Model.hpp"

using namespace cv;
using namespace model;

InterfaceMain::InterfaceMain(const cv::Mat& img) : m_img(img.clone()) {
    int longueur = m_img.cols / 3;
    int espace = 10; 

    buttons[0] = Rect(0, m_img.rows + 50, longueur - espace, 70); 
    buttons[1] = Rect(longueur, m_img.rows + 50, longueur - espace, 70); 
    buttons[2] = Rect(2 * longueur, m_img.rows + 50, longueur - espace, 70); 

}

void InterfaceMain::show(int deg) {
    Mat button_area(150, m_img.cols, CV_8UC1, Scalar(169));
    Mat display;
    vconcat(m_img, button_area, display);

    for (unsigned int i = 0; i < 3; ++i) {
        rectangle(display, buttons[i], cv::Scalar(255, 255, 255), -1);
    }

    putTextFit("Kirsh", buttons[0], display);
    putTextFit("Prewitt", buttons[1], display);
    putTextFit("Sobel", buttons[2], display);

    imshow("Application", display);
}

void InterfaceMain::onMouse(int event, int x, int y) {
    if (event == EVENT_LBUTTONDOWN) {
        if (buttons[0].contains(Point(x, y))) {
            Interface* newInterface = new InterfaceKernel<KIRSH>(m_img);
            delete currentInterface;
            currentInterface = newInterface;
            currentInterface->show(1);
        } else if (buttons[1].contains(Point(x, y))) {
            Interface* newInterface = new InterfaceKernel<PREWITT>(m_img);
            delete currentInterface;
            currentInterface = newInterface;
            currentInterface->show(1);
        } else if (buttons[2].contains(Point(x, y))) {
            Interface* newInterface = new InterfaceKernel<SOBEL>(m_img);
            delete currentInterface;
            currentInterface = newInterface;
            currentInterface->show(1);
        }
    }
}
