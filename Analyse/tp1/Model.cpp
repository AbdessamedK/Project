// utils.cpp
#include "Model.hpp"

using namespace cv;

/*
    Pour ecrire du texte sur l'interface graphique selon un cv::Rect, 
    procedure generer avec une IA.
*/
void model::putTextFit(const std::string& text, const Rect& rect, Mat& img, int fontFace, int thickness) {
    // Initialiser l'échelle de la police (fontScale) pour commencer avec une taille de texte arbitraire
    double fontScale = 1.0;

    // Obtenir la taille du texte avec la taille de police initiale
    Size textSize = getTextSize(text, fontFace, fontScale, thickness, nullptr);

    // Ajuster la taille de la police pour que le texte tienne dans le rectangle
    while (textSize.width > rect.width || textSize.height > rect.height) {
        fontScale -= 0.05;  // Réduire progressivement la taille de la police
        textSize = getTextSize(text, fontFace, fontScale, thickness, nullptr);
    }

    // Calculer la position pour centrer le texte dans le rectangle
    Point textOrg(rect.x + (rect.width - textSize.width) / 2, rect.y + (rect.height + textSize.height) / 2);

    // Ajouter le texte sur l'image
    putText(img, text, textOrg, fontFace, fontScale, Scalar(0, 0, 0), thickness, LINE_AA);
}

Mat model::convolutionGris(const cv::Mat& img, const cv::Mat& kernel) {
    cv::Mat res = img.clone();
    int half = kernel.rows / 2;

    cv::Mat imgBorder;
    cv::copyMakeBorder(img, imgBorder, half, half, half, half, cv::BORDER_DEFAULT);

    for (int i = 0; i < img.rows; ++i) {
        for (int j = 0; j < img.cols; ++j) {
            float somme = 0.0;

            for (int y = 0; y < kernel.rows; ++y) {
                for (int x = 0; x < kernel.cols; ++x) {
                    int dy = i + y;
                    int dx = j + x;

                    somme += kernel.at<float>(y, x) * imgBorder.at<uchar>(dy, dx);
                }
            }

            res.at<uchar>(i, j) = cv::saturate_cast<uchar>(somme);
        }
    }
    return res;
}

Mat model::affinage(const Mat& img, model::Direction dir) {
    Mat res = img.clone();

    for (int i = 1; i < img.rows - 1; ++i) {
        for (int j = 1; j < img.cols - 1; ++j) {
            switch(dir) {
                case HORIZONTAL : {
                    res.at<uchar>(i,j) = 
                        img.at<uchar>(i,j) >= img.at<uchar>(i+1,j) && img.at<uchar>(i,j) >= img.at<uchar>(i-1,j)
                        ? img.at<uchar>(i,j) : saturate_cast<uchar>(0);

                    break;
                }
                case VERTICAL : {
                    res.at<uchar>(i,j) = 
                        img.at<uchar>(i,j) >= img.at<uchar>(i,j+1) && img.at<uchar>(i,j) >= img.at<uchar>(i,j-1)
                        ? img.at<uchar>(i,j) : saturate_cast<uchar>(0);
                        
                    break;
                }
                case D_PI_4 : {
                    res.at<uchar>(i,j) = 
                        img.at<uchar>(i,j) >= img.at<uchar>(i-1,j+1) && img.at<uchar>(i,j) >= img.at<uchar>(i+1,j-1)
                        ? img.at<uchar>(i,j) : saturate_cast<uchar>(0);
                        
                    break;
                }
                case D_3PI_4 : {
                    res.at<uchar>(i,j) = 
                        img.at<uchar>(i,j) >= img.at<uchar>(i-1,j-1) && img.at<uchar>(i,j) >= img.at<uchar>(i+1,j+1)
                        ? img.at<uchar>(i,j) : saturate_cast<uchar>(0);
                        
                    break;
                }
            }
        }
    }

    return res;
}

Mat model::convolutionColor(const Mat& convolved_img, const Vec3b& color) {
    // Crée une image de sortie avec 3 canaux (pour les couleurs)
    Mat colored_img = Mat::zeros(convolved_img.size(), CV_8UC3);

    // Parcourt chaque pixel de l'image convoluée
    for (int i = 0; i < convolved_img.rows; ++i) {
        for (int j = 0; j < convolved_img.cols; ++j) {
            // Récupère la valeur du pixel convolué
            uchar intensity = convolved_img.at<uchar>(i, j);

            // Multiplie l'intensité par la couleur
            colored_img.at<Vec3b>(i, j)[0] = saturate_cast<uchar>((intensity * 1.75 * color[0]) / 255.0); // Bleu
            colored_img.at<Vec3b>(i, j)[1] = saturate_cast<uchar>((intensity * 1.75 * color[1]) / 255.0); // Vert
            colored_img.at<Vec3b>(i, j)[2] = saturate_cast<uchar>((intensity * 1.75 * color[2]) / 255.0); // Rouge
        }
    }

    return colored_img;
}

void model::Hist(const Mat& img, double hist[256]) {
    for (int i = 0; i < 256; ++i) {
        hist[i] = 0;
    }

    for (int i = 0; i < img.rows; ++i) {
        for (int j = 0; j < img.cols; ++j) {
            hist[(int) img.at<uchar>(i,j)]++;
        }
    }

    for (int i = 0; i < 256; ++i) {
        hist[i] = (double) hist[i] / ((double) (img.cols * img.rows));
    }
}

Mat model::applySeuil(const Mat& img, int seuille) {
    Mat res = img.clone();
    
    double hist[256];
    model::Hist(img, hist);

    for (int i = 0; i < img.rows; ++i) {
        for (int j = 0; j < img.cols; ++j) {
            if ((int)img.at<uchar>(i,j) <= seuille) {
                res.at<uchar>(i,j) = saturate_cast<uchar>(0);
            }
            else {
                res.at<uchar>(i,j) = saturate_cast<uchar>(255);
            }   
        }
    }
    
    return res;
}

int model::moyIntensite(const Mat& img, double k) {
    float somme = 0;
    float moyenne = 0.f;
    float sommeET = 0.f;
    float ecartType = 0.f;

    for (int i = 0; i < img.rows; ++i) {
        for (int j = 0; j < img.cols; ++j) {
            somme += (int) img.at<uchar>(i,j);
        }
    }

    moyenne = somme / (img.cols * img.rows);

    for (int i = 0; i < img.rows; ++i) {
        for (int j = 0; j < img.cols; ++j) {
            sommeET += (img.at<uchar>(i,j) - moyenne) * (img.at<uchar>(i,j) - moyenne);
        }
    }

    ecartType = sommeET / (img.cols * img.rows);
    ecartType = std::sqrt(ecartType);

    return moyenne + (k * ecartType);
}

Mat model::NormeModule(const Mat& img1, const Mat& img2, const Mat& img3, const Mat& img4) {
    Mat output = img1.clone();

    for (int i = 0; i < output.rows; ++i) {
        for (int j = 0; j < output.cols; ++j) {

            int g1 = (int)img1.at<uchar>(i,j);
            int g2 = (int)img2.at<uchar>(i,j);
            int g3 = (int)img3.at<uchar>(i,j);
            int g4 = (int)img4.at<uchar>(i,j);

            output.at<uchar>(i,j) = std::sqrt( g1*g1 + g2*g2 + g3*g3 + g4*g4 );
        }
    }
    return output;
}

Mat model::maxModule(const Mat& img1, const Mat& img2, const Mat& img3, const Mat& img4) {
    Mat output = img1.clone();

    for (int i = 0; i < output.rows; ++i) {
        for (int j = 0; j < output.cols; ++j) {

            int g1 = (int)img1.at<uchar>(i,j);
            int g2 = (int)img2.at<uchar>(i,j);
            int g3 = (int)img3.at<uchar>(i,j);
            int g4 = (int)img4.at<uchar>(i,j);

            output.at<uchar>(i,j) = std::max( std::max(g1, g2), std::max(g3, g4) );
        }
    }
    return output;
}

Mat model::absModule(const Mat& img1, const Mat& img2, const Mat& img3, const Mat& img4) {
    Mat output = img1.clone();

    for (int i = 0; i < output.rows; ++i) {
        for (int j = 0; j < output.cols; ++j) {

            int g1 = (int)img1.at<uchar>(i,j);
            int g2 = (int)img2.at<uchar>(i,j);
            int g3 = (int)img3.at<uchar>(i,j);
            int g4 = (int)img4.at<uchar>(i,j);

            output.at<uchar>(i,j) = std::abs(g1) + std::abs(g2) + std::abs(g3) + std::abs(g4);
        }
    }
    return output;
}

Mat model::moyModule(const Mat& img1, const Mat& img2, const Mat& img3, const Mat& img4) {
    Mat output = img1.clone();

    for (int i = 0; i < output.rows; ++i) {
        for (int j = 0; j < output.cols; ++j) {

            int g1 = (int)img1.at<uchar>(i,j);
            int g2 = (int)img2.at<uchar>(i,j);
            int g3 = (int)img3.at<uchar>(i,j);
            int g4 = (int)img4.at<uchar>(i,j);

            output.at<uchar>(i,j) = (g1+g2+g3+g4) / 4.f;
        }
    }
    return output;
}

Mat model::hysteresis(const Mat& img, double seuilBas, double seuilHaut, unsigned int steps) {
    CV_Assert(steps > 0);

    Mat res = img.clone();

    Mat imgBorder;
    copyMakeBorder(img, imgBorder, 1, 1, 1, 1, BORDER_DEFAULT);

    for (int i = 0; i < img.rows; ++i) {
        for (int j = 0; j < img.cols; ++j) {
            
            if ((int) img.at<uchar>(i,j) < seuilBas) {
                res.at<uchar>(i,j) = saturate_cast<uchar>(0);
                imgBorder.at<uchar>(i+1,j+1) = saturate_cast<uchar>(0);
            } else if ((int) img.at<uchar>(i,j) > seuilHaut) {
                res.at<uchar>(i,j) = saturate_cast<uchar>(255);
                imgBorder.at<uchar>(i+1,j+1) = saturate_cast<uchar>(255);
            }
        }
    }

    for (int step = 0; step < steps; ++step) {
        copyMakeBorder(res, imgBorder, 1, 1, 1, 1, BORDER_DEFAULT);

        for (int i = 0; i < img.rows; ++i) {
            for (int j = 0; j < img.cols; ++j) {
                
                if (res.at<uchar>(i,j) >= seuilBas && res.at<uchar>(i,j) <= seuilHaut) {

                    bool voisinFort = false;
                    
                    for (int y = 0; y < 3; ++y) {
                        for (int x = 0; x < 3; ++x) {
                            if ((int) imgBorder.at<uchar>(i+y, j+x) == 255) {
                                voisinFort = true;
                            }
                        }
                    }
    
                    if (voisinFort) {
                        res.at<uchar>(i,j) = saturate_cast<uchar>(255);
                    }
                    else if (step == steps - 1)
                        res.at<uchar>(i,j) = saturate_cast<uchar>(0);
                }
            }
        }
    }

    return res;
}
