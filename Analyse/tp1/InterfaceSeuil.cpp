#include <opencv2/opencv.hpp>
#include <iostream>

#include "InterfaceSeuil.hpp"
#include "InterfaceMulti.hpp"
#include "Model.hpp"
#include "kernel.hpp"

using namespace cv;
using namespace model;

template<Type T>
InterfaceSeuil<T>::InterfaceSeuil(const Mat& img, const Mat& imgBase) 
    : m_img(img.clone()), m_imgBase(imgBase.clone()), m_img2(img.clone()) {
    
    int longueur = m_img.cols / 3;
    int espace = 10; 

    buttons[0] = Rect(0, m_img.rows + 50, longueur - espace, 70); 
    buttons[1] = Rect(longueur, m_img.rows + 50, longueur - espace, 70); 
    buttons[2] = Rect(2 * longueur, m_img.rows + 50, longueur - espace, 70); 
}

template<Type T>
void InterfaceSeuil<T>::show(int param) {
    Mat button_area(150, m_img.cols, CV_8UC1, Scalar(169));
    Mat display, img;

    if (param == 1) {
        vconcat(m_img, button_area, display);
        putTextFit("Veuillez suivre les instructions du terminal", Rect(0, m_img.rows, m_img.cols, 150), display);
        imshow("Application", display);
        waitKey(1);

        int seuil;
        std::cout << "Donnez le seuil : ";
        std::cin >> seuil;

        m_img2 = applySeuil(m_img, seuil);
        show(2);
        return;
    }

    if (param == 0) { 
        int seuil = moyIntensite(m_img, 0.8);
        m_img2 = applySeuil(m_img2, seuil);
    }

    img = m_img2;

    vconcat(img, button_area, display);

    for (unsigned int i = 0; i < 3; ++i) {
        rectangle(display, buttons[i], cv::Scalar(255, 255, 255), -1);
    }

    putTextFit("Automatique", buttons[0], display);
    putTextFit("Personnaliser", buttons[1], display);
    putTextFit("Retour", buttons[2], display);

    imshow("Application", display);
}

template<Type T>
void InterfaceSeuil<T>::onMouse(int event, int x, int y) {
    if (event == EVENT_LBUTTONDOWN) {
        if (buttons[0].contains(Point(x, y))) {
            Interface* newInterface = new InterfaceSeuil<T>(m_img, m_imgBase);
            delete currentInterface;
            currentInterface = newInterface;
            currentInterface->show(0);
        } else if (buttons[1].contains(Point(x, y))) {
            Interface* newInterface = new InterfaceSeuil<T>(m_img, m_imgBase);
            delete currentInterface;
            currentInterface = newInterface;
            currentInterface->show(1);
        } else if (buttons[2].contains(Point(x, y))) {
            Interface* newInterface = new InterfaceMulti<T>(m_imgBase);
            delete currentInterface;
            currentInterface = newInterface;
            currentInterface->show(0);
        }
    }
}

template class InterfaceSeuil<KIRSH>;
template class InterfaceSeuil<SOBEL>;
template class InterfaceSeuil<PREWITT>;
