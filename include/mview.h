#ifndef INCLUDEGUARD_WLEZWRAP_MVIEWH
#define INCLUDEGUARD_WLEZWRAP_MVIEWH

#include <stdint.h>

#include "../include/wlezwrap.h"

typedef struct WlezwrapMview WlezwrapMview;

struct WlezwrapMview {
	uint8_t button;
	bool click;
	bool drag;
	double px;
	double py;
	void *data;
	void (*event)(WlezwrapMview *self, double x, double y);
};

void wlezwrap_mview_update(
	WlezwrapMview *wewmv, uint8_t type, WlezwrapEvent* e);

#endif
