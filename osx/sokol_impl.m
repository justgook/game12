/*
    sokol_impl.m

    When using the Metal backend, the implementation source must be
    Objective-C (.m or .mm), but we want the samples to be in C. Thus
    move the sokol implementation into its own .m file.
*/
#define SOKOL_IMPL
#define SOKOL_METAL
#include "../vendor/sokol_app.h"
#include "../vendor/sokol_gfx.h"
#include "../vendor/sokol_log.h"
#include "sokol_glue.h"
