#include "ActionSelector.h"
#include "../../defs.hpp"
#include "PatternEditor.h"
#include "Bitmaps/actions.hpp"

const AutoAction::Type types[] = {
		AutoAction::Type::FORWARD, AutoAction::Type::BACKWARD, AutoAction::Type::LEFT, AutoAction::Type::RIGHT,
		AutoAction::Type::LIGHT_ON, AutoAction::Type::LIGHT_OFF, AutoAction::Type::TONE, AutoAction::Type::TUNE
};

const uint16_t* SelectorActionSprites[] = {
		arrow_up, arrow_down, arrow_left, arrow_right, light_on, light_off, tone, tune
};

ActionSelector* ActionSelector::instance = nullptr;

ActionSelector::ActionSelector(PatternEditor* editor) : editor(editor), Modal(*editor, 74, 74),
		layers(&screen), fleha(&layers, 74, 74), actionGrid(&layers, 3),
		selectedBorder(&layers, border, 18, 18){

	instance = this;
	buildUI();
}

void ActionSelector::draw(){
	getScreen().draw();
	getScreen().commit();
}

void ActionSelector::selectAction(){
	Element* selected = actionGrid.getChild(selectedAction);
	selectedBorder.setPos(selected->getX(), selected->getY());
}

void ActionSelector::start(){
	Input::getInstance()->setBtnPressCallback(BTN_A, [](){
		if(instance == nullptr) return;
		instance->pop(new int(-1));
	});

	Input::getInstance()->setBtnPressCallback(BTN_B, [](){
		if(instance == nullptr) return;
		instance->pop(new int(instance->selectedAction));
	});

	Input::getInstance()->setBtnPressCallback(BTN_C, [](){
		if(instance == nullptr) return;

		if(instance->selectedAction == 0){
			instance->selectedAction = sizeof(types) / sizeof(AutoAction::type) - 1;
		}else{
			instance->selectedAction--;
		}

		instance->selectAction();
		instance->draw();
	});

	Input::getInstance()->setBtnPressCallback(BTN_D, [](){
		if(instance == nullptr) return;

		instance->selectedAction = (instance->selectedAction + 1) % (sizeof(types) / sizeof(AutoAction::type));
		instance->selectAction();
		instance->draw();
	});

	draw();
}

void ActionSelector::stop(){
	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnPressCallback(BTN_B);
	Input::getInstance()->removeBtnPressCallback(BTN_C);
	Input::getInstance()->removeBtnPressCallback(BTN_D);
}

void ActionSelector::unpack(){
	Context::unpack();
	selectedAction = 0;
	selectAction();
}


void ActionSelector::fillMenu(){
	for(const auto& type : types){
		actionGrid.addChild(new BitmapElement(&actionGrid, SelectorActionSprites[type], 18, 18));
	}
}

void ActionSelector::buildUI(){
	fillMenu();

	layers.setWHType(FIXED, FIXED);
	layers.setWidth(74);
	layers.setHeight(74);

	layers.addChild(&fleha);
	layers.addChild(&actionGrid);
	layers.addChild(&selectedBorder);
	layers.reflow();

	actionGrid.setWHType(PARENT, PARENT);
	actionGrid.setPadding(5);
	actionGrid.setGutter(5);
	actionGrid.reflow();
	actionGrid.repos();

	screen.addChild(&layers);
}
