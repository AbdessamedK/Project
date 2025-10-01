#pragma once

#include <functional>
#include <getfem/bgeot_small_vector.h>

typedef LCC::Point Point;


template<typename T, unsigned int dim>
struct point {
};

template<typename T>
struct point<T, 2> {
    point() : x(0), y(0) {
    }

    point(T _x, T _y) : x(_x), y(_y) {
    }

    operator Point() const {
        return Point(x, y, 0.);
    }

    point &operator=(const bgeot::small_vector<double> &sv) {
        x = sv[0];
        y = sv[1];
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const point &p) {
        os << "(" << p.x << ", " << p.y << ")";
        return os;
    }

    bool operator==(const point &p) const {
        return x == p.x && y == p.y;
    }

    point &operator+=(const point &p) {
        x += p.x;
        y += p.y;
        return *this;
    }

public:
    T x, y;
};

template<typename T>
struct point<T, 3> {
    point() : x(0), y(0), z(0) {
    }

    point(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {
    }

    operator Point() const {
        return Point(x, y, z);
    }

    point &operator=(const bgeot::small_vector<double> &sv) {
        x = sv[0];
        y = sv[1];
        z = sv[2];
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const point &p) {
        os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
        return os;
    }

    bool operator==(const point &p) const {
        return x == p.x && y == p.y && z == p.z;
    }

    point &operator+=(const point &p) {
        x += p.x;
        y += p.y;
        z += p.z;
        return *this;
    }

public :
    T x, y, z;
};

using point2 = point<double, 2>;
using point3 = point<double, 3>;

template<typename T>
void hash_combine(std::size_t &seed, const T &value) {
    std::hash<T> hasher;
    seed ^= hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template<typename T>
struct std::hash<point<T, 2> > {
    std::size_t operator()(const point<T, 2> &p) const {
        std::size_t seed = 0;
        hash_combine(seed, p.x);
        hash_combine(seed, p.y);
        return seed;
    }
};

template<typename T>
struct std::hash<point<T, 3> > {
    std::size_t operator()(const point<T, 3> &p) const {
        std::size_t seed = 0;
        hash_combine(seed, p.x);
        hash_combine(seed, p.y);
        hash_combine(seed, p.z);
        return seed;
    }
};
