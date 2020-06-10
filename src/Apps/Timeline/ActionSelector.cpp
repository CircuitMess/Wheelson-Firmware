#include "ActionSelector.h"
#include "../../defs.hpp"

ActionSelector* ActionSelector::instance = nullptr;

ActionSelector::ActionSelector(Display& display) : Context(display), menu(&screen, 2),
		driveImage(&menu, 50, 50), turnImage(&menu, 50, 50), lightImage(&menu, 108, 35) {

	instance = this;

	addSprite(&driveImage);
	addSprite(&turnImage);
	addSprite(&lightImage);

	buildUI();
	pack();
}

void ActionSelector::draw(){
	screen.draw();
	screen.commit();
}

void ActionSelector::start(){
	Input::getInstance()->setBtnPressCallback(BTN_A, [](){
		if(instance == nullptr) return;
		instance->pop();
	});

	Input::getInstance()->setBtnPressCallback(BTN_B, [](){
		if(instance == nullptr) return;
		int* data = new int(instance->menu.getSelected());
		instance->pop(data);
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

void ActionSelector::stop(){
	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnPressCallback(BTN_B);
	Input::getInstance()->removeBtnPressCallback(BTN_C);
	Input::getInstance()->removeBtnPressCallback(BTN_D);
}

void ActionSelector::unpack(){
	Context::unpack();

	driveImage.getSprite()->clear(TFT_GREEN);
	turnImage.getSprite()->clear(TFT_GREEN);
	lightImage.getSprite()->clear(TFT_WHITE);

	driveImage.getSprite()->fillRect(21, 21, 8, 8, TFT_BLACK);
	turnImage.getSprite()->fillRect(19, 19, 12, 12, TFT_BLACK);

	lightImage.getSprite()->fillRect(0, 17, 108, 3, TFT_BLACK);
	for(int i = 11; i < 100; i += 12){
		lightImage.getSprite()->fillRect(i, 0, 1, 35, TFT_BLACK);
	}
}

void ActionSelector::fillMenu(){
	menu.addItem({ "Drive", &driveImage });
	menu.addItem({ "Turn", &turnImage });
	menu.addItem({ "Lights", &lightImage });
}

void ActionSelector::buildUI(){
	fillMenu();

	// Don't do this, please. Should implement a function in GridMenu and ListMenu to return container element.
	Layout* container = static_cast<Layout*>(menu.getChild(1));
	container = static_cast<Layout*>(container->getChild(0));

	container->setPadding(10);
	container->setGutter(8);

	menu.setWHType(PARENT, PARENT);
	menu.reflow();
	menu.repos();

	screen.addChild(&menu);
}
