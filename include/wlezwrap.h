#include <wayland-client.h>
#include "../../wlbasic/include/wlbasic.h"

const static uint8_t wlezwrap(lclick) = 128;
const static uint8_t wlezwrap(mclick) = 129;
const static uint8_t wlezwrap(rclick) = 130;
const static uint8_t wlezwrap(lshift) = 131;
const static uint8_t wlezwrap(lctrl) = 132;
const static uint8_t wlezwrap(lalt) = 133;
const static uint8_t wlezwrap(proximity) = 134;

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
	void *data;
	bool flip_button;
	// eraser is treated as middle button
	struct zwp_tablet_tool_v2 *peraser;
	float pressure; // pressure comes from independent event
} Wlezwrap();

void wlezwrap(confgen)(Wlezwrap()* wew);
void wlezwrap(init)(Wlezwrap()* wew);
void wlezwrap(deinit)(Wlezwrap()* wew);
