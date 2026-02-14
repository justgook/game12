/**
 * App Event Bridge for Sokol GFX WASM
 * Maps browser events to Sokol App sapp_event structure
 */

class AppBridge {
  constructor(canvas, wasmMemory) {
    this.canvas = canvas;
    this.memory = wasmMemory;

    // Event structure offsets (must match sapp_event in sokol_app.h)
    // Verified with offsetof on native (wasm32 has same layout)
    this.eventOffsets = {
      frame_count: 0,      // uint64_t (8 bytes)
      type: 8,             // sapp_event_type (uint32_t, 4 bytes)
      key_code: 12,        // sapp_keycode (uint32_t, 4 bytes)
      char_code: 16,       // uint32_t (4 bytes)
      key_repeat: 20,      // bool (1 byte, but aligned to 4)
      modifiers: 24,       // uint32_t (4 bytes)
      mouse_button: 28,    // sapp_mousebutton (int, 4 bytes)
      mouse_x: 32,         // float (4 bytes)
      mouse_y: 36,         // float (4 bytes)
      mouse_dx: 40,        // float (4 bytes)
      mouse_dy: 44,        // float (4 bytes)
      scroll_x: 48,        // float (4 bytes)
      scroll_y: 52,        // float (4 bytes)
      num_touches: 56,     // int (4 bytes)
      touches: 64,         // sapp_touchpoint[8] array (each 24 bytes)
      window_width: 256,   // int (4 bytes)
      window_height: 260,  // int (4 bytes)
      framebuffer_width: 264,  // int (4 bytes)
      framebuffer_height: 268, // int (4 bytes)
    };

    // Total event structure size (verified with sizeof)
    this.eventSize = 272;

    // Current event pointer in WASM memory
    this.eventPtr = 0;
    this.eventBufferPtr = null;
    this.offsetsUpdated = false;

    // Event type constants (must match sapp_event_type in sokol_app.h)
    this.eventTypes = {
      INVALID: 0,
      KEY_DOWN: 1,
      KEY_UP: 2,
      CHAR: 3,
      MOUSE_DOWN: 4,
      MOUSE_UP: 5,
      MOUSE_SCROLL: 6,
      MOUSE_MOVE: 7,
      MOUSE_ENTER: 8,
      MOUSE_LEAVE: 9,
      TOUCHES_BEGAN: 10,
      TOUCHES_MOVED: 11,
      TOUCHES_ENDED: 12,
      TOUCHES_CANCELLED: 13,
      RESIZED: 14,
      ICONIFIED: 15,
      RESTORED: 16,
      FOCUSED: 17,
      UNFOCUSED: 18,
      SUSPENDED: 19,
      RESUMED: 20,
      QUIT_REQUESTED: 21,
      CLIPBOARD_PASTED: 22,
      FILES_DROPPED: 23,
    };

    // Mouse button constants (must match sapp_mousebutton in sokol_app.h)
    this.mouseButtons = {
      LEFT: 0,
      RIGHT: 1,
      MIDDLE: 2,
      INVALID: 0x100,
    };

    // Modifier key constants (must match sapp_modifier in sokol_app.h)
    this.modifiers = {
      SHIFT: 0x1,
      CTRL: 0x2,
      ALT: 0x4,
      SUPER: 0x8,
      LMB: 0x100,
      RMB: 0x200,
      MMB: 0x400,
    };

    // Frame counter
    this.frameCount = 0;

    // Previous mouse position for delta calculation
    this.prevMouseX = 0;
    this.prevMouseY = 0;

    // Debug flag
    this.debug = false;

    // Event listeners tracking
    this.eventListeners = [];

    // Initialize event listeners
    this.initEventListeners();
  }

  // Initialize event listeners on canvas
  initEventListeners() {
    if (!this.canvas) return;

    this.eventListeners = [];

    // Helper to add and track listener
    const addListener = (target, event, handler, options) => {
      const boundHandler = (e) => handler.call(this, e);
      target.addEventListener(event, boundHandler, options);
      this.eventListeners.push({ target, event, handler: boundHandler });
    };

    // Mouse movement
    addListener(this.canvas, 'mousemove', this.handleMouseMove);

    // Mouse buttons
    addListener(this.canvas, 'mousedown', this.handleMouseDown);
    addListener(this.canvas, 'mouseup', this.handleMouseUp);

    // Mouse wheel
    addListener(this.canvas, 'wheel', this.handleMouseWheel);

    // Mouse enter/leave
    addListener(this.canvas, 'mouseenter', this.handleMouseEnter);
    addListener(this.canvas, 'mouseleave', this.handleMouseLeave);

    // Window resize
    addListener(window, 'resize', this.handleResize);

    // Keyboard events (only when canvas has focus)
    addListener(this.canvas, 'keydown', this.handleKeyDown);
    addListener(this.canvas, 'keyup', this.handleKeyUp);
    addListener(this.canvas, 'keypress', this.handleKeyPress);

    // Focus events
    addListener(window, 'focus', () => this.handleFocus(true));
    addListener(window, 'blur', () => this.handleFocus(false));

    // Make canvas focusable for keyboard events
    this.canvas.setAttribute('tabindex', '0');
    this.canvas.style.outline = 'none';
  }

  // Get event buffer pointer from WASM (allocated in C)
  getEventBuffer() {
    if (!this.memory || !this.memory.buffer) {
      console.error('AppBridge: No WASM memory available');
      return 0;
    }

    this.updateOffsets();

    if (this.eventBufferPtr === null) {
      // Try to get buffer pointer from WASM exports
      if (typeof window.wasmModule !== 'undefined' && window.wasmModule.exports && window.wasmModule.exports.get_event_buffer) {
        this.eventBufferPtr = window.wasmModule.exports.get_event_buffer();
        console.log(`AppBridge: Event buffer at 0x${this.eventBufferPtr.toString(16)}`);
      } else {
        console.error('AppBridge: get_event_buffer not exported from WASM');
        // Fallback to fixed offset (less safe)
        this.eventBufferPtr = 1024 * 1024;
      }
    }

    return this.eventBufferPtr;
  }

  // Update offsets from WASM exports (if available)
  updateOffsets() {
    if (this.offsetsUpdated || !window.wasmModule || !window.wasmModule.exports) return;

    const exports = window.wasmModule.exports;
    if (exports.event_offset_mouse_x) {
      this.eventOffsets.mouse_x = exports.event_offset_mouse_x();
      this.eventOffsets.mouse_y = exports.event_offset_mouse_y();
      this.eventOffsets.window_width = exports.event_offset_window_width();
      this.eventOffsets.window_height = exports.event_offset_window_height();
      this.eventOffsets.framebuffer_width = exports.event_offset_framebuffer_width();
      this.eventOffsets.framebuffer_height = exports.event_offset_framebuffer_height();
      console.log('AppBridge: Updated event offsets from WASM');
      console.log(`  mouse_x=${this.eventOffsets.mouse_x}, window_width=${this.eventOffsets.window_width}`);
      this.offsetsUpdated = true;
    }
  }

  // Write event to WASM memory and call event function
  sendEvent(eventType, eventData = {}) {
    const ptr = this.getEventBuffer();
    if (!ptr) return;

    if (this.debug) {
      const typeName = Object.keys(this.eventTypes).find(key => this.eventTypes[key] === eventType) || 'UNKNOWN';
      console.log(`AppBridge: Sending event ${typeName} (${eventType})`, eventData);
    }

    const view = new DataView(this.memory.buffer);

    // Write common event fields
    this.writeUint64(view, ptr + this.eventOffsets.frame_count, this.frameCount++);
    this.writeUint32(view, ptr + this.eventOffsets.type, eventType);
    this.writeUint32(view, ptr + this.eventOffsets.modifiers, eventData.modifiers || 0);

    // Write mouse-specific fields
    if (eventData.mouse_x !== undefined) {
      this.writeFloat(view, ptr + this.eventOffsets.mouse_x, eventData.mouse_x);
    }
    if (eventData.mouse_y !== undefined) {
      this.writeFloat(view, ptr + this.eventOffsets.mouse_y, eventData.mouse_y);
    }
    if (eventData.mouse_dx !== undefined) {
      this.writeFloat(view, ptr + this.eventOffsets.mouse_dx, eventData.mouse_dx);
    }
    if (eventData.mouse_dy !== undefined) {
      this.writeFloat(view, ptr + this.eventOffsets.mouse_dy, eventData.mouse_dy);
    }
    if (eventData.mouse_button !== undefined) {
      this.writeInt32(view, ptr + this.eventOffsets.mouse_button, eventData.mouse_button);
    }
    if (eventData.scroll_x !== undefined) {
      this.writeFloat(view, ptr + this.eventOffsets.scroll_x, eventData.scroll_x);
    }
    if (eventData.scroll_y !== undefined) {
      this.writeFloat(view, ptr + this.eventOffsets.scroll_y, eventData.scroll_y);
    }

    // Write window size fields
    if (eventData.window_width !== undefined) {
      this.writeInt32(view, ptr + this.eventOffsets.window_width, eventData.window_width);
    }
    if (eventData.window_height !== undefined) {
      this.writeInt32(view, ptr + this.eventOffsets.window_height, eventData.window_height);
    }
    if (eventData.framebuffer_width !== undefined) {
      this.writeInt32(view, ptr + this.eventOffsets.framebuffer_width, eventData.framebuffer_width);
    }
    if (eventData.framebuffer_height !== undefined) {
      this.writeInt32(view, ptr + this.eventOffsets.framebuffer_height, eventData.framebuffer_height);
    }

    // Write key-specific fields
    if (eventData.key_code !== undefined) {
      this.writeUint32(view, ptr + this.eventOffsets.key_code, eventData.key_code);
    }
    if (eventData.char_code !== undefined) {
      this.writeUint32(view, ptr + this.eventOffsets.char_code, eventData.char_code);
    }
    if (eventData.key_repeat !== undefined) {
      this.writeBool(view, ptr + this.eventOffsets.key_repeat, eventData.key_repeat);
    }

    // Call WASM event function
    if (typeof window.wasmModule !== 'undefined' && window.wasmModule.exports && window.wasmModule.exports.event) {
      window.wasmModule.exports.event(ptr);
    }
  }

  // Helper methods for writing to WASM memory
  writeUint32(view, offset, value) {
    view.setUint32(offset, value, true);
  }

  writeInt32(view, offset, value) {
    view.setInt32(offset, value, true);
  }

  writeUint64(view, offset, value) {
    // Write 64-bit value as two 32-bit parts
    view.setUint32(offset, value & 0xFFFFFFFF, true);
    view.setUint32(offset + 4, Math.floor(value / 0x100000000), true);
  }

  writeFloat(view, offset, value) {
    view.setFloat32(offset, value, true);
  }

  writeBool(view, offset, value) {
    view.setUint8(offset, value ? 1 : 0);
  }

  // Convert browser coordinates to canvas coordinates
  getCanvasCoordinates(clientX, clientY) {
    const rect = this.canvas.getBoundingClientRect();
    const scaleX = this.canvas.width / rect.width;
    const scaleY = this.canvas.height / rect.height;

    return {
      x: (clientX - rect.left) * scaleX,
      y: (clientY - rect.top) * scaleY
    };
  }

  // Calculate mouse delta
  updateMouseDelta(x, y) {
    const dx = x - this.prevMouseX;
    const dy = y - this.prevMouseY;
    this.prevMouseX = x;
    this.prevMouseY = y;
    return { dx, dy };
  }

  // Event handlers
  handleMouseMove(e) {
    const coords = this.getCanvasCoordinates(e.clientX, e.clientY);
    const delta = this.updateMouseDelta(coords.x, coords.y);

    this.sendEvent(this.eventTypes.MOUSE_MOVE, {
      mouse_x: coords.x,
      mouse_y: coords.y,
      mouse_dx: delta.dx,
      mouse_dy: delta.dy,
      modifiers: this.getModifiers(e)
    });
  }

  handleMouseDown(e) {
    const coords = this.getCanvasCoordinates(e.clientX, e.clientY);
    const button = this.getMouseButton(e.button);

    this.sendEvent(this.eventTypes.MOUSE_DOWN, {
      mouse_x: coords.x,
      mouse_y: coords.y,
      mouse_button: button,
      modifiers: this.getModifiers(e) | this.getMouseButtonModifier(button)
    });
  }

  handleMouseUp(e) {
    const coords = this.getCanvasCoordinates(e.clientX, e.clientY);
    const button = this.getMouseButton(e.button);

    this.sendEvent(this.eventTypes.MOUSE_UP, {
      mouse_x: coords.x,
      mouse_y: coords.y,
      mouse_button: button,
      modifiers: this.getModifiers(e) | this.getMouseButtonModifier(button)
    });
  }

  handleMouseWheel(e) {
    e.preventDefault();

    // Normalize wheel delta across browsers
    const deltaX = e.deltaX || 0;
    const deltaY = e.deltaY || 0;

    this.sendEvent(this.eventTypes.MOUSE_SCROLL, {
      scroll_x: deltaX * 0.01,
      scroll_y: deltaY * 0.01,
      modifiers: this.getModifiers(e)
    });
  }

  handleMouseEnter(e) {
    this.sendEvent(this.eventTypes.MOUSE_ENTER, {
      modifiers: this.getModifiers(e)
    });
  }

  handleMouseLeave(e) {
    this.sendEvent(this.eventTypes.MOUSE_LEAVE, {
      modifiers: this.getModifiers(e)
    });
  }

  handleResize() {
    // Use canvas dimensions as window dimensions
    this.sendEvent(this.eventTypes.RESIZED, {
      window_width: this.canvas.width,
      window_height: this.canvas.height,
      framebuffer_width: this.canvas.width,
      framebuffer_height: this.canvas.height
    });
  }

  handleKeyDown(e) {
    const keyCode = this.mapKeyCode(e.keyCode);
    this.sendEvent(this.eventTypes.KEY_DOWN, {
      key_code: keyCode,
      key_repeat: e.repeat,
      modifiers: this.getModifiers(e)
    });

    // Prevent default for certain keys
    if (this.shouldPreventDefault(e.keyCode)) {
      e.preventDefault();
    }
  }

  handleKeyUp(e) {
    const keyCode = this.mapKeyCode(e.keyCode);
    this.sendEvent(this.eventTypes.KEY_UP, {
      key_code: keyCode,
      key_repeat: e.repeat,
      modifiers: this.getModifiers(e)
    });
  }

  handleKeyPress(e) {
    this.sendEvent(this.eventTypes.CHAR, {
      char_code: e.charCode || e.keyCode,
      key_repeat: e.repeat,
      modifiers: this.getModifiers(e)
    });
  }

  handleFocus(focused) {
    this.sendEvent(focused ? this.eventTypes.FOCUSED : this.eventTypes.UNFOCUSED, {});
  }

  // Helper: Get modifier keys from browser event
  getModifiers(e) {
    let modifiers = 0;
    if (e.shiftKey) modifiers |= this.modifiers.SHIFT;
    if (e.ctrlKey) modifiers |= this.modifiers.CTRL;
    if (e.altKey) modifiers |= this.modifiers.ALT;
    if (e.metaKey) modifiers |= this.modifiers.SUPER;
    return modifiers;
  }

  // Helper: Map browser mouse button to sokol button
  getMouseButton(button) {
    switch (button) {
      case 0: return this.mouseButtons.LEFT;
      case 1: return this.mouseButtons.MIDDLE;
      case 2: return this.mouseButtons.RIGHT;
      default: return this.mouseButtons.INVALID;
    }
  }

  // Helper: Get modifier for mouse button
  getMouseButtonModifier(button) {
    switch (button) {
      case this.mouseButtons.LEFT: return this.modifiers.LMB;
      case this.mouseButtons.RIGHT: return this.modifiers.RMB;
      case this.mouseButtons.MIDDLE: return this.modifiers.MMB;
      default: return 0;
    }
  }

  // Helper: Map browser key codes to sokol key codes (simplified)
  mapKeyCode(keyCode) {
    // This is a simplified mapping
    // For a complete implementation, see sokol_app.h key codes
    return keyCode;
  }

  // Helper: Determine if we should prevent default for this key
  shouldPreventDefault(keyCode) {
    // Prevent default for arrow keys, space, etc. to avoid scrolling
    const preventKeys = [
      32,  // Space
      37,  // Arrow Left
      38,  // Arrow Up
      39,  // Arrow Right
      40,  // Arrow Down
      9,   // Tab
      13,  // Enter
    ];
    return preventKeys.includes(keyCode);
  }

  // Cleanup event listeners
  cleanup() {
    if (this.eventListeners) {
      for (const { target, event, handler } of this.eventListeners) {
        target.removeEventListener(event, handler);
      }
      this.eventListeners = [];
    }
    if (this.debug) {
      console.log('AppBridge: Cleaned up event listeners');
    }
  }
}

// Export for use in browser
if (typeof module !== 'undefined' && module.exports) {
  module.exports = { AppBridge };
}
