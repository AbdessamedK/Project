#include <opencv2/opencv.hpp>
#include <iostream>

#include "InterfaceKernel.hpp"
#include "InterfaceMain.hpp"
#include "InterfaceMulti.hpp"
#include "Model.hpp"
#include "kernel.hpp"

using namespace cv;
using namespace model;

template<Type T>
InterfaceKernel<T>::InterfaceKernel(const cv::Mat& img) : m_img(img.clone()), m_imgBase(img.clone()) {
    int longueur = m_img.cols / 3; 
    int espace = 10; 
    int hauteur = 70; 
    int y_offset = m_img.rows + 1; 
    int y_offset2 = m_img.rows + 75;

    buttons[0] = Rect(0, y_offset, longueur - espace, hauteur); 
    buttons[1] = Rect(longueur + espace, y_offset, longueur - espace, hauteur);
    buttons[2] = Rect(2 * longueur + 2 * espace, y_offset, longueur - espace, hauteur); 

    buttons[3] = Rect(0, y_offset2, longueur - espace, hauteur); 
    buttons[4] = Rect(longueur + espace, y_offset2, longueur - espace, hauteur); 
    buttons[5] = Rect(2 * longueur + 2 * espace, y_offset2, longueur - espace, hauteur);

}

template<Type T>
void InterfaceKernel<T>::show(int deg) {
    
    Mat img;

    switch(T) {
        case KIRSH : 
            switch (deg) {
                case 2 :
                    img = convolutionGris(m_img, Kirsh2);
                    if (aff) img = affinage(img, D_PI_4);
                    break;
                case 3 :
                    img = convolutionGris(m_img, Kirsh3);
                    if (aff) img = affinage(img, VERTICAL);
                    break;
                case 4 :
                    img = convolutionGris(m_img, Kirsh4);
                    if (aff) img = affinage(img, D_3PI_4);
                    break;
                default :
                    img = convolutionGris(m_img, Kirsh1);
                    if (aff) img = affinage(img, HORIZONTAL);
            }
            break;
        case SOBEL :
            switch (deg) {
                case 2 :
                    img = convolutionGris(m_img, Sobel2);
                    if (aff) img = affinage(img, D_PI_4);
                    break;
                case 3 :
                    img = convolutionGris(m_img, Sobel3);
                    if (aff) img = affinage(img, D_3PI_4);
                    break;
                case 4 :
                    img = convolutionGris(m_img, Sobel4);
                    if (aff) img = affinage(img, VERTICAL);
                    break;
                default :
                    img = convolutionGris(m_img, Sobel1);
                    if (aff) img = affinage(img, HORIZONTAL);
            }
            break;
        case PREWITT :
            switch (deg) {
                case 2 :
                    img = convolutionGris(m_img, Prewitt2);
                    if (aff) img = affinage(img, VERTICAL);
                    break;
                case 3 :
                    img = convolutionGris(m_img, Prewitt3);
                    if (aff) img = affinage(img, D_PI_4);
                    break;
                case 4 :
                    img = convolutionGris(m_img, Prewitt4);
                    if (aff) img = affinage(img, D_3PI_4);
                    break;
                default :
                    img = convolutionGris(m_img, Prewitt1);
                    if (aff) img = affinage(img, HORIZONTAL);
            }
            break;
    }

    Mat button_area(150, img.cols, CV_8UC1, cv::Scalar(169));
    Mat display;

    vconcat(img, button_area, display);

    for (unsigned int i = 0; i < 6; ++i) {
        rectangle(display, buttons[i], cv::Scalar(255, 255, 255), -1);
    }

    putTextFit("kernel1", buttons[0], display);
    putTextFit("kernel2", buttons[1], display);
    putTextFit("kernel3", buttons[2], display);
    putTextFit("kernel4", buttons[3], display);
    putTextFit("multi-kernel", buttons[4], display);
    putTextFit("retour", buttons[5], display);

    imshow("Application", display);
}

template<Type T>
void InterfaceKernel<T>::onMouse(int event, int x, int y) {
    if (event == EVENT_LBUTTONDOWN) {
        if (buttons[0].contains(Point(x, y))) {

            Interface* newInterface = new InterfaceKernel<T>(m_imgBase);
            delete currentInterface;
            currentInterface = newInterface;
            currentInterface->show(1);

        } else if (buttons[1].contains(Point(x, y))) {

            Interface* newInterface = new InterfaceKernel<T>(m_imgBase);
            delete currentInterface;
            currentInterface = newInterface;
            currentInterface->show(2);

        } else if (buttons[2].contains(Point(x, y))) {

            Interface* newInterface = new InterfaceKernel<T>(m_imgBase);
            delete currentInterface;
            currentInterface = newInterface;
            currentInterface->show(3);

        } else if (buttons[3].contains(Point(x, y))) {

            Interface* newInterface = new InterfaceKernel<T>(m_imgBase);
            delete currentInterface;
            currentInterface = newInterface;
            currentInterface->show(4);

        } else if (buttons[4].contains(Point(x, y))) {

            Interface* newInterface = new InterfaceMulti<T>(m_imgBase);
            delete currentInterface;
            currentInterface = newInterface;
            currentInterface->show(0);

        } else if (buttons[5].contains(Point(x, y))) {
            Interface* newInterface = new InterfaceMain(m_imgBase);
            delete currentInterface;
            currentInterface = newInterface;
            currentInterface->show(0);
        }
    }
}

template class InterfaceKernel<KIRSH>;
template class InterfaceKernel<SOBEL>;
template class InterfaceKernel<PREWITT>;
