#include <Util/Debug.h>
#include "ActionProcessor.h"
#include "AutoAction.h"

ActionProcessor* ActionProcessor::instance = nullptr;

ActionProcessor::ActionProcessor() : actionTask("ActionProcessorTask", ActionProcessor::actionProcessorTask),
		actionQueue(20, sizeof(AutoAction*)){

	Serial.printf("Sizeof AutoAction*: %x\n", sizeof(AutoAction*));
	instance = this;
}

void ActionProcessor::start(){
	actionTask.start(8 | portPRIVILEGE_BIT);
}

void ActionProcessor::stop(){
	actionTask.stop();
}

void ActionProcessor::addAction(AutoAction* action){
	Serial.printf("Adding addr %x\n", action);
	actionQueue.send(&action);
}

void ActionProcessor::actionProcessorTask(Task* task){
	while(task->running){
		if(instance->actionQueue.count() == 0) break;

		volatile AutoAction* receivedAction = nullptr;
		bool success = instance->actionQueue.receive(&receivedAction);

		if(!success || receivedAction == nullptr) continue;

		if(instance->currentAction != nullptr){
			instance->stopCurrentAction();
		}

		instance->currentAction = const_cast<AutoAction*>(receivedAction);
		instance->processAction();
	}

	if(instance->doneListener != nullptr){
		instance->doneListener();
	}
}

void ActionProcessor::processAction(){
	if(actionListener != nullptr){
		actionListener();
	}

	switch(currentAction->type){
		case AutoAction::Type::FORWARD: {
			MoveParams* params = static_cast<MoveParams*>(currentAction->params);
			Motors::getInstance()->leftMotor(MOTOR_FORWARD);
			Motors::getInstance()->rightMotor(MOTOR_FORWARD);
			if(params->millis != 0){
				delay(params->millis);
				stopCurrentAction();
			}
			break;
		}

		case AutoAction::Type::BACKWARD: {
			MoveParams* params = static_cast<MoveParams*>(currentAction->params);
			Motors::getInstance()->leftMotor(MOTOR_BACKWARD);
			Motors::getInstance()->rightMotor(MOTOR_BACKWARD);
			if(params->millis != 0){
				delay(params->millis);
				stopCurrentAction();
			}
			break;
		}

		case AutoAction::Type::LEFT: {
			MoveParams* params = static_cast<MoveParams*>(currentAction->params);
			Motors::getInstance()->leftMotor(MOTOR_BACKWARD);
			Motors::getInstance()->rightMotor(MOTOR_FORWARD);
			if(params->millis != 0){
				delay(params->millis);
				stopCurrentAction();
			}
			break;
		}

		case AutoAction::Type::RIGHT: {
			MoveParams* params = static_cast<MoveParams*>(currentAction->params);
			Motors::getInstance()->leftMotor(MOTOR_FORWARD);
			Motors::getInstance()->rightMotor(MOTOR_BACKWARD);
			if(params->millis != 0){
				delay(params->millis);
				stopCurrentAction();
			}
			break;
		}

		case AutoAction::Type::LIGHT_ON: {
			LightParams* params = static_cast<LightParams*>(currentAction->params);

			break;
		}

		case AutoAction::Type::LIGHT_OFF: {

			break;
		}

		case AutoAction::Type::TONE: {
			ToneParams* params = static_cast<ToneParams*>(currentAction->params);
			break;
		}

		case AutoAction::Type::TUNE: {
			TuneParams* params = static_cast<TuneParams*>(currentAction->params);
			break;
		}

		default:
			logln("ActionProcessor received unrecognized task");
	}
}

void ActionProcessor::stopCurrentAction(){
	if(currentAction == nullptr) return;

	switch(currentAction->type){
		case AutoAction::Type::FORWARD:
		case AutoAction::Type::BACKWARD:
		case AutoAction::Type::LEFT:
		case AutoAction::Type::RIGHT:
			Motors::getInstance()->stopAll();
			break;

		default:
			logln("ActionProcessor stopAction received unrecognized task");
			break;
	}

	if(currentAction->params != nullptr){
		delete currentAction->params;
	}
	delete currentAction;
	currentAction = nullptr;

}

ActionProcessor* ActionProcessor::getInstance(){
	return instance;
}

AutoAction* ActionProcessor::getCurrentAction() const{
	return currentAction;
}

void ActionProcessor::setActionListener(void (* actionListener)()){
	ActionProcessor::actionListener = actionListener;
}

void ActionProcessor::setDoneListener(void (* doneListener)()){
	ActionProcessor::doneListener = doneListener;
}

uint ActionProcessor::count(){
	return actionQueue.count();
}
