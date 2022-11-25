#ifndef APP_H
#define APP_H

#include "image.hpp"
#include "scene.hpp"

// config options, more or less aligned with those received from CLI options
struct ConfigOpts
{
    int w = RES_W_DEFAULT, h = RES_H_DEFAULT;
    int samples_per_pixel = SAMPLES_PER_PIXEL_DEFAULT;
    int max_depth = MAX_DEPTH_DEFAULT;
    double vfov = VFOV_DEFAULT; // verticle field of vision, in radians
    bool once = false;          // whether to just run once and exit
    int num_threads = 1;        // render in parallel?
};

class App
{
  public:
    App() = default;
    App(int w, int h, int samples_per_pixel, int max_depth, bool once, int num_threads);

    // starts the app
    int Spin(std::function<void(rtc::Scene &)>);

    // just renders one frame, then executes some callback on the completed image, so no SDL window will be created
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
    // a lambda to set up the scene
    std::function<void(rtc::Scene &)> set_up_scene_ = rtc::BlankScene;

  private:
    ConfigOpts opts_;
    bool is_running_ = true;

    SDL_Window *window_ = nullptr;
    SDL_Renderer *renderer_ = nullptr;
    SDL_Texture *texture_ = nullptr;

    rtc::Image image_;
    rtc::Scene scene_;
};

#endif