#ifndef INCLUDEGUARD_WLEZWRAPH
#define INCLUDEGUARD_WLEZWRAPH

#include <stdint.h>
#include <stdbool.h>

#include "../../wlbasic/include/wlbasic.h"

typedef struct {
	Wlbasic wl;
	void (*f_resize)(void* data, uint32_t width, uint32_t height);
	void (*f_quit)(void* data);
	void (*f_motion)(void* data, float x, float y);
	void (*f_button)(void* data, uint8_t button, bool pressed);
	void (*f_key)(void* data, char ch, bool pressed);
	void* data;
} Wlezwrap;

void wlezwrap_confgen(Wlezwrap* wew);
void wlezwrap_init(Wlezwrap* wew);
void wlezwrap_deinit(Wlezwrap* wew);

#endif
