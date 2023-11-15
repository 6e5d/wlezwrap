#ifndef INCLUDEGUARD_WLEZWRAPH
#define INCLUDEGUARD_WLEZWRAPH

#include <stdbool.h>
#include <stdint.h>

#include "../../wlbasic/include/wlbasic.h"

typedef struct {
	Wlbasic wl;
	void (*f_resize)(void* data, uint32_t w, uint32_t h);
	void (*f_quit)(void* data);
	void (*f_motion)(void* data, double x, double y, double pressure);
	void (*f_button)(void* data, uint8_t button, bool pressed);
	void (*f_key)(void* data, char ch, bool pressed);
	void* data;
	struct zwp_tablet_tool_v2* peraser; // eraser is treated as middle button
	double pressure;
} Wlezwrap;

void wlezwrap_confgen(Wlezwrap* wew);
void wlezwrap_init(Wlezwrap* wew);
void wlezwrap_deinit(Wlezwrap* wew);

#endif
