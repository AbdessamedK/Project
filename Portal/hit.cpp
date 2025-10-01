#include <vec.h>
#include <limits>
#include <array>

struct Hit
{
    float t;
    Vector n;

    Hit(float _t, Vector _n) : t(_t), n(_n) {}
};

bool memeSigne(float a, float b, float c)
{
    return (a >= 0 && b >= 0 && c >= 0) || (a < 0 && b < 0 && c < 0);
}

Hit triangle_hit(const Point &origin, const Vector &direction, const std::array<Point, 3> pts)
{
    Vector n = (cross(pts[1] - pts[0], pts[2] - pts[0]));
    if (dot(direction, n) == 0)
        return Hit(std::numeric_limits<float>::infinity(), Vector());
    float t = dot(pts[0] - origin, n) / dot(direction, n);
    Point p = origin + (t * direction);

    Vector abp = cross(pts[1] - pts[0], p - pts[0]);
    Vector bcp = cross(pts[2] - pts[1], p - pts[1]);
    Vector cap = cross(pts[0] - pts[2], p - pts[2]);

    if (dot(abp, n) < 0 || dot(bcp, n) < 0 || dot(cap, n) < 0 || t < 0)
        return Hit(std::numeric_limits<float>::infinity(), Vector());

    return Hit(t, n);
}