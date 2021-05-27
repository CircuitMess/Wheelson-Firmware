#include "App.h"
#include "Edit.h"
#include "Playback.h"
#include <FS/CompressedFile.h>
#include <U8g2_for_TFT_eSPI.h>
#include <Wheelson.h>
#include <Input/Input.h>
#include <Loop/LoopManager.h>

Simple::App::App(Display& display) : Context(display), scrollLayout(new ScrollLayout(&screen)), list(new LinearLayout(scrollLayout, VERTICAL)), addIcon(new AddIcon(list)){

	buildUI();
	App::pack();
}

Simple::App::~App(){

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

	loadProgs();
}

void Simple::App::loadProgs(){
	for(auto& prog : programs){
		delete prog;
	}

	programs.clear();
	list->getChildren().clear();

	for(int i = 0; i < storage.getNumProgs(); i++){
		programs.push_back(new ProgramElement(list, "Program " + String((long) i + 1)));
		list->addChild(programs.back());
	}

	list->addChild(addIcon);

	list->reflow();
	list->repos();
	scrollLayout->scrollIntoView(0, 0);
	addIcon->setX(70);

	if(programs.empty()){
		addIcon->setSelected(true);
	}else{
		addIcon->setSelected(false);
		programs.front()->setIsSelected(true);
	}
}

void Simple::App::buildUI(){
	scrollLayout->setWHType(PARENT, FIXED);
	scrollLayout->setHeight(80);
	scrollLayout->addChild(list);

	list->setWHType(PARENT, CHILDREN);
	list->setY(100);
	list->setPadding(5);
	list->setGutter(5);

	scrollLayout->reflow();
	list->reflow();

	screen.addChild(scrollLayout);
	screen.repos();

	scrollLayout->setY(screen.getTotalY() + 25);
	addIcon->setX(70);
}

void Simple::App::loop(uint micros){
	currentTime = millis();
	if(currentTime - previousTime > 1000){
		previousTime = currentTime;
		seconds++;
	}

}

void Simple::App::selectAction(uint8_t num){
	for(auto& prog : programs){
		prog->setIsSelected(false);
	}
	addIcon->setSelected(false);
	if(num >= programs.size()){
		addIcon->setSelected(true);
	}else{
		programs[num]->setIsSelected(true);
	}

	programNum = num;
}

void Simple::App::buttonPressed(uint id){
	switch(id){
		case BTN_UP:
			if(programNum == 0){
				selectAction(list->getChildren().size() - 1);
			}else{
				Serial.println(programNum - 1);
				selectAction(programNum - 1);
			}
			scrollLayout->scrollIntoView(programNum, 5);
			draw();
			screen.commit();
			break;

		case BTN_DOWN:
			if(programNum == list->getChildren().size() - 1){
				Serial.println(0);
				selectAction(0);
			}else{
				Serial.println(programNum + 1);
				selectAction(programNum + 1);
			}

			scrollLayout->scrollIntoView(programNum, 5);
			draw();
			screen.commit();
			break;

		case BTN_BACK:
			pop();
			return;

		case BTN_MID:
			LoopManager::addListener(this);

			break;
	}
}

void Simple::App::buttonReleased(uint id){
	if(BTN_MID){
		if(seconds == 0)return;
		LoopManager::removeListener(this);
		if(seconds < 2){
			seconds = 0;
			LoopManager::removeListener(this);
			if(programNum == programs.size()){
				storage.addProg(nullptr, 0);
			}

			Simple::Edit* edit = new Simple::Edit(*getScreen().getDisplay(), &storage, programNum);
			edit->push(this);
		}else if(seconds >= 2){
			if(programNum == programs.size()) return;
			seconds = 0;
			LoopManager::removeListener(this);
			Simple::Playback* playback = new Simple::Playback(*getScreen().getDisplay(), (Action*) Action::FORWARD, 0);
			playback->push(this);
		}

	}
}

