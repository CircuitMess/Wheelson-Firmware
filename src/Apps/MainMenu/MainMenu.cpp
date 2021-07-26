#include "MainMenu.h"
#include "../Simple/Simple.h"
#include "../Settings/SettingsScreen.h"
#include "../Autonomous/Autonomous.h"
#include "../Autonomous/MarkerDriver.h"
#include "../Autonomous/LineDriver.h"
#include "../Autonomous/BallDriver.h"
#include <FS/CompressedFile.h>
#include <U8g2_for_TFT_eSPI.h>
#include <Wheelson.h>
#include <Input/Input.h>
#include <Loop/LoopManager.h>
#include <SPIFFS.h>


const char* const MainMenu::AppTitles[] = {"Simple programming", "Line tracking", "Ball tracking", "Marker tracking", "Settings"};

Context* (*MainMenu::AppLaunch[])(Display& display) = {
		[](Display& display) -> Context* { return new Simple::Simple(display); },
		[](Display& display) -> Context* { return new Autonomous(display, new LineDriver()); },
		[](Display& display) -> Context* { return new Autonomous(display, new BallDriver()); },
		[](Display& display) -> Context* { return new Autonomous(display, new MarkerDriver()); },
		[](Display& display) -> Context* { return new SettingsScreen::SettingsScreen(display); }
};

MainMenu* MainMenu::instance = nullptr;

MainMenu::MainMenu(Display& display) : Context(display), layout(&screen){
	instance = this;
	buildUI();

	apps[appNum]->setSelected(true);

	MainMenu::pack();

}

MainMenu::~MainMenu(){
	instance = nullptr;
}

void MainMenu::start(){
	LoopManager::addListener(this);
	Input::getInstance()->addListener(this);
	draw();
	screen.commit();
}


void MainMenu::stop(){
	LoopManager::removeListener(this);
	Input::getInstance()->removeListener(this);
}

void MainMenu::init(){
	backgroundBuffer = static_cast<Color*>(ps_malloc(160 * 128 * 2));
	if(backgroundBuffer == nullptr){
		Serial.printf("MainMenu background picture unpack error\n");
		return;
	}

	fs::File backgroundFile = CompressedFile::open(SPIFFS.open("/MainMenu/mainmenu_bg.raw.hs"), 13, 12);

	backgroundFile.read(reinterpret_cast<uint8_t*>(backgroundBuffer), 160 * 128 * 2);
	backgroundFile.close();

}

void MainMenu::deinit(){
	free(backgroundBuffer);
}

void MainMenu::draw(){
	screen.getSprite()->drawIcon(backgroundBuffer, 0, 0, 160, 128, 1);
	Battery.drawIcon(screen.getSprite());

	FontWriter u8f = screen.getSprite()->startU8g2Fonts();
	u8f.setFont(u8g2_font_HelvetiPixel_tr);
	u8f.setForegroundColor(TFT_WHITE);
	u8f.setFontMode(1);
	u8f.setCursor((160 - u8f.getUTF8Width(AppTitles[appNum])) / 2, 120);
	u8f.print(AppTitles[appNum]);
	screen.draw();

}

void MainMenu::buildUI(){
	layout.setWHType(PARENT, PARENT);
	layout.setGutter(5);

	for(int i = 0; i < 5; i++){
		MainMenuItem* app = new MainMenuItem(&screen, static_cast<MenuApp>(i));
		apps.push_back(app);
		layout.addChild(app);
	}
	apps[0]->setX(10);
	apps[0]->setY(15);
	apps[1]->setX(60);
	apps[1]->setY(15);
	apps[2]->setX(110);
	apps[2]->setY(15);
	apps[3]->setX(40);
	apps[3]->setY(65);
	apps[4]->setX(90);
	apps[4]->setY(65);
	layout.reflow();
	screen.addChild(&layout);
	screen.repos();
}

void MainMenu::selectApp(int8_t num){
	apps[appNum]->setSelected(false);
	appNum = num;
	apps[appNum]->setSelected(true);
}

void MainMenu::buttonPressed(uint id){
	switch(id){
		case BTN_LEFT:
			if(appNum == 0){
				selectApp(4);
			}else{
				selectApp(appNum - 1);
			}

			draw();
			screen.commit();
			break;

		case BTN_RIGHT:
			selectApp((appNum + 1) % 5);

			draw();
			screen.commit();
			break;

		case BTN_UP:
		case BTN_DOWN:
			if(appNum == 2){
				selectApp(4);
			}else if(appNum > 2){
				selectApp(appNum - 3);
			}else{
				selectApp((appNum + 3) % 5);
			}

			draw();
			screen.commit();
			break;

		case BTN_MID:
			Context* app = AppLaunch[appNum](*screen.getDisplay());
			if(app == nullptr) break;
			app->push(this);
			break;
	}
}

void MainMenu::loop(uint micros){
	if(lastDrawnBatteryLevel != Battery.getLastDrawnLevel()){
		lastDrawnBatteryLevel = Battery.getLastDrawnLevel();
		screen.draw();
		Battery.drawIcon(screen.getSprite());
		screen.commit();
	}

}


