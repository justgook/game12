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
	./sokol-shdc --input triangle-sapp.glsl --output triangle-sapp.glsl.h --slang glsl430:metal_macos

.PHONY: all
all: build

.PHONY: build
build: triangle-sapp.glsl.h | $(BUILD_DIR)
	$(call QUIET, @echo "  ZIG CC  triangle")
	$(Q)zig cc sokol_impl.m main.c \
		-framework Metal -framework MetalKit \
		-framework Cocoa -framework QuartzCore -framework Foundation \
		-o $(BUILD_DIR)/triangle

$(BUILD_DIR):
	$(Q)$(MKDIR_P) $(BUILD_DIR)

.PHONY: clean
clean:
	$(Q)rm -rf $(BUILD_DIR)
