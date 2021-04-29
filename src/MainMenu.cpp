#include "defs.hpp"
#include "MainMenu.h"
#include "Apps/Timeline/TimelineApp.h"
#include <Input/Input.h>
#include <SPIFFS.h>
#include "mem.h"

const char* const MainMenu::MainMenu::icons[] = {"/app_action.raw", "/app_autonomous.raw", "/app_kuglica.raw", "/app_qr.raw", "/app_settings.raw"};

MainMenu* MainMenu::instance = nullptr;

MainMenu::MainMenu(Display& display) : Context(display), appMenu(&screen, 3){
	for(int i = 0; i < 5; i++){
		buffer[i] = static_cast<Color*>(w_malloc(40 * 40 * 2));
		if(buffer[i]== nullptr){
			Serial.printf("MainMenu picture %s unpack error\n", icons[i]);
			return;
		}
		iconFile[i] = SPIFFS.open(icons[i]);
		iconFile[i].seek(0);
		iconFile[i].read(reinterpret_cast<uint8_t*>(buffer[i]), 40 * 40 * 2);
		iconFile[i].close();
	}
	instance = this;

	menuItems.push_back({"Simple programming mode", new TimelineApp(display), new BitmapElement(&appMenu, buffer[0] , 40, 40)});
	menuItems.push_back({"Autonomous driving mode", new AutonomousApp(display), new BitmapElement(&appMenu,buffer[1] , 40, 40)});
	menuItems.push_back({"Object tracking", nullptr, new BitmapElement(&appMenu,buffer[2] , 40, 40)});
	menuItems.push_back({"QR tracker", nullptr, new BitmapElement(&appMenu,buffer[3] , 40, 40)});
	menuItems.push_back({"Settings", nullptr, new BitmapElement(&appMenu,buffer[4] , 40, 40)});

	buildUI();
	pack();

}

void MainMenu::start(){
	draw();
	screen.commit();
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