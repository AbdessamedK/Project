#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <opencv2/opencv.hpp>

class Interface {
public:
    virtual void onMouse(int event, int x, int y) = 0;
    virtual void show(int deg) = 0;
    virtual ~Interface() {}
};

#endif // INTERFACE_HPP
