#include "app.hpp"
#include "base.hpp"
#include "scene.hpp"

App::App(int w, int h, int samples_per_pixel, int max_depth, bool once, int num_threads)
    : set_up_scene_(rtc::BlankScene), opts_{w, h, samples_per_pixel, max_depth, VFOV_DEFAULT, once, num_threads},
      image_(w, h), scene_(num_threads)
{
    // let the SDL2 stuff be initialized through OnInit
}

int App::Spin(std::function<void(rtc::Scene &)> set_up_scene)
{
    set_up_scene_ = set_up_scene;
    if (!OnInit()) return -1;

    SDL_Event event;
    while (is_running_)
    {
        while (SDL_PollEvent(&event)) OnEvent(event);
        OnLoop();
        OnRender();
    }

    OnExit();
    return 0;
}

int App::Once(std::function<void(rtc::Scene &)> set_up_scene, std::function<void(const rtc::Image &)> cb)
{
    set_up_scene_ = set_up_scene;
    if (!OnInit()) return -1;

    scene_.Render(image_);
    cb(image_);

    return 0;
}

bool App::OnInit()
{
    if (!opts_.once && !InitSDL()) return false;

    scene_.samples_per_pixel_ = opts_.samples_per_pixel;
    scene_.max_depth_ = opts_.max_depth;

    set_up_scene_(scene_);

    return true;
}

bool App::InitSDL()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        fprintf(stderr, "while initializing SDL: %s", SDL_GetError());
        return false;
    }

    window_ =
        SDL_CreateWindow("Yet Another Ray Tracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, opts_.w, opts_.h, 0);

    if (!window_)
    {
        fprintf(stderr, "while creating SDL window: %s", SDL_GetError());
        return false;
    }
    renderer_ = SDL_CreateRenderer(window_, -1, 0);
    if (!renderer_)
    {
        fprintf(stderr, "while creating SDL renderer: %s", SDL_GetError());
        return false;
    }

    texture_ = SDL_CreateTexture(renderer_,
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
                                 SDL_PIXELFORMAT_RGBA8888,
#else
                                 SDL_PIXELFORMAT_ABGR8888,
#endif
                                 SDL_TEXTUREACCESS_TARGET, opts_.w, opts_.h);
    if (!texture_)
    {
        fprintf(stderr, "while creating SDL texture: %s", SDL_GetError());
        return false;
    }

    return true;
}

void App::OnEvent(const SDL_Event &event)
{
    if (event.type == SDL_QUIT) is_running_ = false;
}

void App::OnLoop() {}

void App::OnRender()
{
    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
    SDL_RenderClear(renderer_);

    scene_.Render(image_);
    image_.Display(texture_, renderer_);
}

void App::OnExit()
{
    SDL_DestroyTexture(texture_);
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);

    window_ = nullptr;

    SDL_Quit();
}
