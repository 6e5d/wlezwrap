// TODO: buffer
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <wayland-client.h>

#include "../include/wlezwrap.h"

static void f_event(void* data, uint8_t type, WlezwrapEvent *event) {
	if (type == 0) {
		*(bool*)data = true;
	}
}

int main(void) {
	bool quit = false;
	Wlezwrap wew = {0};
	wew.data = (void*)&quit;
	wew.event = f_event;
	wlezwrap_confgen(&wew);
	wlezwrap_init(&wew);
	while (!quit) {
		usleep(10000);
		wl_display_roundtrip(wew.wl.display);
	}
	wlezwrap_deinit(&wew);
}
