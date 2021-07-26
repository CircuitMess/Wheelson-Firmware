#ifndef WHEELSON_FIRMWARE_ACTION_HPP
#define WHEELSON_FIRMWARE_ACTION_HPP

#include <Arduino.h>

namespace Simple {

struct Action {
	enum Type {
		FORWARD, BACKWARD, LEFT, RIGHT, LED_ON, LED_OFF, PAUSE, COUNT
	} type;
	float time;
	int8_t speed;
};

}

#endif //WHEELSON_FIRMWARE_ACTION_HPP
