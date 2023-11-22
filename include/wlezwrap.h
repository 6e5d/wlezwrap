#ifndef INCLUDEGUARD_WLEZWRAPH
#define INCLUDEGUARD_WLEZWRAPH

#define WLEZWRAP_LCLICK -1
#define WLEZWRAP_MCLICK -2
#define WLEZWRAP_RCLICK -3
#define WLEZWRAP_LSHIFT -4
#define WLEZWRAP_LCTRL -5
#define WLEZWRAP_LALT -6

#include <stdbool.h>
#include <stdint.h>

#include "../../wlbasic/include/wlbasic.h"

bool wlezwrap_isclick(int8_t key);

typedef union {
	uint8_t quit;
	uint32_t resize[2];
	double motion[3];
	int8_t key[2];
} WlezwrapEvent;

typedef struct {
	Wlbasic wl;
	void (*event)(void* data, uint8_t type, WlezwrapEvent *event);
	void* data;
	bool flip_button;
	struct zwp_tablet_tool_v2* peraser; // eraser is treated as middle button
	double pressure;
} Wlezwrap;

void wlezwrap_confgen(Wlezwrap* wew);
void wlezwrap_init(Wlezwrap* wew);
void wlezwrap_deinit(Wlezwrap* wew);

#endif
