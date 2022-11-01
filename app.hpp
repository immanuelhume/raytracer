#ifndef APP_H
#define APP_H

#include "lib/image.hpp"
#include "lib/scene.hpp"

struct ConfigOpts
{
    int w, h;
    int samples_per_pixel;
    int max_depth;
    // verticle field of vision, in radians
    double vfov;
};

class App
{
  public:
    App();
    App(int w, int h, int samples_per_pixel, int max_depth);

    // starts the app
    int Spin(std::function<void(rtc::Scene &)>);

  private:
    // initializes SDL2 and other resources and returns whether it was successful
    bool OnInit();
    void OnLoop();
    void OnEvent(const SDL_Event &event);
    void OnRender();
    void OnExit();

  public:
    std::function<void(rtc::Scene &)> set_up_scene_;

  private:
    ConfigOpts opts_;
    bool is_running_;

    SDL_Window *window_;
    SDL_Renderer *renderer_;

    rtc::Image image_;
    rtc::Scene scene_;
};

#endif