// TODO: buffer
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <wayland-client.h>

#include "../include/wlezwrap.h"

static void f_quit(void* data) {
	*(bool*)data = true;
}

int main(void) {
	bool quit = false;
	Wlezwrap wew = {0};
	wew.data = (void*)&quit;
	wew.f_quit = f_quit;
	wlezwrap_confgen(&wew);
	wlezwrap_init(&wew);
	while (!quit) {
		usleep(10000);
		wl_display_roundtrip(wew.wl.display);
	}
	wlezwrap_deinit(&wew);
}
