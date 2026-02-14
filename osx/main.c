#include "../vendor/sokol_app.h"
#include "../vendor/sokol_gfx.h"
#include "../vendor/sokol_log.h"
#include "sokol_glue.h"

void init(void);
void frame(void);
void cleanup(void);
void event(const sapp_event *e);
sg_swapchain get_sokol_swapchain(void) { return sglue_swapchain(); }

sapp_desc sokol_main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  return (sapp_desc){
      .init_cb = init,
      .frame_cb = frame,
      .cleanup_cb = cleanup,
      .event_cb = event,
      .width = 640,
      .height = 480,
      .window_title = "Triangle",
      .icon.sokol_default = true,
      .logger.func = slog_func,
  };
}
