#include <opencv2/opencv.hpp>
#include <iostream>

#include "InterfaceHysteresis.hpp"
#include "InterfaceMulti.hpp"
#include "Model.hpp"
#include "kernel.hpp"

using namespace cv;
using namespace model;

template<Type T>
InterfaceHysteresis<T>::InterfaceHysteresis(const Mat& img, const Mat& imgBase) 
    : m_img(img.clone()), m_imgBase(imgBase.clone()), m_img2(img.clone()) {
    
    int longueur = m_img.cols / 3;
    int espace = 10; 

    buttons[0] = Rect(0, m_img.rows + 50, longueur - espace, 70); 
    buttons[1] = Rect(longueur, m_img.rows + 50, longueur - espace, 70); 
    buttons[2] = Rect(2 * longueur, m_img.rows + 50, longueur - espace, 70); 
}

template<Type T>
void InterfaceHysteresis<T>::show(int param) {
    Mat button_area(150, m_img.cols, CV_8UC1, Scalar(169));
    Mat display, img;

    if (param == 1) {
        vconcat(m_img, button_area, display);
        putTextFit("Veuillez suivre les instructions du terminal", Rect(0, m_img.rows, m_img.cols, 150), display);
        imshow("Application", display);
        waitKey(1);

        double seuilBas, seuilHaut;
        int profondeur;

        std::cout<< "Donnez le seuil bas : ";
        std::cin>> seuilBas;
        std::cout<< "Donnez le seuil haut : ";
        std::cin>> seuilHaut;
        std::cout<< "Donnez la profondeur (par exemple 2) : ";
        std::cin>> profondeur;

        m_img2 = hysteresis(m_img, seuilBas, seuilHaut, profondeur);

        show(2);
        return;
    }

    if (param == 0) { 
        double minVal, maxVal;
        minMaxLoc(m_img, &minVal, &maxVal);

        m_img2 = hysteresis(m_img2, 0.23*maxVal, 0.33*maxVal, 100);
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
void InterfaceHysteresis<T>::onMouse(int event, int x, int y) {
    if (event == EVENT_LBUTTONDOWN) {
        if (buttons[0].contains(Point(x, y))) {
            Interface* newInterface = new InterfaceHysteresis<T>(m_img, m_imgBase);
            delete currentInterface;
            currentInterface = newInterface;
            currentInterface->show(0);
        } else if (buttons[1].contains(Point(x, y))) {
            Interface* newInterface = new InterfaceHysteresis<T>(m_img, m_imgBase);
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

template class InterfaceHysteresis<KIRSH>;
template class InterfaceHysteresis<SOBEL>;
template class InterfaceHysteresis<PREWITT>;
