#include "app.hpp"
#include "lib/base.hpp"
#include "lib/scene.hpp"

// put some reasonable defaults
App::App() : set_up_scene_(rtc::SetUpBlankScene)
{
    // let the SDL2 stuff be initialized through OnInit
}

App::App(int w, int h, int samples_per_pixel, int max_depth, bool once)
    : opts_{w, h, samples_per_pixel, max_depth, VFOV_DEFAULT, once}, set_up_scene_(rtc::SetUpBlankScene)
{
    // let the SDL2 stuff be initialized through OnInit
}

int App::Spin(std::function<void(rtc::Scene &)> set_up_scene)
{
    set_up_scene_ = set_up_scene;
    if (!OnInit())
        return -1;

    SDL_Event event;
    while (is_running_)
    {
        while (SDL_PollEvent(&event))
            OnEvent(event);

        OnLoop();
        OnRender();
    }

    OnExit();
    return 0;
}

int App::Once(std::function<void(rtc::Scene &)> set_up_scene, std::function<void(const rtc::Image &)> cb)
{
    set_up_scene_ = set_up_scene;
    if (!OnInit())
        return -1;

    scene_.Render(image_);
    cb(image_);

    return 0;
}

bool App::OnInit()
{
    if (!opts_.once && !InitSDL())
        return false;

    image_.Init(opts_.w, opts_.h, renderer_);

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

    // TODO: make resizable
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

    return true;
}

void App::OnEvent(const SDL_Event &event)
{
    if (event.type == SDL_QUIT)
        is_running_ = false;
}

void App::OnLoop()
{
}

void App::OnRender()
{
    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
    SDL_RenderClear(renderer_);

    scene_.Render(image_);
    image_.Display();
}

void App::OnExit()
{
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);

    window_ = nullptr;

    SDL_Quit();
}
