#include <Support/ContextTransition.h>
#include "Timeline.h"
#include "../../defs.hpp"

Timeline* Timeline::instance = nullptr;

Timeline::Timeline(Display& display) : Context(display), editor(new PatternEditor(display)),
									   patternMenu(&screen, "Patterns"){

	instance = this;

	buildUI();
	pack();
}

void Timeline::draw(){
	screen.draw();
	screen.commit();
}

void Timeline::start(){
	Input::getInstance()->setBtnPressCallback(BTN_A, [](){
		if(instance == nullptr) return;
		instance->pop();
	});

	Input::getInstance()->setBtnPressCallback(BTN_B, [](){
		if(instance == nullptr) return;
		instance->editor->push(instance);
	});

	draw();
}

void Timeline::stop(){
	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnPressCallback(BTN_B);
}

void Timeline::fillMenu(){
	patternMenu.addItem("New");
}

void Timeline::buildUI(){
	fillMenu();

	patternMenu.setWHType(PARENT, PARENT);
	patternMenu.reflow();
	patternMenu.repos();

	screen.addChild(&patternMenu);
	screen.repos();
}

