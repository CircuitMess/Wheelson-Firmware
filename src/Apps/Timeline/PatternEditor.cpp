#include <Support/ContextTransition.h>
#include <Util/Debug.h>
#include "PatternEditor.h"
#include "../../defs.hpp"

#include "Bitmaps/arrow_down.hpp"
#include "Bitmaps/arrow_up.hpp"
#include "Bitmaps/arrow_left.hpp"
#include "Bitmaps/arrow_right.hpp"
#include "Bitmaps/add.hpp"
#include "Bitmaps/tune.hpp"
#include "Bitmaps/tone.hpp"
#include "Bitmaps/light_off.hpp"
#include "Bitmaps/light_on.hpp"
#include "Bitmaps/color.hpp"
#include "Bitmaps/border.hpp"

const uint16_t* ActionSprites[] = {
		arrow_up, arrow_down, arrow_left, arrow_right, light_on, light_off, tone, tune
};



PatternEditor* PatternEditor::instance = nullptr;

PatternEditor::PatternEditor(Display& display) : Context(display), fleha(&screen, display.getWidth(), display.getHeight()){
	instance = this;

	ActionSelector a(display);

	buildUI();
	pack();
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
}

void PatternEditor::draw(){
	screen.clear();
	screen.draw();
	drawTimeline();
	screen.commit();
}

void PatternEditor::start(){
	Input::getInstance()->setBtnPressCallback(BTN_A, [](){
		if(instance == nullptr) return;
		instance->pop();
	});

	Input::getInstance()->setBtnPressCallback(BTN_B, [](){
		if(instance == nullptr) return;

	});

	Input::getInstance()->setBtnPressCallback(BTN_C, [](){
		if(instance == nullptr) return;

		if(instance->selectedAction == 0){
			instance->selectedAction = instance->actions.size();
		}else{
			instance->selectedAction--;
		}

		instance->draw();
	});

	Input::getInstance()->setBtnPressCallback(BTN_D, [](){
		if(instance == nullptr) return;

		instance->selectedAction = (instance->selectedAction + 1) % (instance->actions.size() + 1);
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

void PatternEditor::drawTimeline(){
	Sprite* canvas = screen.getSprite();

	const uint margin = 5;
	const uint x = 10;
	int y = 10;
	uint i = 0;

	for(const auto& action : actions){
		canvas->drawIcon(ActionSprites[action.type], x, y, 18, 18, 1, TFT_TRANSPARENT);
		if(i == selectedAction){
			canvas->drawIcon(border, x, y, 18, 18, 1, TFT_TRANSPARENT);
		}

		y += 18 + margin;
		i++;
	}

	canvas->drawIcon(add, x, y, 18, 18, 1, TFT_TRANSPARENT);
	if(i == selectedAction){
		canvas->drawIcon(border, x, y, 18, 18, 1, TFT_TRANSPARENT);
	}
}

void PatternEditor::buildUI(){
	addAction(AutoAction::FORWARD);
	addAction(AutoAction::LEFT);
	addAction(AutoAction::FORWARD);
	addAction(AutoAction::RIGHT);
	addAction(AutoAction::LIGHT_ON);
	addAction(AutoAction::BACKWARD);

	screen.addChild(&fleha);
}

