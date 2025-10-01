#include <opencv2/opencv.hpp>
#include <iostream>

#include "InterfaceMulti.hpp"
#include "InterfaceSeuil.hpp"
#include "InterfaceHysteresis.hpp"
#include "kernel.hpp"
#include "Model.hpp"

using namespace cv;
using namespace model;

template<Type T>
InterfaceMulti<T>::InterfaceMulti(const cv::Mat& img) : m_img(img.clone()), m_imgBase(img.clone()) {
    int espace = 10;
    int largeur_bouton = (m_img.cols - 3 * espace) / 4;  
    int hauteur = 70; 
    int y_offset1 = m_img.rows + 1; 
    int y_offset2 = m_img.rows + 75; 

    buttons[0] = Rect(0, y_offset1, largeur_bouton, hauteur);
    buttons[1] = Rect(largeur_bouton + espace, y_offset1, largeur_bouton, hauteur);
    buttons[2] = Rect(2 * largeur_bouton + 2 * espace, y_offset1, largeur_bouton, hauteur);
    buttons[3] = Rect(3 * largeur_bouton + 3 * espace, y_offset1, largeur_bouton, hauteur);

    buttons[4] = Rect(0, y_offset2, largeur_bouton, hauteur);
    buttons[5] = Rect(largeur_bouton + espace, y_offset2, largeur_bouton, hauteur);
    buttons[6] = Rect(2 * largeur_bouton + 2 * espace, y_offset2, largeur_bouton, hauteur);
    buttons[7] = Rect(3 * largeur_bouton + 3 * espace, y_offset2, largeur_bouton, hauteur);
}

template<Type T>
void InterfaceMulti<T>::show(int module) {
    Mat button_area(150, m_img.cols, CV_8UC1, Scalar(169));
    Mat display;

    Mat img, img1, img2, img3, img4;

    switch(T) {
        case KIRSH :
            img1 = convolutionGris(m_img, Kirsh1);
            img2 = convolutionGris(m_img, Kirsh2);
            img3 = convolutionGris(m_img, Kirsh3);
            img4 = convolutionGris(m_img, Kirsh4);
            if (aff) {
                img1 = affinage(img1, HORIZONTAL);
                img2 = affinage(img2, D_PI_4);
                img3 = affinage(img3, D_3PI_4);
                img4 = affinage(img4, VERTICAL);
            }
            break;
        case SOBEL :
            img1 = convolutionGris(m_img, Sobel1);
            img2 = convolutionGris(m_img, Sobel2);
            img3 = convolutionGris(m_img, Sobel3);
            img4 = convolutionGris(m_img, Sobel4);
            if (aff) {
                img1 = affinage(img1, HORIZONTAL);
                img2 = affinage(img2, D_PI_4);
                img3 = affinage(img3, D_3PI_4);
                img4 = affinage(img4, VERTICAL);
            }
            break;
        case PREWITT :
            img1 = convolutionGris(m_img, Prewitt1);
            img2 = convolutionGris(m_img, Prewitt2);
            img3 = convolutionGris(m_img, Prewitt3);
            img4 = convolutionGris(m_img, Prewitt4);
            if (aff) {
                img1 = affinage(img1, HORIZONTAL);
                img2 = affinage(img2, D_PI_4);
                img3 = affinage(img3, D_3PI_4);
                img4 = affinage(img4, VERTICAL);
            }
            break;
    }

    switch(module) {
        case 1 :
            img = absModule(img1, img2, img3, img4);
            break;
        case 2 :
            img = maxModule(img1, img2, img3, img4);
            break;
        case 3 :
            img = moyModule(img1, img2, img3, img4);
            break;
        default :
            img = NormeModule(img1, img2, img3, img4);
    }

    if (module == 10) {
        button_area = Mat(150, m_img.cols, CV_8UC3, Scalar(169,169,169));
        
        img1 = convolutionColor(img1, Vec3b(125,0,125));
        img2 = convolutionColor(img2, Vec3b(0,0,255));
        img3 = convolutionColor(img3, Vec3b(0,255,0));
        img4 = convolutionColor(img4, Vec3b(255,0,0));

        img = max(img1, img2);
        img = max(img, img3);
        img = max(img, img4);
    }

    m_img = img.clone();

    vconcat(img, button_area, display);

    for (unsigned int i = 0; i < 8; ++i) {
        rectangle(display, buttons[i], cv::Scalar(255, 255, 255), -1);
    }

    putTextFit("Norme", buttons[0], display);
    putTextFit("Somme abs", buttons[1], display);
    putTextFit("Maximum", buttons[2], display);
    putTextFit("Moyenne", buttons[3], display);
    putTextFit("Couleur", buttons[4], display);
    putTextFit("Seuil Unique", buttons[5], display);
    putTextFit("Hysteresis", buttons[6], display);
    putTextFit("Retour", buttons[7], display);

    imshow("Application", display);
}

template<Type T>
void InterfaceMulti<T>::onMouse(int event, int x, int y) {
    if (event == EVENT_LBUTTONDOWN) {
        if (buttons[0].contains(Point(x, y))) {

            Interface* newInterface = new InterfaceMulti<T>(m_imgBase);
            delete currentInterface;
            currentInterface = newInterface;
            currentInterface->show(0);

        } else if (buttons[1].contains(Point(x, y))) {

            Interface* newInterface = new InterfaceMulti<T>(m_imgBase);
            delete currentInterface;
            currentInterface = newInterface;
            currentInterface->show(1);

        } else if (buttons[2].contains(Point(x, y))) {

            Interface* newInterface = new InterfaceMulti<T>(m_imgBase);
            delete currentInterface;
            currentInterface = newInterface;
            currentInterface->show(2);

        } else if (buttons[3].contains(Point(x, y))) {

            Interface* newInterface = new InterfaceMulti<T>(m_imgBase);
            delete currentInterface;
            currentInterface = newInterface;
            currentInterface->show(3);

        } else if (buttons[4].contains(Point(x, y))) {

            Interface* newInterface = new InterfaceMulti<T>(m_imgBase);
            delete currentInterface;
            currentInterface = newInterface;
            currentInterface->show(10);

        } else if (buttons[5].contains(Point(x, y))) {

            Interface* newInterface = new InterfaceSeuil<T>(m_img, m_imgBase);
            delete currentInterface;
            currentInterface = newInterface;
            currentInterface->show(0);
            
        } else if (buttons[6].contains(Point(x, y))) {

            Interface* newInterface = new InterfaceHysteresis<T>(m_img, m_imgBase);
            delete currentInterface;
            currentInterface = newInterface;
            currentInterface->show(0);
            
        } else if (buttons[7].contains(Point(x, y))) {

            Interface* newInterface = new InterfaceKernel<T>(m_imgBase);
            delete currentInterface;
            currentInterface = newInterface;
            currentInterface->show(0);

        }
    }
}

template class InterfaceMulti<KIRSH>;
template class InterfaceMulti<SOBEL>;
template class InterfaceMulti<PREWITT>;
