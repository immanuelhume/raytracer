#ifndef APP_H
#define APP_H

#include "lib/image.hpp"
#include "lib/scene.hpp"
#include <SDL2/SDL.h>

class App
{
  public:
    App();
    App(int w, int h, int samples_per_pixel, int max_depth);

    // starts the app
    int Spin();

  private:
    // initializes SDL2 and other resources and returns whether it was successful
    bool OnInit();
    void OnLoop();
    void OnEvent(const SDL_Event &event);
    void OnRender();
    void OnExit();

  private:
    int w_, h_;
    int samples_per_pixel_;
    int max_depth_;
    bool is_running_;

    SDL_Window *window_;
    SDL_Renderer *renderer_;

    rtc::Image image_;
    rtc::Scene scene_;
};

#endif