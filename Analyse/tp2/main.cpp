#include <iostream>
#include <opencv2/opencv.hpp>
#include "Model.hpp"
#include "kernel.hpp"

using namespace cv;

Mat img, resContour, resCerclefinal;
bool grad;

void onTrackbarChange(int, void*) {
    int seuil = getTrackbarPos("Seuil", "Droite Hough final");

    Mat res = model::houghDroite(model::Hough(resContour, seuil, grad), resContour.rows, resContour.cols);
    Mat imgRes = min(res, resContour);

    Mat resFinal = max(resCerclefinal, imgRes);
    
    imshow("Droite Hough", res);
    imshow("Droite Hough final", imgRes);
    imshow("Resultat final", resFinal);
}

void onTrackbarChange2(int, void*) {
    int seuil = getTrackbarPos("Seuil", "Fenetre2");

    Mat img1 = model::convolutionGris(img, Sobel1);
    Mat img2 = model::convolutionGris(img, Sobel2);
    Mat img3 = model::convolutionGris(img, Sobel3);
    Mat img4 = model::convolutionGris(img, Sobel4);

    Mat imgMax = model::NormeModule(img1, img2, img3, img4);

    Mat res = model::houghDroite(model::Hough(imgMax, seuil, true), resContour.rows, resContour.cols);
    res = min(res, imgMax);

    imshow("max", imgMax);
    imshow("Fenetre2", res);

}

int main(void) {
    srand(time(NULL));
    
    std::string filename;

    std::cout << "Donnez le chemin vers l'image (exemple : C:/Users/img.png)" << std::endl;
    std::cin >> filename;

    img = imread(filename, cv::IMREAD_GRAYSCALE);

    while (img.empty()) {
        std::cout << "Veuillez donner un chemin valide" << std::endl;
        std::cin >> filename;
        img = imread(filename, cv::IMREAD_GRAYSCALE);
    }

    std::cout<< "Quelle version voulez vous ? (0 pour vote uniforme et 1 pour vote pondere (gradiant))" <<std::endl;
    std::cin>> grad;

    int kernel, module = 0;

    std::cout<< "Quel kernel : " <<std::endl;
    std::cout<< "1 - Sobel"<<std::endl;
    std::cout<< "2 - Kirsh"<<std::endl;
    std::cout<< "3 - Prewitt"<<std::endl;
    std::cout<< "4 - Canny"<<std::endl;
    std::cin>> kernel;

    if (kernel != 4) {
        std::cout<< "Quel module : " <<std::endl;
        std::cout<< "1 - Norme" <<std::endl;
        std::cout<< "2 - Max" <<std::endl;
        std::cout<< "3 - Valeur absolu" <<std::endl;
        std::cout<< "4 - Moyenne" <<std::endl;
        std::cin>> module;
    }

    Mat img1, img2, img3, img4;

    switch (kernel) {
        case 1 : {
            img1 = model::convolutionGris(img, Sobel1);
            img2 = model::convolutionGris(img, Sobel2);
            img3 = model::convolutionGris(img, Sobel3);
            img4 = model::convolutionGris(img, Sobel4);
            break;
        }
        case 2 : {
            img1 = model::convolutionGris(img, Kirsh1);
            img2 = model::convolutionGris(img, Kirsh2);
            img3 = model::convolutionGris(img, Kirsh3);
            img4 = model::convolutionGris(img, Kirsh4);
            break;
        }
        default : {
            img1 = model::convolutionGris(img, Prewitt1);
            img2 = model::convolutionGris(img, Prewitt2);
            img3 = model::convolutionGris(img, Prewitt3);
            img4 = model::convolutionGris(img, Prewitt4);
        }
    }

    switch (module) {
        case 1 : {
            resContour = model::NormeModule(img1, img2, img3, img4);
            break;
        }
        case 2 : {
            resContour = model::maxModule(img1, img2, img3, img4);
            break;
        }
        case 3 : {
            resContour = model::absModule(img1, img2, img3, img4);
            break;
        }
        default : {
            resContour = model::moyModule(img1, img2, img3, img4);
        }
    }

    if (!grad && kernel != 4) {
        int s;
        std::cout<< "Donnez le seuil du post traitement"<<std::endl;
        std::cin>> s;
        resContour = model::applySeuil(resContour, s);
    }
    if (kernel == 4) {
        int b,h;
        std::cout<< "Donnez le seuil bas et haut pour Canny"<<std::endl;
        std::cin>> b >> h;
        Canny(img, resContour, b, h);
    }

    int version;
    std::cout<< "Que voulez vous ?"<<std::endl;
    std::cout<< "1 - Transformee de Hough (droite et cercle)"<<std::endl;
    std::cout<< "2 - RANSAC" <<std::endl;
    std::cin>> version;

    if (version == 2) {
        int iteration, seuilRansac;
        std::cout<< "Donnez le nombre d'iteration ainsi que le seuil"<<std::endl;
        std::cin>> iteration >> seuilRansac;

        std::pair<Point, Point> res = model::Ransac(resContour, iteration, seuilRansac);
        Mat ligne = Mat::zeros(img.size(), CV_8UC1);
        line(ligne, res.first, res.second, Scalar(255), 2);

        imshow("Image contours", resContour);
        imshow("Resultat final", ligne);

        waitKey(0);

        return 0;
    }

    int seuilCercle, rayonMin, rayonMax;
    
    std::cout<< "Donnez le seuil pour Hough Cercle" <<std::endl;
    std::cin>> seuilCercle;
    std::cout<< "Donnez rayonMin et rayonMax" <<std::endl;
    std::cin>> rayonMin >> rayonMax;

    imshow("Image contours", resContour);

    Mat resCercle = model::HoughCircleIm(resContour, rayonMin, rayonMax, seuilCercle);
    resCerclefinal = min(resContour, resCercle);

    namedWindow("Droite Hough final");
    createTrackbar("Seuil", "Droite Hough final", nullptr, 255, onTrackbarChange);
    
    int seuilbase;
    std::cout<< "Donnez le seuil de base pour les droites"<<std::endl;
    std::cin>> seuilbase;
    
    setTrackbarPos("Seuil", "Droite Hough final", seuilbase);

    imshow("Hough Cercle", resCercle);
    imshow("Hough Cercle final", resCerclefinal);

    namedWindow("Resultat final");

    waitKey(0);
    return 0;
}
