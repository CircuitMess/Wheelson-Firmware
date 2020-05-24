#include <Support/ContextTransition.h>
#include <Util/Debug.h>
#include "PatternEditor.h"
#include "../../defs.hpp"

PatternEditor* PatternEditor::instance = nullptr;

PatternEditor::PatternEditor(Display& display) : Context(display), actionList(&screen, "Actions"){
	instance = this;
	actionSelector = new ActionSelector(display);

	buildUI();
	pack();
}

void PatternEditor::returned(void* data){
	int act = *static_cast<int*>(data);
	delete data;

	AutoAction action = { static_cast<AutoAction::Type>(act) };

	switch(action.type){
		case AutoAction::Type::MOVE:
			action.params = new MoveParams(MoveParams::Direction::FORWARD, 1000);
			break;

		case AutoAction::Type::TURN:
			action.params = new TurnParams(TurnParams::Direction::LEFT, 1000);
			break;

		case AutoAction::Type::LIGHTS:
			action.params = new LightsParams(LightsParams::State::ON);
			break;

		default:
			logln("Invalid AutoAction type passed from ActionSelector");
			return;
	}

	actions.push_back(action);
	fillMenu();
}

void PatternEditor::draw(){
	screen.draw();
	screen.commit();
}

const int moves[3][2] = {
		{ 68, 100 },
		{ 68, 100 },
		{ 100, 10 }
};

void PatternEditor::redrawSelected(){
	ListMenu::ListMenuItem& item = actionList.getSelectedItem();
	AutoAction& action = actions[actionList.getSelected()];

	char* data = (char*) malloc(50);

	if(action.type == AutoAction::LIGHTS){
		sprintf(data, "%s %s", TextActions[action.type], LightsParams::fromVoidPtr(action.params)->state == LightsParams::ON ? "ON" : "OFF");
	}else if(action.type == AutoAction::MOVE){
		sprintf(data, "%s %s %.0fs", TextActions[action.type],
				MoveParams::fromVoidPtr(action.params)->direction == MoveParams::FORWARD ? "F" : "B",
				MoveParams::fromVoidPtr(action.params)->millis / 1000.0f);
	}else if(action.type == AutoAction::TURN){
		sprintf(data, "%s %s %.0fs", TextActions[action.type],
				TurnParams::fromVoidPtr(action.params)->direction == TurnParams::LEFT ? "L" : "R",
				TurnParams::fromVoidPtr(action.params)->millis / 1000.0f);
	}

	//delete item.title;
	item.title = data;
	if(editingIndex == -1) return;

	actionList.drawItem(actionList.getSelected());
	item.image->getSprite()->drawRect(0, 0, 118, 16, TFT_YELLOW);
	item.image->getSprite()->fillRect(moves[action.type][editingIndex], 14, 10, 2, TFT_CYAN);
	item.image->draw();
	screen.commit();
}

void PatternEditor::start(){
	Input::getInstance()->setBtnPressCallback(BTN_A, [](){
		if(instance == nullptr) return;
		instance->pop();
	});

	Input::getInstance()->setBtnPressCallback(BTN_B, [](){
		if(instance == nullptr) return;

		if(instance->actionList.getSelected() == instance->actions.size()){
			instance->actionSelector->push(instance);
		}else{
			instance->editingIndex = 0;
			instance->stop();

			instance->redrawSelected();

			Input::getInstance()->setBtnPressCallback(BTN_A, [](){
				instance->stop();
				instance->start();
				instance->editingIndex = -1;
				instance->actionList.draw();
				instance->screen.commit();
			});

			Input::getInstance()->setBtnPressCallback(BTN_B, [](){
				if(instance == nullptr) return;

				AutoAction& action = instance->actions[instance->actionList.getSelected()];

				if(instance->editingIndex == 0){
					LightsParams* params = LightsParams::fromVoidPtr(action.params);
					if(params->state == LightsParams::ON) params->state = LightsParams::OFF;
					else params->state = LightsParams::ON;
				}else if(instance->editingIndex == 1){
					MoveParams* params = static_cast<MoveParams*>(action.params);
					params->millis += 1000;
				}

				instance->redrawSelected();
			});

			Input::getInstance()->setBtnPressCallback(BTN_C, [](){
				if(instance == nullptr) return;

				if(instance->actions[instance->actionList.getSelected()].type == AutoAction::Type::LIGHTS) return;

				instance->editingIndex = (instance->editingIndex+1) % 2;
				instance->redrawSelected();
			});

			Input::getInstance()->setBtnPressCallback(BTN_D, [](){
				if(instance == nullptr) return;

				if(instance->actions[instance->actionList.getSelected()].type == AutoAction::Type::LIGHTS) return;

				if(instance->editingIndex == 0) instance->editingIndex = 1;
				else instance->editingIndex = 0;
				instance->redrawSelected();
			});
		}
	});

	Input::getInstance()->setBtnPressCallback(BTN_C, [](){
		if(instance == nullptr) return;
		instance->actionList.selectPrev();
		instance->screen.commit();
	});

	Input::getInstance()->setBtnPressCallback(BTN_D, [](){
		if(instance == nullptr) return;
		instance->actionList.selectNext();
		instance->screen.commit();
	});

	draw();
}

void PatternEditor::stop(){
	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnPressCallback(BTN_B);
	Input::getInstance()->removeBtnPressCallback(BTN_C);
	Input::getInstance()->removeBtnPressCallback(BTN_D);
}

void PatternEditor::fillMenu(){
	if(actions.empty()){
		actionList.addItem("+ Add");
	}else{
		actionList.addItem(TextActions[actions.back().type]);
		actionList.relocate(actions.size(), actions.size()-1);
		redrawSelected();
		actionList.setSelected(actions.size());

		actionList.reflow();
		actionList.repos();
	}
}

void PatternEditor::buildUI(){
	fillMenu();

	actionList.setWHType(PARENT, PARENT);
	actionList.reflow();
	actionList.repos();

	screen.addChild(&actionList);
	screen.repos();
}

