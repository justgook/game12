.ONESHELL:
.SHELLFLAGS := -eu -c
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
	$(Q)echo "Bild Native App"
	$(Q)zig build-exe \
		osx/sokol_impl.m osx/main.c main.c \
		-framework Metal -framework MetalKit \
		-framework Cocoa -framework QuartzCore -framework Foundation \
		-femit-bin=$(BUILD_DIR)/triangle

.PHONY: web
web: $(BUILD_DIR)/web/triangle.wasm $(BUILD_DIR)/web/index.html $(BUILD_DIR)/web/gl-bridge.js $(BUILD_DIR)/web/app-bridge.js

$(BUILD_DIR)/web/triangle.wasm: web/main.c main.c | $(BUILD_DIR)/web
	$(Q)echo "Bild WASM App"
	$(Q)zig build-exe \
		$^ \
		-target wasm32-freestanding \
		-fno-entry \
		-rdynamic \
		-O ReleaseFast \
		-I web/wasm-include \
		-femit-bin=$@

$(BUILD_DIR)/web/index.html: web/index.html | $(BUILD_DIR)/web
	$(Q)$(CP) $< $@

$(BUILD_DIR)/web/gl-bridge.js: web/gl-bridge.js | $(BUILD_DIR)/web
	$(Q)$(CP) $< $@

$(BUILD_DIR)/web/app-bridge.js: web/app-bridge.js | $(BUILD_DIR)/web
	$(Q)$(CP) $< $@

$(BUILD_DIR):
	$(Q)$(MKDIR_P) $(BUILD_DIR)

$(BUILD_DIR)/web: | $(BUILD_DIR)
	$(Q)$(MKDIR_P) $(BUILD_DIR)/web

.PHONY: serve
serve: web
	$(call QUIET, @echo "  SERVE   http://localhost:8812")
	$(Q)python3 -m http.server 8812 --directory $(BUILD_DIR)/web

.PHONY: clean
clean:
	$(Q)rm -rf $(BUILD_DIR)
