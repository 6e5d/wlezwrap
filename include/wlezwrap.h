#ifndef INCLUDEGUARD_WLEZWRAP
#define INCLUDEGUARD_WLEZWRAP

#define WLEZWRAP_LCLICK 128
#define WLEZWRAP_MCLICK 129
#define WLEZWRAP_RCLICK 130
#define WLEZWRAP_LSHIFT 131
#define WLEZWRAP_LCTRL 132
#define WLEZWRAP_LALT 133
#define WLEZWRAP_PROXIMITY 134

#include "../../wlbasic/include/wlbasic.h"

bool wlezwrap_isclick(uint8_t key);

typedef union {
	uint8_t quit;
	uint32_t resize[2];
	float motion[3];
	uint8_t key[2];
} WlezwrapEvent;

typedef struct {
	bool keystate[256];
	bool hide_cursor;
	Wlbasic wl;
	void (*event)(void* data, uint8_t type, WlezwrapEvent *event);
	void* data;
	bool flip_button;
	// eraser is treated as middle button
	struct zwp_tablet_tool_v2* peraser;
	float pressure; // pressure comes from independent event
} Wlezwrap;

void wlezwrap_confgen(Wlezwrap* wew);
void wlezwrap_init(Wlezwrap* wew);
void wlezwrap_deinit(Wlezwrap* wew);

#endif
