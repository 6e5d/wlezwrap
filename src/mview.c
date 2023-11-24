#include <stdbool.h>
#include <stdint.h>

#include "../include/mview.h"
#include "../include/wlezwrap.h"

void wlezwrap_mview_update(
	WlezwrapMview *wewmv, uint8_t type, WlezwrapEvent *e) {
	switch(type) {
	case 2: {
		if (!wewmv->click) {
			wewmv->drag = false;
			return;
		}
		double x = e->motion[0];
		double y = e->motion[1];
		if (wewmv->drag) {
			wewmv->event(wewmv, x, y);
		}
		wewmv->drag = true;
		wewmv->px = x;
		wewmv->py = y;
		break;
	}
	case 3: {
		bool pressed = e->key[1];
		char key = e->key[0];
		if (!pressed) {
			wewmv->click = false;
			wewmv->drag = false;
			if (key < 0) { // modifiers
				wewmv->button = 0;
			}
			return;
		}
		if (key == WLEZWRAP_LSHIFT) {
			wewmv->button = 1;
		} else if (key == WLEZWRAP_LCTRL) {
			wewmv->button = 2;
		} else if (key == WLEZWRAP_MCLICK) {
			wewmv->click = true;
		}
		break;
	}
	}
}
