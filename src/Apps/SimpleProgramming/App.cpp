#include "App.h"
#include <FS/CompressedFile.h>
#include <U8g2_for_TFT_eSPI.h>

Simple::App* Simple::App::instance = nullptr;

Simple::App::App(Display& display) : Context(display), scrollLayout(new ScrollLayout(&screen)), list(new LinearLayout(scrollLayout, VERTICAL)){

	instance = this;

	for(int i = 0; i < 3; i++){
		programs.push_back(new ProgramList(list, "Program"));
	}

	buildUI();
	App::pack();

}

Simple::App::~App(){
	instance = nullptr;

}

void Simple::App::start(){
	draw();
	screen.commit();

}

void Simple::App::stop(){

}

void Simple::App::draw(){
	screen.getSprite()->drawIcon(backgroundBuffer, 0, 0, 160, 128, 1);
	screen.getSprite()->drawIcon(addBuffer, 66, 108, 15, 15, 1,TFT_TRANSPARENT);

	FontWriter u8f = screen.getSprite()->startU8g2Fonts();
	u8f.setFont(u8g2_font_profont12_tf);
	u8f.setForegroundColor(TFT_WHITE);
	u8f.setFontMode(1);
	u8f.setCursor((160 - u8f.getUTF8Width("SIMPLE PROGRAMMING")) / 2, screen.getTotalY() + 17);
	u8f.println("SIMPLE PROGRAMMING");

	screen.draw();
}

void Simple::App::deinit(){
	free(backgroundBuffer);
}

void Simple::App::init(){
	backgroundBuffer = static_cast<Color*>(ps_malloc(160 * 128 * 2));
	if(backgroundBuffer == nullptr){
		Serial.printf("App background picture unpack error\n");
		return;
	}
	fs::File backgroundFile = CompressedFile::open(SPIFFS.open("/mainmenu_bg.raw.hs"), 13, 12);

	backgroundFile.read(reinterpret_cast<uint8_t*>(backgroundBuffer), 160 * 128 * 2);
	backgroundFile.close();

	addBuffer = static_cast<Color*>(ps_malloc(15 * 15 * 2));
	if(addBuffer == nullptr){
		Serial.printf("App add_button picture unpack error\n");
		return;
	}
	fs::File addFile = SPIFFS.open("/add_button.raw");

	addFile.read(reinterpret_cast<uint8_t*>(addBuffer), 15*15 * 2);
	addFile.close();
	screen.draw();
}

void Simple::App::buildUI(){
	scrollLayout->setWHType(PARENT, FIXED);
	scrollLayout->setHeight(80);
	scrollLayout->addChild(list);
	//scrollLayout->setBorder(2, TFT_RED);

	list->setWHType(PARENT, CHILDREN);
	list->setY(100);
	list->setPadding(5);
	list->setGutter(5);

	for(int i = 0; i < programs.size(); i++){
		list->addChild((programs[i]));
	}

	scrollLayout->reflow();
	list->reflow();

	screen.addChild(scrollLayout);
	screen.repos();

	scrollLayout->setY(screen.getTotalY()+25);
}

void Simple::App::loop(uint micros){

}
