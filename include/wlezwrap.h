#ifndef INCLUDEGUARD_WLEZWRAP
#define INCLUDEGUARD_WLEZWRAP

#include "../../wlbasic/include/wlbasic.h"
#define Tabtool zwp_tablet_tool_v2
#define Tab zwp_tablet_v2
#define WlPointer wl_pointer
#define WlKeyboard wl_keyboard
#define WlSurface wl_surface
#define WlArray wl_array
#define WlFixed wl_fixed_t
#define XdgToplevel xdg_toplevel

bool wlezwrap(isclick)(uint8_t key);

typedef union {
	uint8_t quit;
	uint32_t resize[2];
	float motion[3];
	uint8_t key[2];
} Wlezwrap(Event);

typedef struct {
	bool keystate[256];
	bool hide_cursor;
	uint32_t pointer_enter_serial;
	Wlbasic() wl;
	void (*event)(void*, uint8_t, Wlezwrap(Event)*);
	void* data;
	bool flip_button;
	// eraser is treated as middle button
	struct Tabtool* peraser;
	float pressure; // pressure comes from independent event
} Wlezwrap();

void wlezwrap(confgen)(Wlezwrap()* wew);
void wlezwrap(init)(Wlezwrap()* wew);
void wlezwrap(deinit)(Wlezwrap()* wew);

#endif
