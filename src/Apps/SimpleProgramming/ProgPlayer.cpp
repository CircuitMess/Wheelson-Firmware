#include "ProgPlayer.h"
#include <Loop/LoopManager.h>

Simple::ProgPlayer::ProgPlayer(Simple::Action *actions, uint8_t numActions) : actions(actions), numActions(numActions){

}

uint8_t Simple::ProgPlayer::getCurrent(){
	return currentAction;
}

bool Simple::ProgPlayer::isDone(){
	return done;
}

void Simple::ProgPlayer::start(){
	LoopManager::addListener(this);
	currentAction = 0;
	actionStartTime = millis();
	processAction();
	done = false;
}

void Simple::ProgPlayer::stop(){
	LoopManager::removeListener(this);
}

void Simple::ProgPlayer::processAction(){
	if(currentAction == numActions) return;

}

void Simple::ProgPlayer::loop(uint){
	if(millis() - actionStartTime >= actions[currentAction].time){
		if(currentAction == numActions - 1){
			done = true;
			stop();
			return;
		}
		currentAction++;
		actionStartTime = millis();
		processAction();
	}
}
