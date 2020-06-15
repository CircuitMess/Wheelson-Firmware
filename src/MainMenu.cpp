#include "defs.hpp"
#include "MainMenu.h"
#include "Apps/Timeline/TimelineApp.h"

MainMenu* MainMenu::instance = nullptr;

MainMenu::MainMenu(Display& display) : Context(display), appMenu(&screen, 3){

	instance = this;

	menuItems.push_back({ "Simple", new Timeline(display), new Image(&appMenu, 40, 40) });
	menuItems.push_back({ "Autonomous", new AutonomousApp(display), new Image(&appMenu, 40, 40)});
	menuItems.push_back({ "App 2", nullptr, new Image(&appMenu, 40, 40) });
	menuItems.push_back({ "App 3", nullptr, new Image(&appMenu, 40, 40) });
	menuItems.push_back({ "App 4", nullptr, new Image(&appMenu, 40, 40) });
	menuItems.push_back({ "App 5", nullptr, new Image(&appMenu, 40, 40) });

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
		if(app == nullptr) return;
		app->push(instance);
	});

	Input::getInstance()->setBtnPressCallback(BTN_LEFT, [](){
		if(instance == nullptr) return;
		Serial.println("LEFT PRESS");
		delay(5);
		instance->appMenu.selectPrev();
		instance->screen.commit();
	});

	Input::getInstance()->setBtnPressCallback(BTN_RIGHT, [](){
		if(instance == nullptr) return;
		Serial.println("RIGHT PRESS");
		delay(5);
		instance->appMenu.selectNext();
		instance->screen.commit();
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
	appMenu.setTitleColor(TFT_GOLD, TFT_BLACK);
	appMenu.reflow();

	screen.addChild(&appMenu);
	screen.repos();
}
