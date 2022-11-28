#include "app.hpp"
#include "base.hpp"
#include "scene.hpp"
#include <iomanip>

// command line options
static struct argp_option options[] = {
    {"samples", 's', "NUM_SAMPLES", 0, "Number of samples per pixel"},
    {"maxdepth", 'd', "MAX_DEPTH", 0, "The maximum recursion depth when bouncing rays"},
    {"res", 'r', "RESOLUTION", 0, "Initial screen resolution, e.g. 1280x720"},
    {"file", 'f', "FILENAME", 0, "Save to a PNG file instead of viewing it in a window"},
    {"listscenes", 'l', 0, 0, "List available scenes"},
    {"scene", 'c', "SCENE_NUM", 0, "Select a scene to render"},
    {"numthreads", 't', "NUM_THREADS", 0, "Number of threads to use, defaults to 1"},
    {0}, // end of array
};

struct Arguments
{
    int w = RES_W_DEFAULT, h = RES_H_DEFAULT;
    int samples = SAMPLES_PER_PIXEL_DEFAULT;
    int max_depth = MAX_DEPTH_DEFAULT;
    char *filename = nullptr;
    rtc::SceneDemo scene = rtc::SceneDemo::kNoise;
    bool list_only = false; // list available scenes
    int num_threads = 1;
};

static std::regex resolution_rgx("(\\d+)x(\\d+)"); // matches stuff like 1280x720
static std::cmatch matches;

static error_t ParseOpt(int key, char *arg, struct argp_state *state)
{
    struct Arguments *args = (struct Arguments *)state->input;
    int x;
    switch (key)
    {
    case 's': args->samples = atoi(arg); break;
    case 'd': args->max_depth = atoi(arg); break;
    case 'r':
        if (!std::regex_search(arg, matches, resolution_rgx)) return -1;
        args->w = stoi(matches[1]);
        args->h = stoi(matches[2]);
        break;
    case 'f': args->filename = arg; break;
    case 'l': args->list_only = true; break;
    case 'c':
        // Scene selection. If it exceeds the available numbers throw an error.
        x = atoi(arg);
        if (x >= rtc::SceneDemo::kLast)
        {
            std::stringstream err_msg;
            err_msg << "Scene " << x << " is invalid";
            argp_error(state, "%s", err_msg.str().c_str());
            return 1;
        }
        args->scene = (rtc::SceneDemo)x;
        break;
    case 't':
        x = atoi(arg);
        if (x > (int)std::thread::hardware_concurrency())
        {
            std::stringstream err_msg;
            err_msg << x << " threads wanted but only " << std::thread::hardware_concurrency() << " available";
            argp_error(state, "%s", err_msg.str().c_str());
            return 1;
        }
        args->num_threads = x;
        break;
    case ARGP_KEY_ARG: break;
    case ARGP_KEY_END: break;
    default: return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = {options, ParseOpt, "", "Another Peter Shirley Ray Tracer clone"};

int main(int argc, char *argv[])
{
    struct Arguments args; // create arg struct with defaults
    argp_parse(&argp, argc, argv, 0, 0, &args);

    // print out the scenes, then exit
    if (args.list_only)
    {
        std::cout << "These are the scenes available, identified by their numbers.\n\n";
        for (int i = rtc::SceneDemo::kRandomBalls; i < rtc::SceneDemo::kLast; i++)
        {
            std::cout << std::left << std::setw(3) << i;
            std::cout << rtc::sceneDesc[i] << std::endl;
        }
        std::cout << "\nFor example, to render the perlin noise demo use:\n"
                  << "    rtc -c 3" << std::endl;
        return 0;
    }

    App app(args.w, args.h, args.samples, args.max_depth, args.filename, args.num_threads);

    // if the filename is not null, pass in a callback to save the render to PNG
    // TODO make sure the file exists/can create
    if (args.filename)
        return app.Once(rtc::scenes[args.scene], [&args](const rtc::Image &i) { i.SavePNG(args.filename); });

    return app.Spin(rtc::scenes[args.scene]);
}