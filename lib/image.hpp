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

    void Init(int w, int h, SDL_Renderer *renderer);

    void SetPixel(int x, int y, double red, double blue, double green, double alpha);
    void SetPixel(int x, int y, const color &color);

    // send image data to SDL texture
    void Display();

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