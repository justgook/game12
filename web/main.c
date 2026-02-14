#include <stddef.h>
#include <stdint.h>

#define SOKOL_GLES3
#define SOKOL_EXTERNAL_GL_LOADER

// Include GL types before sokol_gfx.h when using external loader
#include <GLES3/gl3.h>
#include <gl_funcs.h>

#define SOKOL_IMPL
#include "sokol_gfx.h"
#include "triangle-sapp.glsl.h"

// Custom minimal logger for freestanding WASM (no sokol_log.h)
#include <stdlib.h>
void slog_func(const char *tag, uint32_t log_level, uint32_t log_item,
               const char *message, uint32_t line_nr, const char *filename,
               void *user_data) {
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

// JS imports
__attribute__((import_module("env"), import_name("js_canvas_width"))) int
js_canvas_width(void);

__attribute__((import_module("env"), import_name("js_canvas_height"))) int
js_canvas_height(void);

__attribute__((import_module("env"),
               import_name("js_webgl_framebuffer"))) unsigned int
js_webgl_framebuffer(void);

// Triangle rendering state
static struct {
  sg_pipeline pip;
  sg_bindings bind;
  sg_pass_action pass_action;
} state;

// supply an environment for sokol_gfx
sg_environment get_sokol_environment(void) {
  sg_environment env = {0};
  // For GL backend, environment doesn't need special setup
  // Backend is determined by compile-time defines (SOKOL_GLES3)
  return env;
}

// called once from JS
__attribute__((visibility("default"))) void init(void) {
  sg_desc desc = {0};
  desc.logger.func = slog_func;

  // provide environment descriptor via your host
  extern sg_environment get_sokol_environment(void);
  desc.environment = get_sokol_environment();

  sg_setup(&desc);

  // Create vertex buffer with triangle data
  // Each vertex: position (3 floats) + color (4 floats) = 7 floats
  float vertices[] = {
      // position (xyz)      color (rgba)
      0.0f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, // top - red
      0.5f,  -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, // bottom right - green
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, // bottom left - blue
  };

  state.bind.vertex_buffers[0] = sg_make_buffer(&(sg_buffer_desc){
      .data = {.ptr = vertices, .size = sizeof(vertices)},
  });

  // Create pipeline with shader
  state.pip = sg_make_pipeline(&(sg_pipeline_desc){
      .shader = sg_make_shader(triangle_shader_desc(sg_query_backend())),
      .layout = {.attrs = {[ATTR_triangle_position].format =
                               SG_VERTEXFORMAT_FLOAT3,
                           [ATTR_triangle_color0].format =
                               SG_VERTEXFORMAT_FLOAT4}},
  });

  // Configure pass action (clear to black)
  state.pass_action =
      (sg_pass_action){.colors[0] = {.load_action = SG_LOADACTION_CLEAR,
                                     .clear_value = {0.0f, 0.0f, 0.0f, 1.0f}}};
}

// build and return a swapchain struct
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

__attribute__((visibility("default"))) void frame(void) {
  // begin a swapchain pass
  sg_begin_pass(&(sg_pass){
      .action = state.pass_action,
      .swapchain = get_sokol_swapchain(),
  });

  // Apply pipeline and bindings
  sg_apply_pipeline(state.pip);
  sg_apply_bindings(&state.bind);

  // Draw the triangle (3 vertices, 1 instance)
  sg_draw(0, 3, 1);

  sg_end_pass();
  sg_commit();
}

__attribute__((visibility("default"))) void cleanup(void) { sg_shutdown(); }
