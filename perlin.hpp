#ifndef PERLIN_H
#define PERLIN_H

#include "base.hpp"

namespace rtc
{

class Perlin
{
  public:
    Perlin();
    double Noise(const point &p) const;
    double Turbulence(point p, int depth = 7) const;

  private:
    // fills an array with natural numbers, then shuffles them
    template <size_t N> static void FillAndShuffle(std::array<int, N> &xs);
    static double Lerp(vec3 c[2][2][2], double u, double v, double w);

  private:
    // The noise generated will repeat itself in 256x256x256 blocks. The number is chosen because we can efficiently
    // compute modulos using bitwise AND instead of actual modulo (%).
    static const size_t n_ = 256;

    std::array<vec3, n_> rand_vecs_;
    std::array<int, n_> perm_x_;
    std::array<int, n_> perm_y_;
    std::array<int, n_> perm_z_;
};

} // namespace rtc

#endif