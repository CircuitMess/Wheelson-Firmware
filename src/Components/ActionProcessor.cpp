#include <Util/Debug.h>
#include "ActionProcessor.h"
#include "AutoAction.h"

ActionProcessor* ActionProcessor::instance = nullptr;

ActionProcessor::ActionProcessor() : actionTask("ActionProcessorTask", ActionProcessor::actionProcessorTask),
		actionQueue(20, sizeof(AutoAction*)){
	instance = this;
}

void ActionProcessor::start(){
	actionTask.start();
}

void ActionProcessor::stop(){
	actionTask.stop();
}

void ActionProcessor::addAction(AutoAction* action){
	actionQueue.send(action);
}

void ActionProcessor::actionProcessorTask(Task* task){
	while(task->running){
		instance->actionQueue.receive(&instance->currentAction);

		if(instance->currentAction != nullptr){
			instance->stopCurrentAction();
		}

		instance->processAction();
	}
}

void ActionProcessor::processAction(){
	switch(currentAction->type){
		case AutoAction::Type::FORWARD: {
			MoveParams* params = static_cast<MoveParams*>(currentAction->params);
			// start motore
			if(params->millis != 0){
				delay(params->millis);
				stopCurrentAction();
			}
			break;
		}

		default:
			logln("ActionProcessor received unrecognized task");
	}
}

void ActionProcessor::stopCurrentAction(){
	if(currentAction == nullptr) return;

	switch(currentAction->type){
		case AutoAction::Type::FORWARD: {
			MoveParams* params = static_cast<MoveParams*>(currentAction->params); // ako ti trebaju parametri
			// zaustavi motore
			break;
		}

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
