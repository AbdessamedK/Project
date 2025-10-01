#ifndef KERNELS_HPP
#define KERNELS_HPP

#include <opencv2/opencv.hpp>
#include "Interface.hpp"

// Kirsh
extern cv::Mat Kirsh1;
extern cv::Mat Kirsh2;
extern cv::Mat Kirsh3;
extern cv::Mat Kirsh4;


// Prewitt
extern cv::Mat Prewitt1;
extern cv::Mat Prewitt2;
extern cv::Mat Prewitt3;
extern cv::Mat Prewitt4;


// Sobel
extern cv::Mat Sobel1;
extern cv::Mat Sobel2;
extern cv::Mat Sobel3;
extern cv::Mat Sobel4;


// Interface graphique
extern Interface* currentInterface;

// Affinage ou non
extern bool aff;

#endif // KERNELS_HPP
