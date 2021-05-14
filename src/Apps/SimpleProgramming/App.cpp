#include "App.h"
#include "Edit.h"
#include <FS/CompressedFile.h>
#include <U8g2_for_TFT_eSPI.h>
#include <Wheelson.h>
#include <Input/Input.h>

Simple::App* Simple::App::instance = nullptr;

Simple::App::App(Display& display) : Context(display), scrollLayout(new ScrollLayout(&screen)), list(new LinearLayout(scrollLayout, VERTICAL)), addIcon(new AddIcon(list)){

	instance = this;

	for(int i = 0; i < 3; i++){
		programs.push_back(new ProgramElement(list, "Program"));
	}

	buildUI();
	programs[programNum]->setIsSelected(true);
	App::pack();

}

Simple::App::~App(){
	instance = nullptr;

}

void Simple::App::start(){
	Input::getInstance()->addListener(this);
	draw();
	screen.commit();

}

void Simple::App::stop(){
	Input::getInstance()->removeListener(this);
}

void Simple::App::draw(){
	screen.getSprite()->drawIcon(backgroundBuffer, 0, 0, 160, 128, 1);
	screen.draw();
	screen.getSprite()->drawIcon(backgroundBuffer, 0, 0, 160, 19, 1);

	FontWriter u8f = screen.getSprite()->startU8g2Fonts();
	u8f.setFont(u8g2_font_profont12_tf);
	u8f.setForegroundColor(TFT_WHITE);
	u8f.setFontMode(1);
	u8f.setCursor((160 - u8f.getUTF8Width("SIMPLE PROGRAMMING")) / 2, screen.getTotalY() + 17);
	u8f.println("SIMPLE PROGRAMMING");


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
	fs::File backgroundFile = CompressedFile::open(SPIFFS.open("/Simple/app_bg.raw.hs"), 13, 12);

	backgroundFile.read(reinterpret_cast<uint8_t*>(backgroundBuffer), 160 * 128 * 2);
	backgroundFile.close();

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
	list->addChild(addIcon);

	scrollLayout->reflow();
	list->reflow();

	screen.addChild(scrollLayout);
	screen.repos();

	scrollLayout->setY(screen.getTotalY() + 25);
	addIcon->setX(70);
}

void Simple::App::loop(uint micros){

}

void Simple::App::selectAction(uint8_t num){
	if(programNum == list->getChildren().size() - 1){
		addIcon->setSelected(false);
	}else{
		programs[programNum]->setIsSelected(false);
	}
	if(num == list->getChildren().size() - 1){
		programNum = num;
		addIcon->setSelected(true);
	}else{
		programNum = num;
		programs[programNum]->setIsSelected(true);
	}
}

void Simple::App::buttonPressed(uint id){
	switch(id){
		case BTN_UP:
			if(programNum == 0){
				selectAction(list->getChildren().size() - 1);
			}else{
				selectAction(programNum - 1);
			}

			scrollLayout->scrollIntoView(programNum, 5);
			draw();
			screen.commit();
			break;

		case BTN_DOWN:
			if(programNum == list->getChildren().size() - 1){
				selectAction(0);
			}else{
				selectAction(programNum + 1);
			}

			scrollLayout->scrollIntoView(programNum, 5);
			draw();
			screen.commit();
			break;
		case BTN_MID:
			if(programNum == list->getChildren().size() - 1){
				Display& display = *instance->getScreen().getDisplay();
				Simple::Edit* edit = new Simple::Edit(display,storage,0);
				edit->unpack();
				edit->start();
			}else{

			}
			break;

	}
}


