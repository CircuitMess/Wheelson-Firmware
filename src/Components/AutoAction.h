#ifndef AUTOCAR_AUTOACTION_H
#define AUTOCAR_AUTOACTION_H

#include <Arduino.h>
#include <Display/Color.h>

extern char* TextActions[3];

struct DurationActionParams {
	uint millis = 3000;
};

struct MoveParams : DurationActionParams {

};

struct LightParams {
	Color color = 0xFFFF;
};

struct TuneParams {
	uint tune = 0;
	uint8_t volume = 10;
};

struct ToneParams : DurationActionParams {
	uint frequency = 2000;
	uint8_t volume = 10;
};

struct AutoAction {
	enum Type { FORWARD, BACKWARD, LEFT, RIGHT, LIGHT_ON, LIGHT_OFF, TONE, TUNE } type;
	void* params;
};

#endif //AUTOCAR_AUTOACTION_H
