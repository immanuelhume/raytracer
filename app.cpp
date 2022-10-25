#include "app.hpp"
#include "lib/base.hpp"

// put some reasonable defaults
App::App()
    : is_running_(true), opts_{RES_W_DEFAULT, RES_H_DEFAULT, SAMPLES_PER_PIXEL_DEFAULT, MAX_DEPTH_DEFAULT, VFOV_DEFAULT}
{
    // let the SDL2 stuff be initialized through OnInit
}

App::App(int w, int h, int samples_per_pixel, int max_depth)
    : is_running_(true), opts_{w, h, samples_per_pixel, max_depth, VFOV_DEFAULT}
{
    // let the SDL2 stuff be initialized through OnInit
}

int App::Spin()
{
    if (!OnInit())
    {
        return -1;
    }

    SDL_Event event;
    while (is_running_)
    {
        while (SDL_PollEvent(&event))
        {
            OnEvent(event);
        }

        OnLoop();
        OnRender();
    }

    OnExit();
    return 0;
}

bool App::OnInit()
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

    image_.Init(opts_.w, opts_.h, renderer_);
    scene_.samples_per_pixel_ = opts_.samples_per_pixel;
    scene_.max_depth_ = opts_.max_depth;

    return true;
}

void App::OnEvent(const SDL_Event &event)
{
    if (event.type == SDL_QUIT)
    {
        is_running_ = false;
    }
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
