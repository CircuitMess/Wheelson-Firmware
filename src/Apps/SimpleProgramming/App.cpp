#include "App.h"
#include "Edit.h"
#include "Playback.h"
#include <FS/CompressedFile.h>
#include <U8g2_for_TFT_eSPI.h>
#include <Wheelson.h>
#include <Input/Input.h>
#include <Loop/LoopManager.h>
#include <SPIFFS.h>

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
	screen.getSprite()->drawLine(0, 19, screen.getWidth(), 19, TFT_WHITE);
	Battery.drawIcon(screen.getSprite());

	FontWriter u8f = screen.getSprite()->startU8g2Fonts();
	u8f.setFont(u8g2_font_profont12_tf);
	u8f.setForegroundColor(TFT_WHITE);
	u8f.setFontMode(1);
	u8f.setCursor((160 - u8f.getUTF8Width("Simple programming")) / 2, screen.getTotalY() + 13);
	u8f.println("Simple programming");
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
		const Simple::Program* prog = storage.getProg(i);
		programs.push_back(new ProgramElement(list, "Program " + String((long) prog->id + 1)));
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
		programs[programNum]->setIsSelected(true);
		scrollLayout->scrollIntoView(programNum, 5);

	}
}

void Simple::App::buildUI(){
	scrollLayout->setWHType(PARENT, FIXED);
	scrollLayout->setHeight(60);
	scrollLayout->addChild(list);

	list->setWHType(PARENT, CHILDREN);
	list->setY(100);
	list->setPadding(5);
	list->setGutter(5);
	list->addChild(addIcon);

	scrollLayout->reflow();
	list->reflow();

	screen.addChild(scrollLayout);
	screen.repos();

	scrollLayout->setY(screen.getTotalY() + 20);
	addIcon->setX(70);
}

void Simple::App::loop(uint micros){
	if(midPressTime == 0 && backPressTime == 0){
		LoopManager::removeListener(this);
		return;
	}

	if(backPressTime != 0 && millis() - backPressTime >= 1000){
		backPressTime = 0;
		LoopManager::removeListener(this);

		if(programNum >= programs.size()) return;

		storage.removeProg(programNum);
		if(programs.size() > 1){
			if(programNum == 0){
				selectAction(programNum + 1);
			}
			selectAction(programNum - 1);
		}
		loadProgs();
		draw();
		screen.commit();
		return;
	}

	if(midPressTime != 0 && millis() - midPressTime >= 1000){
		midPressTime = 0;
		LoopManager::removeListener(this);

		if(programNum >= programs.size()) return;

		const Program* prog = storage.getProg(programNum);

		Context* play = new Playback(*screen.getDisplay(), prog);
		play->push(this);
		return;
	}

	draw();
	screen.commit();
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

	if(programNum == list->getChildren().size()-1 && list->getChildren().size() != 1){
		scrollLayout->scrollIntoView(programNum-1, 5);
	}else{
		scrollLayout->scrollIntoView(programNum, 5);
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
			draw();
			screen.commit();
			break;

		case BTN_DOWN:
			if(programNum == list->getChildren().size() - 1){
				selectAction(0);
			}else{
				selectAction(programNum + 1);
			}

			draw();
			screen.commit();
			break;

		case BTN_BACK:
			if(midPressTime != 0) break;
			backPressTime = millis();
			LoopManager::addListener(this);

			if(programNum < list->getChildren().size()-1){
				reinterpret_cast<ProgramElement*>(list->getChildren()[programNum])->touchStart(TFT_RED);
			}

			break;

		case BTN_MID:
			if(backPressTime != 0) break;
			midPressTime = millis();
			LoopManager::addListener(this);

			if(programNum < list->getChildren().size()-1){
				reinterpret_cast<ProgramElement*>(list->getChildren()[programNum])->touchStart(C_RGB(0, 170, 0));
			}

			break;
	}
}

void Simple::App::buttonReleased(uint id){
	if(id == BTN_BACK){
		uint32_t elapsed = millis() - backPressTime;

		backPressTime = 0;
		LoopManager::removeListener(this);

		if(programNum < list->getChildren().size()-1){
			reinterpret_cast<ProgramElement*>(list->getChildren()[programNum])->touchEnd();
			draw();
			screen.commit();
		}

		if(elapsed < 500){
			pop();
			return;
		}
	}else if(id == BTN_MID){
		uint32_t elapsed = millis() - midPressTime;

		midPressTime = 0;
		LoopManager::removeListener(this);

		if(programNum < list->getChildren().size()-1){
			reinterpret_cast<ProgramElement*>(list->getChildren()[programNum])->touchEnd();
			draw();
			screen.commit();
		}

		if(elapsed < 500){
			if(programNum == programs.size()){
				storage.addProg(nullptr, 0);
			}

			Context* edit = new Edit(*screen.getDisplay(), &storage, programNum);
			edit->push(this);
			return;
		}
	}
}

