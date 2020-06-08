#include <Support/ContextTransition.h>
#include <sstream>
#include "Timeline.h"
#include "../../defs.hpp"

Timeline* Timeline::instance = nullptr;

Timeline::Timeline(Display& display) : Context(display), editor(new PatternEditor(display)),
									   menu(&screen, "Patterns"){

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

		if(instance->menu.getSelected() == instance->patterns.size()){
			instance->patterns.emplace_back();
			instance->editor->initPattern(&instance->patterns.back());
		}else{
			instance->editor->initPattern(&instance->patterns[instance->menu.getSelected()]);
		}

		instance->editor->push(instance);
	});

	Input::getInstance()->setBtnPressCallback(BTN_C, [](){
		if(instance == nullptr) return;
		instance->menu.selectPrev();
		instance->screen.commit();
	});

	Input::getInstance()->setBtnPressCallback(BTN_D, [](){
		if(instance == nullptr) return;
		instance->menu.selectNext();
		instance->screen.commit();
	});

	draw();
}

void Timeline::unpack(){
	Context::unpack();
	fillMenu();
}

void Timeline::stop(){
	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnPressCallback(BTN_B);
	Input::getInstance()->removeBtnPressCallback(BTN_C);
	Input::getInstance()->removeBtnPressCallback(BTN_D);
}

void Timeline::fillMenu(){
	menu.clearItems();

	int i = 0;
	std::ostringstream buffer;
	for(const auto& pattern : patterns){
		buffer.str(std::string());
		buffer << "Pattern " << ++i;
		std::string str = buffer.str();

		char* title = static_cast<char*>(malloc(str.size() + 1));
		memset(title, 0, str.size() + 1);
		memcpy(title, str.c_str(), str.size());
		menu.addItem(title);
	}

	menu.addItem("New");
	menu.reflow();
	menu.repos();
}

void Timeline::buildUI(){
	menu.setWHType(PARENT, PARENT);
	fillMenu();

	screen.addChild(&menu);
	screen.repos();
}

