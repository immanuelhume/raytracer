#ifndef IMAGE_H
#define IMAGE_H

#include "base.hpp"

namespace rtc
{

// Just a wrapper around a buffer of pixels.
class Image
{
  public:
    Image() = default;
    Image(int w, int h);
    ~Image();

    void SetPixel(int x, int y, double red, double blue, double green, double alpha);
    void SetPixel(int x, int y, const rgb &color);

    // send image data to SDL texture
    void Display(SDL_Texture *, SDL_Renderer *);

    // save the current buffer as a PNG file, not thread safe!
    bool SavePNG(const std::string &filename) const;

    int w() const;
    int h() const;

  private:
    std::vector<uint32_t> pixels_;
    int w_ = RES_W_DEFAULT;
    int h_ = RES_H_DEFAULT;
};

// Converts floating point rgba values in [0, 1] to uint32 rgba/abgr number representation
uint32_t PixelColor(double red, double green, double blue, double alpha);

} // namespace rtc

#endif