#include "texture.hpp"
#include "image.hpp"
#include <filesystem>

using namespace rtc;

/* ------------------------------- BASIC COLOR ------------------------------ */

SolidColor::SolidColor(const rgb &c) : c_(c) {}
SolidColor::SolidColor(double r, double g, double b) : c_(r, g, b) {}
rgb SolidColor::ValueAt(double u, double v, const point &p) const { return c_; }

/* -------------------------------- CHECKERS -------------------------------- */

Checkers::Checkers(const rgb &a, const rgb &b)
    : a_(std::make_shared<SolidColor>(a)), b_(std::make_shared<SolidColor>(b))
{
}
Checkers::Checkers(std::shared_ptr<Texture> a, std::shared_ptr<Texture> b) : a_(a), b_(b) {}
rgb Checkers::ValueAt(double u, double v, const point &p) const
{
    double sines = glm::sin(10 * p.x) * glm::sin(10 * p.y) * glm::sin(10 * p.z); // big brain stuff
    if (sines < 0) return a_->ValueAt(u, v, p);
    else return b_->ValueAt(u, v, p);
}

/* -------------------------- VANILLA PERLIN NOISE -------------------------- */

Noisy::Noisy(double scale) : scale_(scale) {}
Noisy::Noisy(const rgb &c, double scale) : scale_(scale), c_(c) {}
rgb Noisy::ValueAt(double u, double v, const point &p) const { return c_ * noise_.NoiseX(scale_ * p); }

/* ------------------------------- TURBULENCE ------------------------------- */

Turbulence::Turbulence(double scale) : Noisy(scale) {}
Turbulence::Turbulence(const rgb &c, double scale) : Noisy(c, scale) {}
rgb Turbulence::ValueAt(double u, double v, const point &p) const { return c_ * noise_.Turbulence(scale_ * p); }

/* --------------------------------- MARBLE --------------------------------- */

Marble::Marble(Axis ax, double scale) : Noisy(scale), ax_(ax) {}
Marble::Marble(Axis ax, const rgb &c, double scale) : Noisy(c, scale), ax_(ax) {}
rgb Marble::ValueAt(double u, double v, const point &p) const
{
    return c_ * 0.5 * (1 + glm::sin(scale_ * p[ax_] + 10 * noise_.Turbulence(p)));
}

/* ---------------------------------- WOOD ---------------------------------- */

Wood::Wood(double scale, double f) : Noisy(scale), f_(f) {}
Wood::Wood(const rgb &c, double scale, double f) : Noisy(c, scale), f_(f) {}
rgb Wood::ValueAt(double u, double v, const point &p) const
{
    double g = noise_.NoiseX(scale_ * p) * f_;
    return (g - std::floor(g)) * c_;
}

/* ---------------------------------- IMAGE --------------------------------- */

ImageTexture::ImageTexture(const char *filename)
{
    buf_ = IMG_Load(filename);
    if (!buf_) std::cerr << "error while creating image texture: " << SDL_GetError() << std::endl;
}
ImageTexture::ImageTexture(const unsigned char *filedata, int size)
{
    SDL_RWops *src = SDL_RWFromConstMem(filedata, size);
    buf_ = IMG_Load_RW(src, 0);
}
ImageTexture::~ImageTexture()
{
    if (buf_) SDL_FreeSurface(buf_);
}
rgb ImageTexture::ValueAt(double u, double v, const point &p) const
{
    if (!buf_) return rgb(0, 1, 1); // cyan for debugging

    v = 1 - v; // flip vertically
    int i = static_cast<int>(v * buf_->h);
    int j = static_cast<int>(u * buf_->w);

    i = std::min(buf_->h - 1, i);
    j = std::min(buf_->w - 1, j);

    // At this point (i, j) represents our desired pixel coordinates. A pixel might be represented as 3 or 4 bytes in an
    // image, so we need to compute its location in the pixel buffer via byte offset.

    auto pixels = static_cast<const uint8_t *>(buf_->pixels);
    auto start = pixels + i * buf_->pitch + j * buf_->format->BytesPerPixel;
    uint32_t color = *(const uint32_t *)start;
    uint8_t r, g, b;
    SDL_GetRGB(color, buf_->format, &r, &g, &b);

    return rgb(static_cast<double>(r) / 0xFF, static_cast<double>(g) / 0xFF, static_cast<double>(b) / 0xFF);
}
