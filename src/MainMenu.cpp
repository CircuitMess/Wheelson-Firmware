#include "defs.hpp"
#include "MainMenu.h"
#include "Apps/Timeline/TimelineApp.h"
#include "Bitmaps/apps/apps.hpp"
#include <Input/Input.h>

MainMenu* MainMenu::instance = nullptr;

MainMenu::MainMenu(Display& display) : Context(display), appMenu(&screen, 3){

	instance = this;

	menuItems.push_back({ "Simple programming mode", new TimelineApp(display), new BitmapElement(&appMenu, app_action, 40, 40) });
	menuItems.push_back({ "Autonomous driving mode", new AutonomousApp(display), new BitmapElement(&appMenu, app_autonomous, 40, 40) });
	menuItems.push_back({ "Object tracking", nullptr, new BitmapElement(&appMenu, app_tracking, 40, 40) });
	menuItems.push_back({ "QR tracker", nullptr, new BitmapElement(&appMenu, app_qr, 40, 40) });
	menuItems.push_back({ "Settings", nullptr, new BitmapElement(&appMenu, app_settings, 40, 40) });

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
	appMenu.setTitleColor(TFT_RED, TFT_BLACK);
	appMenu.reflow();

	screen.addChild(&appMenu);
	screen.repos();
}
