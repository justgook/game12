/*
    main_web.c

    WASM entry point for the triangle demo.
    No sokol_app.h — the JavaScript host manages the application lifecycle.
    Exports: wasm_init, wasm_frame, wasm_cleanup
*/
#include "../sokol_gfx.h"
#include "../triangle-sapp.glsl.h"

/* slog_func is implemented in sokol_impl_web.c */
extern void slog_func(const char* tag, unsigned int log_level, unsigned int log_item, const char* message, unsigned int line_nr, const char* filename, void* user_data);

static struct {
    sg_pipeline pip;
    sg_bindings bind;
    sg_pass_action pass_action;
} state;

__attribute__((export_name("wasm_init")))
void wasm_init(int width, int height) {
    (void)width;
    (void)height;

    sg_setup(&(sg_desc){
        .environment = {
            .defaults = {
                .color_format = SG_PIXELFORMAT_RGBA8,
                .depth_format = SG_PIXELFORMAT_DEPTH_STENCIL,
                .sample_count = 1,
            },
        },
        .logger.func = slog_func,
    });

    float vertices[] = {
        0.0f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f,  -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
    };
    state.bind.vertex_buffers[0] = sg_make_buffer(&(sg_buffer_desc){
        .data = SG_RANGE(vertices),
    });

    state.pip = sg_make_pipeline(&(sg_pipeline_desc){
        .shader = sg_make_shader(triangle_shader_desc(sg_query_backend())),
        .layout = {
            .attrs = {
                [ATTR_triangle_position].format = SG_VERTEXFORMAT_FLOAT3,
                [ATTR_triangle_color0].format = SG_VERTEXFORMAT_FLOAT4,
            },
        },
    });

    state.pass_action = (sg_pass_action){
        .colors[0] = {
            .load_action = SG_LOADACTION_CLEAR,
            .clear_value = { 0.0f, 0.0f, 0.0f, 1.0f },
        },
    };
}

__attribute__((export_name("wasm_frame")))
void wasm_frame(int width, int height) {
    sg_begin_pass(&(sg_pass){
        .action = state.pass_action,
        .swapchain = {
            .width = width,
            .height = height,
            .sample_count = 1,
            .color_format = SG_PIXELFORMAT_RGBA8,
            .depth_format = SG_PIXELFORMAT_DEPTH_STENCIL,
            .gl = { .framebuffer = 0 },
        },
    });
    sg_apply_pipeline(state.pip);
    sg_apply_bindings(&state.bind);
    sg_draw(0, 3, 1);
    sg_end_pass();
    sg_commit();
}

__attribute__((export_name("wasm_cleanup")))
void wasm_cleanup(void) {
    sg_shutdown();
}
