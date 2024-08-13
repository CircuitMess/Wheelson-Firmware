#include "MainMenu.h"
#include "../Simple/Simple.h"
#include "../Settings/SettingsScreen.h"
#include "../Autonomous/Autonomous.h"
#include "../Autonomous/MarkerDriver.h"
#include "../Autonomous/LineDriver.h"
#include "../Autonomous/BallDriver.h"
#include "../RemoteControl/RemoteControl.h"
#include <FS/CompressedFile.h>
#include <Wheelson.h>
#include <Input/Input.h>
#include <Loop/LoopManager.h>
#include <SPIFFS.h>
#include "../../Fonts.h"

const char* const MainMenu::AppTitles[] = {"Simple programming", "Line tracking", "Ball tracking", "Marker tracking", "Remote control", "Settings"};

Context* (*MainMenu::AppLaunch[])(Display& display) = {
		[](Display& display) -> Context* { return new Simple::Simple(display); },
		[](Display& display) -> Context* { return new Autonomous(display, new LineDriver()); },
		[](Display& display) -> Context* { return new Autonomous(display, new BallDriver()); },
		[](Display& display) -> Context* { return new Autonomous(display, new MarkerDriver()); },
		[](Display& display) -> Context* { return new RemoteControl(display); },
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

	auto canvas = screen.getSprite();
	canvas->setFont(&u8g2_font_HelvetiPixel_tr);
	canvas->setTextColor(TFT_WHITE);
	canvas->setTextDatum(textdatum_t::top_center);
	canvas->drawString(AppTitles[appNum], canvas->width()/2, 110);
	screen.draw();

}

void MainMenu::buildUI(){
	layout.setWHType(PARENT, PARENT);
	layout.setGutter(5);

	for(int i = 0; i < 6; i++){
		MainMenuItem* app = new MainMenuItem(&screen, static_cast<MenuApp>(i));
		apps.push_back(app);
		layout.addChild(app);
	}
	apps[0]->setPos(10, 15);
	apps[1]->setPos(60, 15);
	apps[2]->setPos(110, 15);
	apps[3]->setPos(10, 65);
	apps[4]->setPos(60, 65);
	apps[5]->setPos(110, 65);
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
				selectApp(5);
			}else{
				selectApp(appNum - 1);
			}

			draw();
			screen.commit();
			break;

		case BTN_RIGHT:
			selectApp((appNum + 1) % 6);

			draw();
			screen.commit();
			break;

		case BTN_UP:
		case BTN_DOWN:
			if(appNum > 2){
				selectApp(appNum - 3);
			}else{
				selectApp((appNum + 3) % 6);
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


