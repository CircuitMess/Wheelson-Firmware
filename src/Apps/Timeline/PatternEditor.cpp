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

PatternEditor* PatternEditor::instance = nullptr;

PatternEditor::PatternEditor(Display& display) : Context(display){
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
	fillMenu();
}

void PatternEditor::draw(){
	screen.clear();
	screen.draw();
	screen.getSprite()->drawIcon(arrow_up, 30, 10, 18, 18, 1, TFT_TRANSPARENT);
	screen.getSprite()->drawIcon(arrow_down, 30, 30, 18, 18, 1, TFT_TRANSPARENT);
	screen.getSprite()->drawIcon(arrow_left, 30, 50, 18, 18, 1, TFT_TRANSPARENT);
	screen.getSprite()->drawIcon(arrow_right, 30, 70, 18, 18, 1, TFT_TRANSPARENT);
	screen.getSprite()->drawIcon(light_on, 30, 90, 18, 18, 1, TFT_TRANSPARENT);
	screen.getSprite()->drawIcon(light_off, 50, 90, 18, 18, 1, TFT_TRANSPARENT);
	screen.getSprite()->drawIcon(tone, 50, 70, 18, 18, 1, TFT_TRANSPARENT);
	screen.getSprite()->drawIcon(tune, 50, 50, 18, 18, 1, TFT_TRANSPARENT);
	screen.getSprite()->drawIcon(add, 50, 30, 18, 18, 1, TFT_TRANSPARENT);
	screen.getSprite()->drawIcon(color, 70, 10, 18, 18, 2, TFT_TRANSPARENT);
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

	});

	Input::getInstance()->setBtnPressCallback(BTN_D, [](){
		if(instance == nullptr) return;

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

}

void PatternEditor::buildUI(){
	fillMenu();
}

