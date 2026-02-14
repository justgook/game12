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

BUILD_DIR ?= build.nosync
ASSETS_DIR ?= assets

$(BUILD_DIR)/renderer.wasm: main.c | $(BUILD_DIR)
	zig build-exe $< \
		-target wasm32-freestanding \
		-I wasm-include \
		-O ReleaseFast \
		-fno-entry \
		-rdynamic \
		-femit-bin=$@

$(BUILD_DIR):
	$(Q)$(MKDIR_P) $(BUILD_DIR)
