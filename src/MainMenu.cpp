#include "defs.hpp"
#include "MainMenu.h"
#include "Apps/Timeline/Timeline.h"

MainMenu* MainMenu::instance = nullptr;

MainMenu::MainMenu(Display& display) : Context(display), appMenu(&screen, 2){

	instance = this;

	menuItems.push_back({ "Simple", new Timeline(display), new Image(&appMenu, 35, 35) });
	menuItems.push_back({ "Autonomous", new AutonomousApp(display), new Image(&appMenu, 35, 35)});

	for(auto& item : menuItems){
		addSprite(item.image);
	}

	buildUI();
	pack();
}

void MainMenu::start(){
	Input::getInstance()->setBtnPressCallback(BTN_A, [](){
		if(instance == nullptr) return;

		Context* app = instance->menuItems[instance->appMenu.getSelected()].context;
		app->push(instance);
	});

	Input::getInstance()->setBtnPressCallback(BTN_LEFT, [](){
		if(instance == nullptr) return;
		Serial.println("LEFT PRESS");
		delay(5);
		instance->appMenu.selectPrev();
		instance->getScreen().commit();
		// instance->draw();
	});

	Input::getInstance()->setBtnPressCallback(BTN_RIGHT, [](){
		if(instance == nullptr) return;
		Serial.println("RIGHT PRESS");
		delay(5);
		instance->appMenu.selectNext();
		instance->getScreen().commit();
	});

	draw();
}

void MainMenu::stop(){
	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnPressCallback(BTN_LEFT);
	Input::getInstance()->removeBtnPressCallback(BTN_RIGHT);
}

void MainMenu::unpack(){
	Context::unpack();

	for(auto& item : menuItems){
		item.image->getSprite()->clear(TFT_GREEN);
	}
}

void MainMenu::draw(){
	screen.draw();
	screen.commit();
}

void MainMenu::fillMenu(){
	for(auto& item : menuItems){
		appMenu.addItem({ item.title.data(), item.image });
	}
}

void MainMenu::buildUI(){
	fillMenu();

	appMenu.setWHType(PARENT, PARENT);
	appMenu.setTitleColor(TFT_NAVY, TFT_WHITE);
	appMenu.reflow();

	screen.addChild(&appMenu);
	screen.repos();
}
