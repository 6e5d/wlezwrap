// TODO: buffer
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <wayland-client.h>

#include "../include/wlezwrap.h"

static bool quit = false;

static void f_quit(void* data) {
	quit = true;
}

int main(void) {
	Wlezwrap wew = {0};
	wew.f_quit = f_quit;
	wlezwrap_confgen(&wew);
	wlezwrap_init(&wew);
	while (!quit) {
		usleep(10000);
		wl_display_roundtrip(wew.wl.display);
	}
	wlezwrap_deinit(&wew);
}
