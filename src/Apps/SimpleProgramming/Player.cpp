#include "Player.h"
#include <Loop/LoopManager.h>
#include <Wheelson.h>

Simple::Player::Player(Simple::Action *actions, uint8_t numActions) : actions(actions), numActions(numActions){

}

uint8_t Simple::Player::getCurrent(){
	return currentAction;
}

bool Simple::Player::isDone(){
	return done;
}

void Simple::Player::start(){
	LoopManager::addListener(this);
	currentAction = 0;
	actionStartTime = millis();
	processAction();
	done = false;
}

void Simple::Player::stop(){
	LoopManager::removeListener(this);
}

void Simple::Player::processAction(){
	if(currentAction == numActions) return;

	switch(actions[currentAction].type){
		case Action::FORWARD:
			Motors.setMotor(MOTOR_FR, actions[currentAction].speed);
			Motors.setMotor(MOTOR_FL, actions[currentAction].speed);
			Motors.setMotor(MOTOR_BR, actions[currentAction].speed);
			Motors.setMotor(MOTOR_BL, actions[currentAction].speed);
			break;
		case Action::BACKWARD:
			Motors.setMotor(MOTOR_FR, -actions[currentAction].speed);
			Motors.setMotor(MOTOR_FL, -actions[currentAction].speed);
			Motors.setMotor(MOTOR_BR, -actions[currentAction].speed);
			Motors.setMotor(MOTOR_BL, -actions[currentAction].speed);
			break;
		case Action::LEFT:
			Motors.setMotor(MOTOR_FR, actions[currentAction].speed);
			Motors.setMotor(MOTOR_FL, -actions[currentAction].speed);
			Motors.setMotor(MOTOR_BR, actions[currentAction].speed);
			Motors.setMotor(MOTOR_BL, -actions[currentAction].speed);
			break;
		case Action::RIGHT:
			Motors.setMotor(MOTOR_FR, -actions[currentAction].speed);
			Motors.setMotor(MOTOR_FL, actions[currentAction].speed);
			Motors.setMotor(MOTOR_BR, -actions[currentAction].speed);
			Motors.setMotor(MOTOR_BL, actions[currentAction].speed);
			break;
		case Action::LED_ON:
			LED.setHeadlight(255);
			break;
		case Action::LED_OFF:
			LED.setHeadlight(0);
			break;
		case Action::PAUSE:
			break;

	}


}

void Simple::Player::loop(uint){
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
