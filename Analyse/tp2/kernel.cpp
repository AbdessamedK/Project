#include "kernel.hpp"

cv::Mat Kirsh1 = (cv::Mat_<float>(3, 3) <<
                    5, 5, 5,
                    -3, 0, -3,
                    -3, -3, -3) / 15.0;

cv::Mat Kirsh2 = (cv::Mat_<float>(3, 3) <<
                    5, 5, -3,
                    5, 0, -3,
                    -3, -3, -3) / 15.0;

cv::Mat Kirsh3 = (cv::Mat_<float>(3, 3) <<
                    5, -3, -3,
                    5, 0, -3,
                    5, -3, -3) / 15.0;

cv::Mat Kirsh4 = (cv::Mat_<float>(3, 3) <<
                    -3, -3, -3,
                    5, 0, -3,
                    5, 5, -3) / 15.0;

cv::Mat Prewitt1  = (cv::Mat_<float>(3, 3) <<
                    -1, 0, 1,
                    -1, 0, 1,
                    -1, 0, 1)/6.0 ;

cv::Mat Prewitt2 = (cv::Mat_<float>(3, 3) <<
                    -1, -1, -1,
                    0, 0, 0,
                    1, 1, 1) / 6.0;

cv::Mat Prewitt3 = (cv::Mat_<float>(3, 3) <<
                    0,  1,  1,
                    -1,  0,  1,
                    -1, -1,  0);

cv::Mat Prewitt4 = (cv::Mat_<float>(3, 3) <<
                    1,  1,  0,
                    1,  0, -1,
                    0, -1, -1);

cv::Mat Sobel1 = (cv::Mat_<float>(3, 3) <<
                    -1, 0, 1,
                    -2, 0, 2,
                    -1, 0, 1) / 4.0;

cv::Mat Sobel2 = (cv::Mat_<float>(3, 3) <<
                    0, 1, 2,
                    -1, 0, 1,
                    -2, -1, 0) / 4.0;

cv::Mat Sobel3 = (cv::Mat_<float>(3, 3) <<
                    2, 1, 0,
                    1, 0, -1,
                    0, -1, -2) / 4.0;

cv::Mat Sobel4 = (cv::Mat_<float>(3, 3) <<
                    -1, -2, -1,
                    0, 0, 0,
                    1, 2, 1) / 4.0;

Interface* currentInterface = nullptr;

bool aff = false;