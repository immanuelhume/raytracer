#ifndef IMAGE_H
#define IMAGE_H

#include "base.hpp"

namespace rtc
{

// wrapper around an SDL texture
class Image
{
  public:
    Image();
    ~Image();

    // initialize the image without an SDL renderer
    void Init(int w, int h);
    void Init(int w, int h, SDL_Renderer *renderer);

    void SetPixel(int x, int y, double red, double blue, double green, double alpha);
    void SetPixel(int x, int y, const color &color);

    // send image data to SDL texture
    void Display();

    // save the current buffer as a PNG file, not thread safe!
    bool SavePNG(const std::string &filename) const;

    int w() const;
    int h() const;

  private:
    std::vector<uint32_t> pixels_;
    int w_;
    int h_;

    SDL_Renderer *renderer_;
    SDL_Texture *texture_;
};

} // namespace rtc

#endif