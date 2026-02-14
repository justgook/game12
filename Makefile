.ONESHELL:
.SHELLFLAGS := -eu -o pipefail -c
.DELETE_ON_ERROR:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

ifdef V
Q=
WGET:=wget
else
Q=@
MAKEFLAGS += --no-print-directory
WGET:=wget -q --show-progress
endif

define QUIET
	$(if $(V), , $(1))
endef

MKDIR_P ?= mkdir -p
CP ?= cp -f

.DEFAULT_GOAL := all

BUILD_DIR ?= build.nosync
ASSETS_DIR ?= assets


triangle-sapp.glsl.h: triangle-sapp.glsl
	./sokol-shdc --input triangle-sapp.glsl --output triangle-sapp.glsl.h --slang glsl430:metal_macos:glsl300es

.PHONY: all
all: build

.PHONY: build
build: triangle-sapp.glsl.h | $(BUILD_DIR)
	$(call QUIET, @echo "  ZIG CC  triangle")
	$(Q)zig cc sokol_impl.m main.c \
		-framework Metal -framework MetalKit \
		-framework Cocoa -framework QuartzCore -framework Foundation \
		-o $(BUILD_DIR)/triangle

.PHONY: web
web: triangle-sapp.glsl.h | $(BUILD_DIR)/web
	$(call QUIET, @echo "  ZIG CC  triangle.wasm")
	$(Q)zig build-exe \
		web/sokol_impl_web.c web/main_web.c \
		-target wasm32-freestanding \
		-fno-entry \
		-rdynamic \
		-O ReleaseFast \
		-isystem web/include \
		-Iweb \
		-DSOKOL_GLES3 \
		-DSOKOL_EXTERNAL_GL_LOADER \
		--import-memory \
		-femit-bin=$(BUILD_DIR)/web/triangle.wasm
	$(Q)$(CP) web/index.html $(BUILD_DIR)/web/index.html

$(BUILD_DIR):
	$(Q)$(MKDIR_P) $(BUILD_DIR)

$(BUILD_DIR)/web: | $(BUILD_DIR)
	$(Q)$(MKDIR_P) $(BUILD_DIR)/web

.PHONY: serve
serve: web
	$(call QUIET, @echo "  SERVE   http://localhost:8000")
	$(Q)python3 -m http.server 8000 --directory $(BUILD_DIR)/web

.PHONY: clean
clean:
	$(Q)rm -rf $(BUILD_DIR)
