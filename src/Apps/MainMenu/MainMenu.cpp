#include "MainMenu.h"
#include "../Timeline/TimelineApp.h"
#include <Input/Input.h>
#include "../../mem.h"
#include <U8g2_for_TFT_eSPI.h>
#include <FS/CompressedFile.h>

const char* const MainMenu::AppTitles[] = {"Autonomous", "Simple", "Ball", "Object", "Settings"};

MainMenu* MainMenu::instance = nullptr;

MainMenu::MainMenu(Display& display) : Context(display), layout(&screen){
	instance = this;
	buildUI();

	pack();

}

MainMenu::~MainMenu(){
	instance = nullptr;
}

void MainMenu::start(){
	draw();
	screen.commit();

}


void MainMenu::stop(){

}

void MainMenu::unpack(){
	Context::unpack();

	fs::File backgroundFile = CompressedFile::open(SPIFFS.open("/mainMenu/mainmenu_bg.raw.hs"),13,12);

	backgroundBuffer = static_cast<Color*>(w_malloc(160 * 128 * 2));
	if(backgroundBuffer == nullptr){
		Serial.printf("MainMenu background picture unpack error\n");
		return;
	}

	backgroundFile.read(reinterpret_cast<uint8_t*>(backgroundBuffer), 160 * 128 * 2);
	backgroundFile.close();

}

void MainMenu::pack(){
	Context::pack();
	free(backgroundBuffer);
}

void MainMenu::draw(){
	screen.getSprite()->drawIcon(backgroundBuffer, 0, 0, 160, 128, 1);
	Sprite* canvas = screen.getSprite();

	FontWriter u8f = canvas->startU8g2Fonts();
	u8f.setFont(u8g2_font_HelvetiPixel_tr);
	u8f.setForegroundColor(TFT_WHITE);
	u8f.setFontMode(1);
	u8f.setCursor(55, 120);
	u8f.print(textIcons[appNum]);
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
	apps[0]->setX(10);apps[0]->setY(15);
	apps[1]->setX(60);apps[1]->setY(15);
	apps[2]->setX(110);apps[2]->setY(15);
	apps[3]->setX(40);apps[3]->setY(65);
	apps[4]->setX(90);apps[4]->setY(65);
	layout.reflow();
	screen.addChild(&layout);
	screen.repos();


}


