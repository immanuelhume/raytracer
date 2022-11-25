#include "perlin.hpp"
#include <glm/gtc/random.hpp>

using namespace rtc;

Perlin::Perlin()
{
    // TODO switch to random vector table instead of sphericalRand
    for (size_t i = 0; i < n_; i++) rand_vecs_[i] = glm::sphericalRand(1.0);

    FillAndShuffle(perm_x_);
    FillAndShuffle(perm_y_);
    FillAndShuffle(perm_z_);
}

double Perlin::GetNoise(const point &p) const
{
    double u = p.x - std::floor(p.x);
    double v = p.y - std::floor(p.y);
    double w = p.z - std::floor(p.z);

    int i = static_cast<int>(floor(p.x));
    int j = static_cast<int>(floor(p.y));
    int k = static_cast<int>(floor(p.z));

    vec3 c[2][2][2];

    for (int di = 0; di < 2; di++)
        for (int dj = 0; dj < 2; dj++)
            for (int dk = 0; dk < 2; dk++)
                c[di][dj][dk] = rand_vecs_[perm_x_[(i + di) & 255] ^ perm_y_[(j + dj) & 255] ^ perm_z_[(k + dk) & 255]];

    return TrilinearInterpolate(c, u, v, w);
}

double Perlin::GetTurbulence(point p, int depth) const
{
    double accum = 0.0;
    double weight = 1.0;

    for (int i = 0; i < depth; i++)
    {
        accum += weight * GetNoise(p);
        weight *= 0.5;
        p *= 2.0;
    }

    return std::fabs(accum);
}

double Perlin::TrilinearInterpolate(vec3 c[2][2][2], double u, double v, double w)
{
    double uu = u * u * (3 - 2 * u);
    double vv = v * v * (3 - 2 * v);
    double ww = w * w * (3 - 2 * w);

    double accum = 0.0;

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++)
            {
                vec3 weight(u - i, v - j, w - k);
                accum += (i * uu + (1 - i) * (1 - uu)) * (j * vv + (1 - j) * (1 - vv)) * (k * ww + (1 - k) * (1 - ww)) *
                         glm::dot(c[i][j][k], weight);
            }

    return accum;
}

template <size_t N> void Perlin::FillAndShuffle(std::array<int, N> &xs)
{
    std::iota(xs.begin(), xs.end(), 0);
    std::shuffle(xs.begin(), xs.end(), rng);
}
