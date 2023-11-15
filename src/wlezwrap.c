#include <stdio.h>
#include <wayland-client.h>

#include "../include/wlezwrap.h"

// definitions of non char key should be formalized
static const char CHARMAP[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 'q', 'w', 'e', 'r',
	't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\r', -2,
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
	0, 0, -1, 0, 'z', 'x', 'c', 'v', 'b', 'n',
	'm', ',', '.', '/', 0, 0, -3, ' ', 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

// default do-nothing functions
static void f_resize(void* data, uint32_t width, uint32_t height) {}
static void f_quit(void* data) {}
static void f_motion(void* data, float x, float y) {}
static void f_button(void* data, uint8_t button, bool pressed) {}
static void f_key(void* data, char ch, bool pressed) {}

// wrappers
static void wrapper_button(void* data, struct wl_pointer *wl_pointer,
	uint32_t serial, uint32_t time, uint32_t button, uint32_t state
) {
	Wlezwrap* wew = ((Wlbasic*)data)->next;
	uint8_t b = 0;
	if (button == 272) {
		b = 0;
	} else if (button == 273) {
		b = 2;
	} else if (button == 274) {
		b = 1;
	} else {
		return;
	}
	wew->f_button(wew->data, b, state == WL_POINTER_BUTTON_STATE_PRESSED);
}

static void wrapper_motion(void *data, struct wl_pointer *wl_pointer,
	uint32_t time, wl_fixed_t x, wl_fixed_t y
) {
	Wlezwrap* wew = ((Wlbasic*)data)->next;
	wew->f_motion(wew->data, (float)x, (float)y);
}

static void wrapper_quit(void* data, struct xdg_toplevel* toplevel) {
	Wlezwrap* wew = ((Wlbasic*)data)->next;
	wew->f_quit(wew->data);
}

static void wrapper_resize(void* data, struct xdg_toplevel* toplevel,
	int32_t width, int32_t height, struct wl_array* states
) {
	if (width <= 0 || height <= 0) { return; }
	Wlezwrap* wew = ((Wlbasic*)data)->next;
	wew->f_resize(wew->data, (uint32_t)width, (uint32_t)height);
}

static void wrapper_key(void* data, struct wl_keyboard *wl_keyboard,
	uint32_t serial, uint32_t time, uint32_t key, uint32_t state
) {
	Wlezwrap* wew = ((Wlbasic*)data)->next;
	if (key >= 128) { return; }
	char c = CHARMAP[(size_t) key];
	wew->f_key(wew->data, c, state == WL_KEYBOARD_KEY_STATE_PRESSED);
}

static void wrapper_tabtool_motion(void *data, struct zwp_tablet_tool_v2* tool,
	wl_fixed_t x, wl_fixed_t y) {
	Wlezwrap* wew = ((Wlbasic*)data)->next;
	wew->f_motion(wew->data, (float)x, (float)y);
}

static void wrapper_tabtool_down(void *data, struct zwp_tablet_tool_v2* tool,
	uint32_t serial) {
	Wlezwrap* wew = ((Wlbasic*)data)->next;
	wew->f_button(wew->data, 0, true);
}
static void wrapper_tabtool_up(void *data, struct zwp_tablet_tool_v2* tool) {
	Wlezwrap* wew = ((Wlbasic*)data)->next;
	wew->f_button(wew->data, 0, false);
}

static void wrapper_tabtool_pin(void *data, struct zwp_tablet_tool_v2* tool,
	uint32_t serial,
	struct zwp_tablet_v2 *tablet,
	struct wl_surface *surface
) {
	Wlezwrap* wew = ((Wlbasic*)data)->next;
	if (tool == wew->peraser) {
		wew->f_button(wew->data, 1, true);
	}
}

static void wrapper_tabtool_pout(void *data, struct zwp_tablet_tool_v2* tool) {
	Wlezwrap* wew = ((Wlbasic*)data)->next;
	if (tool == wew->peraser) {
		wew->f_button(wew->data, 1, false);
	}
}

static void wrapper_tabtool_button(void *data, struct zwp_tablet_tool_v2* tool,
	uint32_t serial, uint32_t button, uint32_t state) {
	Wlezwrap* wew = ((Wlbasic*)data)->next;
	// TODO: what does button id mean?
	uint8_t bid = 1;
	if (button == 331) {
		bid = 2;
	}
	wew->f_button(wew->data, bid,
		state == ZWP_TABLET_TOOL_V2_BUTTON_STATE_PRESSED);
}

static void wrapper_tabtool_pressure(void *data,
	struct zwp_tablet_tool_v2* tool, uint32_t pressure) {
	Wlezwrap* wew = ((Wlbasic*)data)->next;
	wew->pressure = (float)pressure;
}


static void wrapper_tabtool_type(void *data, struct zwp_tablet_tool_v2* tool,
	uint32_t type) {
	if (type == ZWP_TABLET_TOOL_V2_TYPE_ERASER) {
		Wlezwrap* wew = ((Wlbasic*)data)->next;
		wew->peraser = tool;
	}
}

void wlezwrap_confgen(Wlezwrap* wew) {
	wlbasic_config_default(&wew->wl.conf);
	wew->wl.next = (void*)wew; // loopback
	wew->f_resize = f_resize;
	wew->f_quit = f_quit;
	wew->f_motion = f_motion;
	wew->f_button = f_button;
	wew->f_key = f_key;
}

void wlezwrap_init(Wlezwrap* wew) {
	wew->wl.conf.pointer_listener.button = wrapper_button;
	wew->wl.conf.pointer_listener.motion = wrapper_motion;
	wew->wl.conf.toplevel_listener.close = wrapper_quit;
	wew->wl.conf.toplevel_listener.configure = wrapper_resize;
	wew->wl.conf.keyboard_listener.key = wrapper_key;
	wew->wl.conf.tabtool_listener.motion = wrapper_tabtool_motion;
	wew->wl.conf.tabtool_listener.button = wrapper_tabtool_button;
	wew->wl.conf.tabtool_listener.type = wrapper_tabtool_type;
	wew->wl.conf.tabtool_listener.up = wrapper_tabtool_up;
	wew->wl.conf.tabtool_listener.down = wrapper_tabtool_down;
	wew->wl.conf.tabtool_listener.proximity_in = wrapper_tabtool_pin;
	wew->wl.conf.tabtool_listener.proximity_out = wrapper_tabtool_pout;
	wew->wl.conf.tabtool_listener.pressure = wrapper_tabtool_pressure;
	wlbasic_init(&wew->wl);
}

void wlezwrap_deinit(Wlezwrap* wew) {
	wlbasic_deinit(&wew->wl);
}
