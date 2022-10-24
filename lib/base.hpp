#ifndef BASE_H
#define BASE_H

#include <cstdlib>
#include <glm/glm.hpp>
#include <iostream>
#include <limits>

using color = glm::dvec4;
using point = glm::dvec3;
using vec3 = glm::dvec3;

const double epsilon = 1e-8;
const double infinity = std::numeric_limits<double>::max();

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