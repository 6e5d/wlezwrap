#include <stdio.h>
#include <wayland-client.h>

#include "../include/wlezwrap.h"

static const int8_t CHARMAP[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 'q', 'w', 'e', 'r',
	't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\r', WLEZWRAP_LCTRL,
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
	0, 0, WLEZWRAP_LSHIFT, 0, 'z', 'x', 'c', 'v', 'b', 'n',
	'm', ',', '.', '/', 0, 0, WLEZWRAP_LALT, ' ', 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

bool wlezwrap_isclick(int8_t key) {
	return (key <= -1 && key >= -3);
}

// default do-nothing functions
static void f_event(void* data, uint8_t type, WlezwrapEvent *event) {}

// wrappers
static void wrapper_button(void* data, struct wl_pointer *wl_pointer,
	uint32_t serial, uint32_t time, uint32_t button, uint32_t state
) {
	Wlezwrap* wew = ((Wlbasic*)data)->next;
	int8_t b = 0;
	if (button == 272) {
		b = WLEZWRAP_LCLICK;
	} else if (button == 273) {
		b = WLEZWRAP_RCLICK;
	} else if (button == 274) {
		b = WLEZWRAP_MCLICK;
	} else {
		return;
	}
	WlezwrapEvent e;
	e.key[0] = b;
	e.key[1] = 0;
	if (state == WL_POINTER_BUTTON_STATE_PRESSED) {
		e.key[1] = 1;
	}
	wew->event(wew->data, 3, &e);
}

static void wrapper_motion(void *data, struct wl_pointer *wl_pointer,
	uint32_t time, wl_fixed_t x, wl_fixed_t y
) {
	Wlezwrap* wew = ((Wlbasic*)data)->next;
	WlezwrapEvent e;
	e.motion[0] = wl_fixed_to_double(x);
	e.motion[1] = wl_fixed_to_double(y);
	e.motion[2] = wew->pressure;
	wew->event(wew->data, 2, &e);
}

static void wrapper_quit(void* data, struct xdg_toplevel* toplevel) {
	Wlezwrap* wew = ((Wlbasic*)data)->next;
	WlezwrapEvent e = {0};
	wew->event(wew->data, 0, &e);
}

static void wrapper_resize(void* data, struct xdg_toplevel* toplevel,
	int32_t width, int32_t height, struct wl_array* states
) {
	if (width <= 0 || height <= 0) { return; }
	Wlezwrap* wew = ((Wlbasic*)data)->next;
	WlezwrapEvent e;
	e.resize[0] = (uint32_t)width;
	e.resize[1] = (uint32_t)height;
	wew->event(wew->data, 1, &e);
}

static void wrapper_key(void* data, struct wl_keyboard *wl_keyboard,
	uint32_t serial, uint32_t time, uint32_t key, uint32_t state
) {
	Wlezwrap* wew = ((Wlbasic*)data)->next;
	if (key >= 128) { return; }
	WlezwrapEvent e;
	e.key[0] = CHARMAP[(size_t) key];
	e.key[1] = 0;
	if (state == WL_KEYBOARD_KEY_STATE_PRESSED) {
		e.key[1] = 1;
	}
	wew->event(wew->data, 3, &e);
}

static void wrapper_tabtool_motion(void *data, struct zwp_tablet_tool_v2* tool,
	wl_fixed_t x, wl_fixed_t y) {
	Wlezwrap* wew = ((Wlbasic*)data)->next;
	WlezwrapEvent e;
	e.motion[0] = wl_fixed_to_double(x);
	e.motion[1] = wl_fixed_to_double(y);
	e.motion[2] = wew->pressure;
	wew->event(wew->data, 2, &e);
}

static void wrapper_tabtool_down(void *data, struct zwp_tablet_tool_v2* tool,
	uint32_t serial) {
	Wlezwrap* wew = ((Wlbasic*)data)->next;
	WlezwrapEvent e;
	e.key[0] = WLEZWRAP_LCLICK;
	e.key[1] = 1;
	wew->event(wew->data, 3, &e);
}
static void wrapper_tabtool_up(void *data, struct zwp_tablet_tool_v2* tool) {
	Wlezwrap* wew = ((Wlbasic*)data)->next;
	WlezwrapEvent e;
	e.key[0] = WLEZWRAP_LCLICK;
	e.key[1] = 0;
	wew->event(wew->data, 3, &e);
}

static void wrapper_tabtool_pin(void *data, struct zwp_tablet_tool_v2* tool,
	uint32_t serial,
	struct zwp_tablet_v2 *tablet,
	struct wl_surface *surface
) {
	Wlezwrap* wew = ((Wlbasic*)data)->next;
	if (tool == wew->peraser) {
		WlezwrapEvent e;
		e.key[0] = WLEZWRAP_MCLICK;
		e.key[1] = 1;
		wew->event(wew->data, 3, &e);
	}
}

static void wrapper_tabtool_pout(void *data, struct zwp_tablet_tool_v2* tool) {
	Wlezwrap* wew = ((Wlbasic*)data)->next;
	if (tool == wew->peraser) {
		WlezwrapEvent e;
		e.key[0] = WLEZWRAP_MCLICK;
		e.key[1] = 0;
		wew->event(wew->data, 3, &e);
	}
}

static void wrapper_tabtool_button(void *data, struct zwp_tablet_tool_v2* tool,
	uint32_t serial, uint32_t button, uint32_t state) {
	Wlezwrap* wew = ((Wlbasic*)data)->next;
	WlezwrapEvent e;
	e.key[0] = WLEZWRAP_MCLICK;
	// TODO: what does button id mean?
	if (button == 331) {
		e.key[0] = WLEZWRAP_RCLICK;
	}
	e.key[1] = 0;
	if (state == ZWP_TABLET_TOOL_V2_BUTTON_STATE_PRESSED) {
		e.key[1] = 1;
	}
	wew->event(wew->data, 3, &e);
}

static void wrapper_tabtool_pressure(void *data,
	struct zwp_tablet_tool_v2* tool, uint32_t pressure) {
	Wlezwrap* wew = ((Wlbasic*)data)->next;
	wew->pressure = (double)pressure / 65536.0;
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
	wew->event = f_event;
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
