#ifndef AUTOCAR_AUTOACTION_H
#define AUTOCAR_AUTOACTION_H

#include <Arduino.h>

extern char* TextActions[3];

struct MoveParams {
	enum Direction { FORWARD, BACKWARD } direction;
	uint millis;

	MoveParams(Direction direction, uint millis) : direction(direction), millis(millis){}

	static MoveParams* fromVoidPtr(void* params){
		return static_cast<MoveParams*>(params);
	}
};

struct TurnParams {
	enum Direction { LEFT, RIGHT } direction;
	uint millis;

	TurnParams(Direction direction, uint millis) : direction(direction), millis(millis){}

	static TurnParams* fromVoidPtr(void* params){
		return static_cast<TurnParams*>(params);
	}
};

struct LightsParams {
	enum State { OFF, ON } state;

	explicit LightsParams(State state) : state(state){}

	static LightsParams* fromVoidPtr(void* params){
		return static_cast<LightsParams*>(params);
	}
};

struct TuneParams {
	uint tune;
	uint8_t volume;

	TuneParams(uint tune, uint8_t volume) : tune(tune), volume(volume){ }

	static TuneParams* fromVoidPtr(void* params){
		return static_cast<TuneParams*>(params);
	}
};

struct ToneParams {
	uint frequency;
	uint8_t volume;
	uint millis;

	ToneParams(uint frequency, uint8_t volume, uint millis) : frequency(frequency), volume(volume), millis(millis){ }

	static ToneParams* fromVoidPtr(void* params){
		return static_cast<ToneParams*>(params);
	}
};

struct AutoAction {
	enum Type { MOVE, TURN, LIGHTS, TUNE, TONE } type;
	void* params;
};

#endif //AUTOCAR_AUTOACTION_H
