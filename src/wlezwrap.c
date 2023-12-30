#include <wayland-client.h>

#include "../include/wlezwrap.h"

#define WLEZWRAP_LCLICK 128
#define WLEZWRAP_MCLICK 129
#define WLEZWRAP_RCLICK 130
#define WLEZWRAP_LSHIFT 131
#define WLEZWRAP_LCTRL 132
#define WLEZWRAP_LALT 133
#define WLEZWRAP_PROXIMITY 134

bool wlezwrap(isclick)(uint8_t key) {
	return (
		key == WLEZWRAP_LCLICK ||
		key == WLEZWRAP_MCLICK ||
		key == WLEZWRAP_RCLICK);
}

// default do-nothing functions
static void f_event(void* data, uint8_t ty, Wlezwrap(Event)* event) {}

// wrappers
static void wrapper_button(void* data, struct WlPointer *pointer,
	uint32_t unused1, uint32_t unused2, uint32_t button, uint32_t state
) {
	Wlezwrap()* wew = ((Wlbasic()*)data)->next;
	uint8_t b = 0;
	if (button == 272) {
		b = WLEZWRAP_LCLICK;
	} else if (button == 273) {
		b = WLEZWRAP_RCLICK;
	} else if (button == 274) {
		b = WLEZWRAP_MCLICK;
	} else {
		return;
	}
	Wlezwrap(Event) e;
	e.key[0] = b;
	e.key[1] = 0;
	if (state == WL_POINTER_BUTTON_STATE_PRESSED) {
		e.key[1] = 1;
		wew->keystate[(size_t)b] = true;
	} else {
		wew->keystate[(size_t)b] = false;
	}
	wew->event(wew->data, 3, &e);
}

static void wrapper_tabtool_pin(void* data,
	struct Tabtool* tabtool, uint32_t unused1,
	struct Tab* tab, struct WlSurface *surface)
{
	Wlbasic() *wl = data;
	Wlezwrap()* wew = wl->next;
	if (wew->hide_cursor) {
		wl_pointer_set_cursor(wl->pointer, wew->pointer_enter_serial,
			NULL, 0, 0);
	}
	wew->keystate[WLEZWRAP_PROXIMITY] = true;
	Wlezwrap(Event) e;
	e.key[0] = WLEZWRAP_PROXIMITY;
	e.key[1] = true;
	wew->event(wew->data, 3, &e);
}
static void wrapper_tabtool_pout(void* data, struct Tabtool* tabtool) {
	Wlbasic() *wl = data;
	Wlezwrap()* wew = wl->next;
	wew->keystate[WLEZWRAP_PROXIMITY] = false;
	Wlezwrap(Event) e;
	e.key[0] = WLEZWRAP_PROXIMITY;
	e.key[1] = false;
	wew->event(wew->data, 3, &e);
}

static void wrapper_motion(void *data, struct WlPointer *pointer,
	uint32_t unused1, WlFixed x, WlFixed y
) {
	Wlbasic() *wl = data;
	Wlezwrap()* wew = wl->next;
	Wlezwrap(Event) e;
	e.motion[0] = (float)wl_fixed_to_double(x) * (float)wl->scale;
	e.motion[1] = (float)wl_fixed_to_double(y) * (float)wl->scale;
	e.motion[2] = (float)wew->pressure;
	wew->event(wew->data, 2, &e);
}

static void wrapper_quit(void* data, struct XdgToplevel* toplevel) {
	Wlezwrap()* wew = ((Wlbasic()*)data)->next;
	Wlezwrap(Event) e = {0};
	wew->event(wew->data, 0, &e);
}

static void wrapper_pointer_enter(void* data, struct WlPointer *pointer,
	uint32_t serial, struct WlSurface *surface, WlFixed x, WlFixed y
) {
	Wlezwrap()* wew = ((Wlbasic()*)data)->next;
	wew->pointer_enter_serial = serial;
}

static void wrapper_resize(void *data, struct XdgToplevel *toplevel,
	int32_t width, int32_t height, struct WlArray *array
) {
	if (width <= 0 || height <= 0) { return; }
	Wlbasic() *wl = data;
	Wlezwrap() *wew = wl->next;
	Wlezwrap(Event) e;
	e.resize[0] = (uint32_t)(width * wl->scale);
	e.resize[1] = (uint32_t)(height * wl->scale);
	wew->event(wew->data, 1, &e);
}

static void wrapper_key(void* data, struct WlKeyboard *keyboard,
	uint32_t unused1, uint32_t unused2, uint32_t key, uint32_t state
) {
	uint8_t charmap[130] = {
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
	Wlezwrap()* wew = ((Wlbasic()*)data)->next;
	if (key >= 128) { return; }
	Wlezwrap(Event) e;
	uint8_t k = charmap[(size_t) key];
	e.key[0] = k;
	e.key[1] = 0;
	if (state == WL_KEYBOARD_KEY_STATE_PRESSED) {
		e.key[1] = 1;
		wew->keystate[(size_t)k] = true;
	} else {
		wew->keystate[(size_t)k] = false;
	}
	wew->event(wew->data, 3, &e);
}

static void wrapper_tabtool_motion(void *data, struct Tabtool* tabtool,
	WlFixed x, WlFixed y) {
	Wlbasic() *wl = data;
	Wlezwrap() *wew = wl->next;
	Wlezwrap(Event) e;
	e.motion[0] = (float)wl_fixed_to_double(x) * (float)wl->scale;
	e.motion[1] = (float)wl_fixed_to_double(y) * (float)wl->scale;
	e.motion[2] = (float)wew->pressure;
	wew->event(wew->data, 2, &e);
}

static void wrapper_tabtool_down(void *data,
	struct Tabtool* tabtool, uint32_t unused
) {
	Wlezwrap()* wew = ((Wlbasic()*)data)->next;
	Wlezwrap(Event) e;
	e.key[0] = WLEZWRAP_LCLICK;
	e.key[1] = 1;
	wew->event(wew->data, 3, &e);
}
static void wrapper_tabtool_up(void *data, struct Tabtool *tabtool) {
	Wlezwrap()* wew = ((Wlbasic()*)data)->next;
	Wlezwrap(Event) e;
	e.key[0] = WLEZWRAP_LCLICK;
	e.key[1] = 0;
	wew->event(wew->data, 3, &e);
}

static void wrapper_tabtool_button(void *data, struct Tabtool *tabtool,
	uint32_t unused1, uint32_t button, uint32_t state) {
	Wlezwrap()* wew = ((Wlbasic()*)data)->next;
	Wlezwrap(Event) e;
	// very hacky way to ensure physical usage consistency between
	// cintiq tablets and wacom AES laptops (like x1 yoga).
	// The latter one use lower button for eraser
	// and use upper button for middle button
	if (wew->flip_button) {
		e.key[0] = WLEZWRAP_MCLICK;
		if (button == 331) {
			e.key[0] = WLEZWRAP_RCLICK;
		}
	} else {
		e.key[0] = WLEZWRAP_RCLICK;
		if (button == 331) {
			e.key[0] = WLEZWRAP_MCLICK;
		}
	}
	e.key[1] = 0;
	if (state == ZWP_TABLET_TOOL_V2_BUTTON_STATE_PRESSED) {
		e.key[1] = 1;
	}
	wew->event(wew->data, 3, &e);
}

static void wrapper_tabtool_pressure(void *data,
	struct Tabtool *tabtool, uint32_t pressure
) {
	Wlezwrap()* wew = ((Wlbasic()*)data)->next;
	wew->pressure = (float)pressure / 65536.0f;
}


static void wrapper_tabtool_type(void *data, struct Tabtool* tool,
	uint32_t type) {
	if (type == ZWP_TABLET_TOOL_V2_TYPE_ERASER) {
		Wlezwrap()* wew = ((Wlbasic()*)data)->next;
		wew->peraser = tool;
	}
}

void wlezwrap(confgen)(Wlezwrap()* wew) {
	wlbasic(config_default)(&wew->wl.conf);
	wew->wl.next = (void*)wew; // loopback
	wew->event = f_event;
}

static void tabname_hack(void *data, struct Tabtool* tool, char *name) {
	Wlbasic() *wl = data;
	Wlezwrap()* wew = wl->next;
	if (strstr(name, "multitouch")) {
		wew->flip_button = true;
	}
}

void wlezwrap(init)(Wlezwrap()* wew) {
	wew->flip_button = false;
	wew->wl.conf.pointer_listener.button = wrapper_button;
	wew->wl.conf.pointer_listener.motion = wrapper_motion;
	wew->wl.conf.pointer_listener.enter = wrapper_pointer_enter;
	wew->wl.conf.toplevel_listener.close = wrapper_quit;
	wew->wl.conf.toplevel_listener.configure = wrapper_resize;
	wew->wl.conf.keyboard_listener.key = wrapper_key;
	wew->wl.conf.tablet_listener.name = tabname_hack;
	wew->wl.conf.tabtool_listener.motion = wrapper_tabtool_motion;
	wew->wl.conf.tabtool_listener.button = wrapper_tabtool_button;
	wew->wl.conf.tabtool_listener.type = wrapper_tabtool_type;
	wew->wl.conf.tabtool_listener.up = wrapper_tabtool_up;
	wew->wl.conf.tabtool_listener.down = wrapper_tabtool_down;
	wew->wl.conf.tabtool_listener.proximity_in = wrapper_tabtool_pin;
	wew->wl.conf.tabtool_listener.proximity_out = wrapper_tabtool_pout;
	wew->wl.conf.tabtool_listener.pressure = wrapper_tabtool_pressure;
	wlbasic(init)(&wew->wl);
}

void wlezwrap(deinit)(Wlezwrap()* wew) {
	wlbasic(deinit)(&wew->wl);
}
