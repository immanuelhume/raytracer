#ifndef APP_H
#define APP_H

#include "image.hpp"
#include "scene.hpp"

struct ConfigOpts
{
    int w, h;
    int samples_per_pixel;
    int max_depth;
    // verticle field of vision, in radians
    double vfov;
    // whether to just run once and exit
    bool once;
};

class App
{
  public:
    App();
    App(int w, int h, int samples_per_pixel, int max_depth, bool once);

    // starts the app
    int Spin(std::function<void(rtc::Scene &)>);

    // just renders one frame, then executes some callback on the image
    int Once(std::function<void(rtc::Scene &)>, std::function<void(const rtc::Image &)>);

  private:
    bool OnInit();
    // initializes SDL2 stuff, including the window and renderer
    bool InitSDL();
    void OnLoop();
    void OnEvent(const SDL_Event &event);
    void OnRender();
    // deletes SDL2 resources and other stuff
    void OnExit();

  public:
    std::function<void(rtc::Scene &)> set_up_scene_;

  private:
    ConfigOpts opts_{RES_W_DEFAULT, RES_H_DEFAULT, SAMPLES_PER_PIXEL_DEFAULT, MAX_DEPTH_DEFAULT, VFOV_DEFAULT, false};
    bool is_running_ = true;

    SDL_Window *window_ = nullptr;
    SDL_Renderer *renderer_ = nullptr;
    SDL_Texture *texture_ = nullptr;

    rtc::Image image_;
    rtc::Scene scene_;
};

#endif