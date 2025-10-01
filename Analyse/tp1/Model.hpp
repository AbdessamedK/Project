#ifndef MODEL_HPP
#define MODEL_HPP

#include <opencv2/opencv.hpp>

using namespace cv;


namespace model {
    enum Direction {
        HORIZONTAL,
        VERTICAL,
        D_PI_4,
        D_3PI_4
    };

    void putTextFit(const std::string& text, const Rect& rect, Mat& img, int fontFace = FONT_HERSHEY_SIMPLEX, int thickness = 2);
    Mat convolutionGris(const cv::Mat& img, const cv::Mat& kernel);
    Mat convolutionColor(const Mat& convolved_img, const Vec3b& color);
    void Hist(const Mat& img, double hist[256]);
    Mat applySeuil(const Mat& img, int seuille);
    int moyIntensite(const Mat& img, double k);
    Mat NormeModule(const Mat& img1, const Mat& img2, const Mat& img3, const Mat& img4);
    Mat maxModule(const Mat& img1, const Mat& img2, const Mat& img3, const Mat& img4);
    Mat absModule(const Mat& img1, const Mat& img2, const Mat& img3, const Mat& img4);
    Mat moyModule(const Mat& img1, const Mat& img2, const Mat& img3, const Mat& img4);
    Mat hysteresis(const Mat& img, double seuilBas, double seuilHaut, unsigned int steps = 1);
    Mat affinage(const Mat& img, Direction dir);
}

#endif // MODEL_HPP
