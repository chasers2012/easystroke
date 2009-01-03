/*
 * Copyright (c) 2008, Thomas Jaeger <ThJaeger@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#ifndef __GRABBER_H__
#define __GRABBER_H__
#include <gtkmm.h>
#include <string>
#include <map>
#include <X11/Xlib.h>
#include <X11/extensions/XInput.h>
#include <X11/cursorfont.h>

class Grabber;
extern Grabber *grabber;

float rescaleValuatorAxis(int coord, int fmin, int fmax, int tmax);

class Grabber {
public:
	enum State { NONE, BUTTON };
	static const char *state_name[6];
	enum EventType { DOWN = 0, UP = 1, MOTION = 2, BUTTON_MOTION = 3, PROX_IN = 4, PROX_OUT = 5 };
	bool xinput;
	bool is_event(int, EventType);

	struct XiDevice {
		std::string name;
		XDevice *dev;
		XEventClass events[6];
		int event_type[6];
		int all_events_n;
		int min_x, max_x, min_y, max_y;
		bool absolute;
		void fake_press(int b, int core);
		void fake_release(int b, int core);
	};

	XiDevice *xi_dev;

	unsigned int get_device_button_state(XiDevice *&dev);
	XiDevice *get_xi_dev(XID id);

	int nMajor;
private:
	int button_events_n;
	bool init_xi();

	State current, grabbed;
	bool xi_grabbed;
	bool xi_devs_grabbed;
	Cursor cursor_select;

	void set();
	void grab_xi(bool);
	void grab_xi_devs(bool);
	std::string get_wm_class(Window w);
	std::string wm_class;
public:
	Grabber();
	~Grabber();
	std::string get_wm_class() { return wm_class; }

	void fake_button(int b);
	void grab(State s) { current = s; set(); }
	bool is_grabbed(guint b);
	bool update_device_list();

	void unminimize();
};

class GrabFailedException : public std::exception {
	virtual const char* what() const throw();
};

#endif
