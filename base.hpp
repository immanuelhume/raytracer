#ifndef BASE_H
#define BASE_H

#include "pch.hpp"

#define SAMPLES_PER_PIXEL_DEFAULT 4
#define MAX_DEPTH_DEFAULT 50
#define RES_W_DEFAULT 512
#define RES_H_DEFAULT 256
#define VFOV_DEFAULT 0.34906585 // 20 degrees

using color = glm::dvec4;
using point = glm::dvec3;
using vec3 = glm::dvec3;

const double epsilon = 1e-8;
const double infinity = std::numeric_limits<double>::max();

enum Axis
{
    x = 0,
    y,
    z,
};

static std::random_device rd;
static std::mt19937 rng(rd());

// a random floating point number in [0, 1)
inline double rand_double()
{
    return rand() / (RAND_MAX + 1.0);
}

// a random floating point number within some range
inline double rand_double(const double min, const double max)
{
    return min + (max - min) * rand_double();
}

inline color rand_color()
{
    return color(rand_double(0, 1), rand_double(0, 1), rand_double(0, 1), 1);
}

inline color rand_color(double min, double max)
{
    return color(rand_double(min, max), rand_double(min, max), rand_double(min, max), 1);
}

// See https://stackoverflow.com/a/19728404/13317332
inline Axis random_axis()
{
    static std::uniform_int_distribution<int> uni(Axis::x, Axis::z);
    return (Axis)uni(rng);
}

template <glm::length_t n, typename T> inline std::ostream &operator<<(std::ostream &stream, glm::vec<n, T> v)
{
    stream << v[0];
    if (n == 1)
        return stream;

    for (int i = 1; i < n; i++)
        stream << ", " << v[i];

    return stream;
}

template <glm::length_t n, typename T> inline bool almost_zero(const glm::vec<n, T> v)
{
    for (int i = 0; i < n; i++)
    {
        if (fabs(v[i]) > epsilon)
            return false;
    }
    return true;
}

#endif