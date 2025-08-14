#include "Simple.h"
#include "Edit.h"
#include "Playback.h"
#include <FS/CompressedFile.h>
#include <Wheelson.h>
#include <Input/Input.h>
#include <Loop/LoopManager.h>
#include <SPIFFS.h>
#include "../../Fonts.h"

Simple::Simple::Simple(Display& display) : Context(display), scrollLayout(new ScrollLayout(&screen)), list(new LinearLayout(scrollLayout, VERTICAL)), addIcon(new AddIcon(list)){

	buildUI();
	Simple::pack();
}

Simple::Simple::~Simple(){

}

void Simple::Simple::start(){
	Input::getInstance()->addListener(this);
	draw();
	screen.commit();
}

void Simple::Simple::stop(){
	Input::getInstance()->removeListener(this);
}

void Simple::Simple::draw(){
	screen.getSprite()->drawIcon(backgroundBuffer, 0, 0, 160, 128, 1);
	screen.draw();
	screen.getSprite()->drawIcon(backgroundBuffer, 0, 0, 160, 19, 1);
	screen.getSprite()->drawLine(0, 19, screen.getWidth(), 19, TFT_WHITE);
	Battery.drawIcon(screen.getSprite());

	auto canvas = screen.getSprite();
	canvas->setFont(&u8g2_font_profont12_tf);
	canvas->setTextColor(TFT_WHITE);
	canvas->setTextDatum(textdatum_t::top_center);
	canvas->drawString("Simple programming", canvas->width() / 2, 4);
}

void Simple::Simple::deinit(){
	free(backgroundBuffer);
}

void Simple::Simple::init(){
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

void Simple::Simple::loadProgs(){
	for(auto& prog : programs){
		delete prog;
	}

	programs.clear();
	list->getChildren().clear();

	for(int i = 0; i < storage.getNumProgs(); i++){
		const Program* prog = storage.getProg(i);
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

void Simple::Simple::buildUI(){
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

void Simple::Simple::loop(uint micros){
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

void Simple::Simple::selectAction(uint8_t num){
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

	if(programNum == list->getChildren().size() - 1 && list->getChildren().size() != 1){
		scrollLayout->scrollIntoView(programNum - 1, 5);
	}else{
		scrollLayout->scrollIntoView(programNum, 5);
	}
}

void Simple::Simple::buttonPressed(uint id){
	if(id == Pins.get(Pin::BtnUp)){
		if(programNum == 0){
			selectAction(list->getChildren().size() - 1);
		}else{
			selectAction(programNum - 1);
		}
		draw();
		screen.commit();
	}else if(id == Pins.get(Pin::BtnDown)){
		if(programNum == list->getChildren().size() - 1){
			selectAction(0);
		}else{
			selectAction(programNum + 1);
		}
		draw();
		screen.commit();
	}else if(id == Pins.get(Pin::BtnBack)){
		if(midPressTime != 0) return;
		backPressTime = millis();
		LoopManager::addListener(this);

		if(programNum < list->getChildren().size() - 1){
			reinterpret_cast<ProgramElement*>(list->getChildren()[programNum])->touchStart(TFT_RED);
		}
	}else if(id == Pins.get(Pin::BtnMid)){
		if(backPressTime != 0) return;
		midPressTime = millis();
		LoopManager::addListener(this);

		if(programNum < list->getChildren().size() - 1){
			reinterpret_cast<ProgramElement*>(list->getChildren()[programNum])->touchStart(C_RGB(0, 170, 0));
		}
	}
}

void Simple::Simple::buttonReleased(uint id){
	if(id == Pins.get(Pin::BtnBack)){
		uint32_t elapsed = millis() - backPressTime;

		backPressTime = 0;
		LoopManager::removeListener(this);

		if(programNum < list->getChildren().size() - 1){
			reinterpret_cast<ProgramElement*>(list->getChildren()[programNum])->touchEnd();
			draw();
			screen.commit();
		}

		if(elapsed < 500){
			pop();
			return;
		}
	}else if(id == Pins.get(Pin::BtnMid)){
		uint32_t elapsed = millis() - midPressTime;

		midPressTime = 0;
		LoopManager::removeListener(this);

		if(programNum < list->getChildren().size() - 1){
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

