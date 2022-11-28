#include "image.hpp"
#include <SDL2/SDL_image.h>
#include <glm/common.hpp>

using namespace rtc;

Image::Image(int w, int h) : w_(w), h_(h) { pixels_.resize(w * h); }

Image::~Image() {}

// See https://stackoverflow.com/questions/33304351/sdl2-fast-pixel-manipulation
void Image::Display(SDL_Texture *texture, SDL_Renderer *renderer)
{
    SDL_UpdateTexture(texture, nullptr, pixels_.data(), w_ * sizeof(uint32_t));

    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

bool Image::SavePNG(const std::string &filename) const
{
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormatFrom((void *)pixels_.data(), w_, h_, 32, w_ * sizeof(uint32_t),
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
                                                              SDL_PIXELFORMAT_RGBA8888,
#else
                                                              SDL_PIXELFORMAT_ABGR8888
#endif
    );
    if (!surface)
    {
        std::cerr << "failed to create SDL_Surface: " << SDL_GetError() << std::endl;
        return false;
    }
    if (IMG_SavePNG(surface, filename.c_str()) == -1)
    {
        std::cerr << "failed to write to PNG: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return false;
    }
    SDL_FreeSurface(surface);
    return true;
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

int Image::w() const { return w_; }

int Image::h() const { return h_; }
