#include<opencv2/opencv.hpp>
#include<iostream>
#include "InterfaceMain.hpp"
#include "kernel.hpp"

using namespace cv;

void mouseCallback(int event, int x, int y, int, void*) {
    if (currentInterface) {
        currentInterface->onMouse(event, x, y);
    }
}

int main() {
    cv::Mat img;
    std::string filename;

    std::cout << "Donnez le chemin vers l'image (exemple : C:/Users/img.png)" << std::endl;
    std::cin >> filename;

    img = cv::imread(filename, cv::IMREAD_GRAYSCALE);

    while (img.empty()) {
        std::cout << "Veuillez donner un chemin valide" << std::endl;
        std::cin >> filename;
        img = cv::imread(filename, cv::IMREAD_GRAYSCALE);
    }

    std::cout<< "Voulez vous appliquer l'affinage a vos images ? (0/1)" <<std::endl;
    std::cin>> aff;

    InterfaceMain interface(img);

    namedWindow("Application", WINDOW_NORMAL);

    currentInterface = new InterfaceMain(img);
    currentInterface->show(0);

    setMouseCallback("Application", mouseCallback);

    while(true) {
        char key = cv::waitKey(1);
        if (key == 27) {
            break;
        }
    }

    delete currentInterface;
    destroyAllWindows();

    return 0;
}


