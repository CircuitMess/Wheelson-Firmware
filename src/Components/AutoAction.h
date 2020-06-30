#ifndef AUTOCAR_AUTOACTION_H
#define AUTOCAR_AUTOACTION_H

#include <Arduino.h>
#include <Display/Color.h>

extern const char* ActionText[];

struct DurationActionParams {
	int millis = 3000;
};

struct MoveParams : DurationActionParams {

};

struct LightParams {
	int color = 0;
};

struct ToneParams : DurationActionParams {
	int frequency = 2000;
	int volume = 10;
};

struct TuneParams {
	int tune = 0;
	int volume = 10;
};

struct AutoAction {
	enum Type { FORWARD, BACKWARD, LEFT, RIGHT, LIGHT_ON, LIGHT_OFF, TONE, TUNE } type;
	void* params;
};

#endif //AUTOCAR_AUTOACTION_H
