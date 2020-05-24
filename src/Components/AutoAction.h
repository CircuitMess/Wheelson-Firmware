#ifndef AUTOCAR_AUTOACTION_H
#define AUTOCAR_AUTOACTION_H

#include <Arduino.h>

extern char* TextActions[3];

struct MoveParams {
	enum Direction { FORWARD, BACKWARD } direction;
	uint millis;

	MoveParams(Direction direction, uint millis) : direction(direction), millis(millis){}
};

struct TurnParams {
	enum Direction { LEFT, RIGHT } direction;
	uint millis;

	TurnParams(Direction direction, uint millis) : direction(direction), millis(millis){}
};

struct LightsParams {
	enum State { OFF, ON } state;

	explicit LightsParams(State state) : state(state){}
};

struct AutoAction {
	enum Type { MOVE, TURN, LIGHTS } type;
	void* params;
};

#endif //AUTOCAR_AUTOACTION_H
