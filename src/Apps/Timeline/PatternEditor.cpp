#include <Support/ContextTransition.h>
#include <Util/Debug.h>
#include "PatternEditor.h"
#include "../../defs.hpp"

#include "Bitmaps/actions.hpp"
#include "Bitmaps/add.hpp"
#include "../../Elements/ActionItem.h"

const uint16_t* ActionSprites[] = {
		arrow_up, arrow_down, arrow_left, arrow_right, light_on, light_off, tone, tune
};

const char* ActionText[] = {
		"Drive forward", "Drive backward", "Turn left", "Turn right", "Lights ON", "Lights OFF", "Honk tone", "Play tune"
};



PatternEditor* PatternEditor::instance = nullptr;

PatternEditor::PatternEditor(Display& display) : Context(display),
		layers(&screen), fleha(&layers, display.getWidth(), display.getHeight()),
		scroll(&layers), timelineList(&scroll, VERTICAL),
		selector(this), aEditor(this){

	instance = this;

	buildUI();
	pack();
}

void PatternEditor::returned(void* data){
	int* type = static_cast<int*>(data);

	if(*type != -1){
		addAction(static_cast<AutoAction::Type>(*type));
		instance->timelineList.reflow();
		instance->timelineList.getChildren().relocate(instance->actions.size(), instance->actions.size()-1);
		instance->timelineList.repos();
		instance->selectedAction++;
		instance->scroll.scrollIntoView(instance->timelineList.getChildren().size() - 1, 5);
	}

	delete type;
}

void PatternEditor::addAction(AutoAction::Type type){
	AutoAction action = { type, nullptr };

	switch(type){
		case AutoAction::Type::FORWARD:
		case AutoAction::Type::BACKWARD:
		case AutoAction::Type::LEFT:
		case AutoAction::Type::RIGHT:
			action.params = new MoveParams();
			break;

		case AutoAction::Type::LIGHT_ON:
			action.params = new LightParams();
			break;

		case AutoAction::Type::TONE:
			action.params = new ToneParams();
			break;

		case AutoAction::Type::TUNE:
			action.params = new TuneParams();
			break;

		default:
			logln("Invalid AutoAction type passed to PatternEditor::addAction");
			return;
	}

	actions.push_back(action);
	timelineList.addChild(new ActionItem(&timelineList, ActionSprites[action.type], ActionText[action.type]));
}

void PatternEditor::draw(){
	screen.draw();
	screen.commit();
}

void PatternEditor::start(){
	Input::getInstance()->setBtnPressCallback(BTN_A, [](){
		if(instance == nullptr) return;
		instance->pop();
	});

	Input::getInstance()->setBtnPressCallback(BTN_B, [](){
		if(instance == nullptr) return;

		if(instance->selectedAction == instance->actions.size()){
			instance->selector.push(instance);
		}else{
			instance->aEditor.push(instance);
		}
	});

	Input::getInstance()->setBtnPressCallback(BTN_C, [](){
		if(instance == nullptr) return;

		reinterpret_cast<ActionItem*>(instance->timelineList.getChildren()[instance->selectedAction])->setSelected(false);
		if(instance->selectedAction == 0){
			instance->selectedAction = instance->timelineList.getChildren().size() - 1;
		}else{
			instance->selectedAction--;
		}
		reinterpret_cast<ActionItem*>(instance->timelineList.getChildren()[instance->selectedAction])->setSelected(true);

		instance->scroll.scrollIntoView(instance->selectedAction, 5);
		instance->draw();
	});

	Input::getInstance()->setBtnPressCallback(BTN_D, [](){
		if(instance == nullptr) return;

		reinterpret_cast<ActionItem*>(instance->timelineList.getChildren()[instance->selectedAction])->setSelected(false);
		instance->selectedAction = (instance->selectedAction + 1) % (instance->timelineList.getChildren().size());
		reinterpret_cast<ActionItem*>(instance->timelineList.getChildren()[instance->selectedAction])->setSelected(true);

		instance->scroll.scrollIntoView(instance->selectedAction, 5);
		instance->draw();
	});

	draw();
}

void PatternEditor::stop(){
	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnPressCallback(BTN_B);
	Input::getInstance()->removeBtnPressCallback(BTN_C);
	Input::getInstance()->removeBtnPressCallback(BTN_D);
}

void PatternEditor::buildUI(){
	layers.setWHType(PARENT, PARENT);
	layers.addChild(&fleha);
	layers.addChild(&scroll);
	layers.reflow();

	scroll.setWHType(PARENT, PARENT);
	scroll.addChild(&timelineList);
	scroll.reflow();

	timelineList.setWHType(PARENT, CHILDREN);
	timelineList.setPadding(5);
	timelineList.setGutter(5);
	timelineList.reflow();

	addAction(AutoAction::FORWARD);
	addAction(AutoAction::LIGHT_ON);
	addAction(AutoAction::TUNE);
	addAction(AutoAction::BACKWARD);
	reinterpret_cast<ActionItem*>(instance->timelineList.getChildren()[0])->setSelected(true);
	timelineList.addChild(new ActionItem(&timelineList, add, "New action"));
	timelineList.reflow();
	timelineList.repos();

	selector.setPos(27, 43);
	// selector.setBorder(1, C_HEX(0x00ffff));

	screen.addChild(&layers);
}

