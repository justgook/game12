#include <stddef.h>
#include <stdint.h>

#define SOKOL_GLES3
#define SOKOL_EXTERNAL_GL_LOADER

// Include GL types before sokol_gfx.h when using external loader
#include <GLES3/gl3.h>
#include <gl_funcs.h>

#include "../vendor/sokol_app.h"

#define SOKOL_IMPL
#include "../vendor/sokol_gfx.h"

// App functions from main.c
void init(void);
void frame(void);
void cleanup(void);
void event(const sapp_event *e);

// Custom minimal logger for freestanding WASM (no sokol_log.h)
void slog_func(const char *tag, uint32_t log_level, uint32_t log_item,
               const char *message, uint32_t line_nr, const char *filename,
               void *user_data) {
  // In freestanding WASM, we can't easily log to console
  (void)tag;
  (void)log_level;
  (void)log_item;
  (void)message;
  (void)line_nr;
  (void)filename;
  (void)user_data;
}

// JS imports
__attribute__((import_module("env"), import_name("js_canvas_width"))) int
js_canvas_width(void);

__attribute__((import_module("env"), import_name("js_canvas_height"))) int
js_canvas_height(void);

__attribute__((import_module("env"),
               import_name("js_webgl_framebuffer"))) unsigned int
js_webgl_framebuffer(void);

// Supply swapchain for main.c
sg_swapchain get_sokol_swapchain(void) {
  sg_swapchain sc = {0};
  sc.width = js_canvas_width();
  sc.height = js_canvas_height();
  sc.color_format = SG_PIXELFORMAT_RGBA8;
  sc.depth_format = SG_PIXELFORMAT_DEPTH_STENCIL;
  sc.sample_count = 1;
  sc.gl.framebuffer = js_webgl_framebuffer();
  return sc;
}

// Platform init - calls app init, exports as "init" to JS
__attribute__((visibility("default"), export_name("init"))) void
platform_init(void) {
  sg_desc desc = {0};
  desc.logger.func = slog_func;
  sg_setup(&desc);
  init(); // App init from main.c
}

// Platform frame - calls app frame, exports as "frame" to JS
__attribute__((visibility("default"), export_name("frame"))) void
platform_frame(void) {
  frame(); // App frame from main.c
}

// Platform cleanup - calls app cleanup, exports as "cleanup" to JS
__attribute__((visibility("default"), export_name("cleanup"))) void
platform_cleanup(void) {
  cleanup(); // App cleanup from main.c
}
