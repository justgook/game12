#include <stddef.h>
#include <stdint.h>

// #define SOKOL_DUMMY_BACKEND
#define SOKOL_GLES3
#define SOKOL_EXTERNAL_GL_LOADER
// #define SOKOL_WGPU

// Include GL types before sokol_gfx.h when using external loader
#include <GLES3/gl3.h>
#include <gl_funcs.h>

#define SOKOL_IMPL
#include "sokol_gfx.h"

// Custom minimal logger for freestanding WASM (no sokol_log.h)
#include <stdlib.h>
void slog_func(const char* tag, uint32_t log_level, uint32_t log_item, const char* message, uint32_t line_nr, const char* filename, void* user_data) {
    // In freestanding WASM, we can't easily log to console
    // This is a no-op stub - you could import a JS function to log if needed
    (void)tag;
    (void)log_level;
    (void)log_item;
    (void)message;
    (void)line_nr;
    (void)filename;
    (void)user_data;
}

/// THE NEW STUFF //
// JS imports
__attribute__((import_module("env"), import_name("js_canvas_width"))) int
js_canvas_width(void);

__attribute__((import_module("env"), import_name("js_canvas_height"))) int
js_canvas_height(void);

__attribute__((import_module("env"),
               import_name("js_webgl_framebuffer"))) unsigned int
js_webgl_framebuffer(void);

// must define the logger
// void slog_func(const char *tag, const char *msg) {
//   // optional: forward to JS console
// }

// supply an environment for sokol_gfx
sg_environment get_sokol_environment(void) {
  sg_environment env = {0};
  // For GL backend, environment doesn't need special setup
  // Backend is determined by compile-time defines (SOKOL_GLES3)
  return env;
}

/// THE NEW STUFF ENDS //

static sg_pass_action pass_action;

// called once from JS
__attribute__((visibility("default"))) void init(void) {
  sg_desc desc = {0};
  desc.logger.func = slog_func;

  // provide environment descriptor via your host
  extern sg_environment get_sokol_environment(void);
  desc.environment = get_sokol_environment();

  sg_setup(&desc);

  // configure clear values
  pass_action.colors[0].load_action = SG_LOADACTION_CLEAR;
  pass_action.colors[0].clear_value = (sg_color){0.2f, 0.4f, 0.6f, 1.0f};
  pass_action.depth.load_action = SG_LOADACTION_CLEAR;
  pass_action.depth.clear_value = 1.0f;
}

// imported from JS
extern int js_canvas_width(void);
extern int js_canvas_height(void);
extern unsigned int js_webgl_framebuffer(void);

// build and return a swapchain struct
sg_swapchain get_sokol_swapchain(void) {
  sg_swapchain sc = {0};
  sc.width = js_canvas_width();
  sc.height = js_canvas_height();
  sc.color_format = SG_PIXELFORMAT_RGBA8;         // typical WebGL2 default
  sc.depth_format = SG_PIXELFORMAT_DEPTH_STENCIL; // common depth/stencil
  sc.sample_count = 1;
  sc.gl.framebuffer = js_webgl_framebuffer(); // JS must return 0 here
  return sc;
}
__attribute__((visibility("default"))) void frame(void) {
  // begin a swapchain pass, using a helper from your platform code
  sg_begin_pass(&(sg_pass){
      .action = pass_action,
      .swapchain = get_sokol_swapchain(),
  });
  sg_end_pass();
  sg_commit();

  // TODO: draw more here...

  sg_end_pass();
  sg_commit();
}

__attribute__((visibility("default"))) void cleanup(void) { sg_shutdown(); }
