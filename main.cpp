#include "app.hpp"
#include "lib/base.hpp"

static struct argp_option options[] = {
    {"samples", 's', "NUM_SAMPLES", 0, "Number of samples per pixel"},
    {"maxdepth", 'm', "MAX_DEPTH", 0, "The maximum recursion depth when bouncing rays"},
    {"res", 'r', "RESOLUTION", 0, "Initial screen resolution, e.g. 1280x720"},
    {0},
};

struct Arguments
{
    int w, h;
    int samples;
    int max_depth;
};

std::regex resolution_rgx("(\\d+)x(\\d+)"); // matches stuff like 1280x720
std::cmatch matches;

static error_t ParseOpt(int key, char *arg, struct argp_state *state)
{
    struct Arguments *arguments = (struct Arguments *)state->input;
    switch (key)
    {
    case 's':
        arguments->samples = atoi(arg);
        break;
    case 'm':
        arguments->max_depth = atoi(arg);
        break;
    case 'r':
        if (!std::regex_search(arg, matches, resolution_rgx))
            return -1;
        arguments->w = stoi(matches[1]);
        arguments->h = stoi(matches[2]);
        break;
    case ARGP_KEY_ARG:
        break;
    case ARGP_KEY_END:
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = {options, ParseOpt, "", "Another Peter Shirley Ray Tracer clone"};

int main(int argc, char *argv[])
{
    struct Arguments args;

    // defaults
    args.samples = SAMPLES_PER_PIXEL_DEFAULT;
    args.max_depth = MAX_DEPTH_DEFAULT;
    args.w = RES_W_DEFAULT;
    args.h = RES_H_DEFAULT;

    argp_parse(&argp, argc, argv, 0, 0, &args);

    App app(args.w, args.h, args.samples, args.max_depth);
    return app.Spin();
}