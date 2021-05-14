#ifndef WHEELSON_FIRMWARE_PROGPLAYER_H
#define WHEELSON_FIRMWARE_PROGPLAYER_H

#include "Action.hpp"
#include <Loop/LoopListener.h>

namespace Simple {

class ProgPlayer : LoopListener {
public:
	ProgPlayer(Action* actions, uint8_t numActions);
	uint8_t getCurrent();
	bool isDone();
	void start();
	void stop();
	void loop(uint) override;

private:
	void processAction();

	Action* actions = nullptr;
	uint8_t numActions = 0;
	uint8_t currentAction = 0;
	uint32_t actionStartTime = 0;
	bool done = false;

};

}
#endif //WHEELSON_FIRMWARE_PROGPLAYER_H