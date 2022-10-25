#include "image.hpp"
#include <glm/common.hpp>

using namespace rtc;

Image::Image() : w_(0), h_(0), renderer_(nullptr)
{
}

void Image::Init(const int w, const int h, SDL_Renderer *renderer)
{
    w_ = w;
    h_ = h;
    renderer_ = renderer;
    pixels_.resize(w * h * sizeof(uint32_t));
    texture_ = SDL_CreateTexture(renderer_,
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
                                 SDL_PIXELFORMAT_RGBA8888,
#else
                                 SDL_PIXELFORMAT_ABGR8888,
#endif
                                 SDL_TEXTUREACCESS_TARGET, w, h);
}

Image::~Image()
{
    if (texture_)
    {
        SDL_DestroyTexture(texture_);
    }
}

// See https://stackoverflow.com/questions/33304351/sdl2-fast-pixel-manipulation
void Image::Display()
{
    SDL_UpdateTexture(texture_, nullptr, pixels_.data(), w_ * sizeof(uint32_t));

    SDL_RenderCopy(renderer_, texture_, nullptr, nullptr);
    SDL_RenderPresent(renderer_);
}

// Converts floating point rgb values in [0, 1] to uint32 rgba/abgr number representation
uint32_t PixelColor(double red, double green, double blue, double alpha)
{
    red = glm::clamp(red, 0.0, 0.999) * 256;
    green = glm::clamp(green, 0.0, 0.999) * 256;
    blue = glm::clamp(blue, 0.0, 0.999) * 256;
    alpha = glm::clamp(alpha, 0.0, 0.999) * 256;

    unsigned char r = static_cast<unsigned char>(red);
    unsigned char g = static_cast<unsigned char>(green);
    unsigned char b = static_cast<unsigned char>(blue);
    unsigned char a = static_cast<unsigned char>(alpha);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    uint32_t pixelColor = (r << 24) + (g << 16) + (b << 8) + a;
#else
    uint32_t pixelColor = (a << 24) + (b << 16) + (g << 8) + r;
#endif

    return pixelColor;
}

void Image::SetPixel(const int i, const int j, const double red, const double green, const double blue,
                     const double alpha)
{
    pixels_[i * w_ + j] = PixelColor(red, green, blue, alpha);
}

void Image::SetPixel(const int i, const int j, const color &color)
{
    pixels_[i * w_ + j] = PixelColor(color[0], color[1], color[2], color[3]);
}

int Image::w() const
{
    return w_;
}

int Image::h() const
{
    return h_;
}
