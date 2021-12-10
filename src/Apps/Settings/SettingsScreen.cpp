#include "SettingsScreen.h"
#include "../../UserHWTest/UserHWTest.h"
#include <Wheelson.h>
#include <Input/Input.h>
#include <FS/CompressedFile.h>
#include <Settings.h>
#include <SPIFFS.h>

SettingsScreen::SettingsScreen::SettingsScreen(Display& display) : Context(display), screenLayout(new LinearLayout(&screen, VERTICAL)),
																   shutDownSlider(new DiscreteSlider(screenLayout, "Auto shutdown", {0, 1, 5, 15, 30})), inputTest(new TextElement(screenLayout, "Hardware test")),
																   save(new TextElement(screenLayout, "Save")){

	buildUI();
	shutDownSlider->setIsSelected(true);
	shutDownSlider->setIndex(Settings.get().shutdownTime);

	SettingsScreen::pack();
}

void SettingsScreen::SettingsScreen::start(){
	Input::getInstance()->addListener(this);
	draw();
	screen.commit();

}

void SettingsScreen::SettingsScreen::stop(){
	Input::getInstance()->removeListener(this);
}

void SettingsScreen::SettingsScreen::draw(){
	screen.getSprite()->drawIcon(backgroundBuffer, 0, 0, 160, 128, 1);
	screen.getSprite()->setTextColor(TFT_WHITE);
	screen.getSprite()->setTextSize(1);
	screen.getSprite()->setTextFont(1);
	screen.getSprite()->setCursor(screenLayout->getTotalX() + 42, screenLayout->getTotalY() + 115);
	screen.getSprite()->printCenter("Version 1.0.1");

	for(int i = 0; i < 3; i++){
		if(!reinterpret_cast<SettingsElement*>(screenLayout->getChild(i))->isSelected()){
			screenLayout->getChild(i)->draw();
		}
	}
	for(int i = 0; i < 3; i++){
		if(reinterpret_cast<SettingsElement*>(screenLayout->getChild(i))->isSelected()){
			screenLayout->getChild(i)->draw();
		}
	}
}

void SettingsScreen::SettingsScreen::deinit(){
	Context::deinit();
	free(backgroundBuffer);
}

void SettingsScreen::SettingsScreen::init(){
	Context::init();

	backgroundBuffer = static_cast<Color*>(ps_malloc(160 * 128 * 2));
	if(backgroundBuffer == nullptr){
		Serial.println("SettingsScreen background unpack error");
		return;
	}

	fs::File bgFile = CompressedFile::open(SPIFFS.open("/Setts/settings_bg.raw.hs"), 9, 8);
	bgFile.read(reinterpret_cast<uint8_t*>(backgroundBuffer), 160 * 128 * 2);
	bgFile.close();
}

SettingsScreen::SettingsScreen::~SettingsScreen(){

}

void SettingsScreen::SettingsScreen::buildUI(){
	screenLayout->setWHType(PARENT, PARENT);
	screenLayout->setGutter(5);
	screenLayout->addChild(shutDownSlider);
	screenLayout->addChild(inputTest);
	screenLayout->addChild(save);

	screenLayout->reflow();
	screen.addChild(screenLayout);
	screen.repos();
}

void SettingsScreen::SettingsScreen::selectApp(int8_t num){

}

void SettingsScreen::SettingsScreen::buttonPressed(uint id){
	switch(id){
		case BTN_LEFT:
			if(selectedSetting == 0){
				shutDownSlider->selectPrev();
			}
			draw();
			screen.commit();
			break;

		case BTN_RIGHT:
			if(selectedSetting == 0){
				shutDownSlider->selectNext();
			}
			draw();
			screen.commit();
			break;

		case BTN_UP:
			selectedSetting--;
			if(selectedSetting < 0){
				selectedSetting = 2;
			}
			if(selectedSetting == 0){
				shutDownSlider->setIsSelected(true);
			}else{
				shutDownSlider->setIsSelected(false);
			}
			if(selectedSetting == 1){
				inputTest->setIsSelected(true);
			}else{
				inputTest->setIsSelected(false);
			}
			if(selectedSetting == 2){
				save->setIsSelected(true);
			}else{
				save->setIsSelected(false);
			}

			draw();
			screen.commit();
			break;

		case BTN_DOWN:
			selectedSetting++;
			if(selectedSetting > 2){
				selectedSetting = 0;
			}
			if(selectedSetting == 0){
				shutDownSlider->setIsSelected(true);
			}else{
				shutDownSlider->setIsSelected(false);
			}
			if(selectedSetting == 1){
				inputTest->setIsSelected(true);
			}else{
				inputTest->setIsSelected(false);
			}
			if(selectedSetting == 2){
				save->setIsSelected(true);
			}else{
				save->setIsSelected(false);
			}
			draw();
			screen.commit();

			break;

		case BTN_MID:
			if(selectedSetting == 2){
				Settings.get().shutdownTime = shutDownSlider->getIndex();
				Settings.store();
				this->pop();
			}else if(selectedSetting == 1){
				Display& display = *this->getScreen().getDisplay();
				Context* hwTest = new UserHWTest(display);
				hwTest->push(this);
			}
			draw();
			screen.commit();
			break;
		case BTN_BACK:
			Settings.get().shutdownTime = shutDownSlider->getIndex();
			Settings.store();
			this->pop();
			draw();
			screen.commit();
			break;
	}

}