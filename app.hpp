#ifndef APP_H
#define APP_H

#include "lib/image.hpp"
#include "lib/scene.hpp"
#include <SDL2/SDL.h>

class App
{
  public:
    App();

    int Spin();

  private:
    // initializes SDL2 and other resources and returns whether it was successful
    bool OnInit();
    void OnLoop();
    void OnEvent(const SDL_Event &event);
    void OnRender();
    void OnExit();

  private:
    bool is_running_;
    SDL_Window *window_;
    SDL_Renderer *renderer_;

    rtc::Image image_;
    rtc::Scene scene_;
};

#endif