#include "SettingsScreen.h"
#include <Wheelson.h>
#include <Input/Input.h>

SettingsScreen::SettingsScreen::SettingsScreen(Display& display) : Context(display), screenLayout(new LinearLayout(&screen, VERTICAL)),
																   shutDownSlider(new DescreteSlider(screenLayout, "Shutdown time", {0, 1, 5, 15, 30})),
																   speedSlider(new SliderElement(screenLayout, "Speed modifier")), inputTest(new TextElement(screenLayout, "Input Test")),
																   save(new TextElement(screenLayout, "Save")){


	buildUI();
	shutDownSlider->setIsSelected(true);

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
	screen.getSprite()->clear(TFT_BLACK);
	screen.getSprite()->setTextColor(TFT_WHITE);
	screen.getSprite()->setTextSize(1);
	screen.getSprite()->setTextFont(1);
	screen.getSprite()->setCursor(screenLayout->getTotalX() + 42, screenLayout->getTotalY() + 115);
	screen.getSprite()->println("Version 1.0");

	for(int i = 0; i < 4; i++){
		if(!reinterpret_cast<SettingsElement*>(screenLayout->getChild(i))->isSelected()){
			screenLayout->getChild(i)->draw();
		}
	}
	for(int i = 0; i < 4; i++){
		if(reinterpret_cast<SettingsElement*>(screenLayout->getChild(i))->isSelected()){
			screenLayout->getChild(i)->draw();
		}
	}
}

void SettingsScreen::SettingsScreen::deinit(){
	Context::deinit();
}

void SettingsScreen::SettingsScreen::init(){
	Context::init();
}

SettingsScreen::SettingsScreen::~SettingsScreen(){

}

void SettingsScreen::SettingsScreen::buildUI(){
	screenLayout->setWHType(PARENT, PARENT);
	screenLayout->setGutter(5);
	screenLayout->addChild(shutDownSlider);
	screenLayout->addChild(speedSlider);
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
			Serial.println("KlikLijevo");
			if(shutDownSlider->isSliderSelected()){
				shutDownSlider->selectPrev();
			}else if(speedSlider->isSliderSelected()){
				speedSlider->moveSliderValue(-1);
			}
			draw();
			screen.commit();
			break;

		case BTN_RIGHT:
			Serial.println("KlikDesno");
			if(shutDownSlider->isSliderSelected()){
				shutDownSlider->selectNext();
			}else if(speedSlider->isSliderSelected()){
				speedSlider->moveSliderValue(1);
			}
			draw();
			screen.commit();
			break;

		case BTN_UP:
			if(!disableMainSelector){
				selectedSetting--;
				if(selectedSetting > 3){
					selectedSetting = 0;
				}
				if(selectedSetting == 0){
					shutDownSlider->setIsSelected(true);
				}else{
					shutDownSlider->setIsSelected(false);
				}
				if(selectedSetting == 1){
					speedSlider->setIsSelected(true);
				}else{
					speedSlider->setIsSelected(false);
				}
				if(selectedSetting == 2){
					inputTest->setIsSelected(true);
				}else{
					inputTest->setIsSelected(false);
				}
				if(selectedSetting == 3){
					save->setIsSelected(true);
				}else{
					save->setIsSelected(false);
				}

				draw();
				screen.commit();
			}
			break;

		case BTN_DOWN:
			if(!disableMainSelector){
				selectedSetting++;
				if(selectedSetting < 0){
					selectedSetting = 3;
				}
				if(selectedSetting == 0){
					shutDownSlider->setIsSelected(true);
				}else{
					shutDownSlider->setIsSelected(false);
				}
				if(selectedSetting == 1){
					speedSlider->setIsSelected(true);
				}else{
					speedSlider->setIsSelected(false);
				}
				if(selectedSetting == 2){
					inputTest->setIsSelected(true);
				}else{
					inputTest->setIsSelected(false);
				}
				if(selectedSetting == 3){
					save->setIsSelected(true);
				}else{
					save->setIsSelected(false);
				}
				draw();
				screen.commit();
			}
			break;

		case BTN_MID:
			if(selectedSetting == 0){
				shutDownSlider->toggle();
				disableMainSelector = !disableMainSelector;
			}else if(selectedSetting == 1){
				speedSlider->toggle();
				disableMainSelector = !disableMainSelector;
			}else if(selectedSetting == 2){

			}else if(selectedSetting == 3){

			}
			draw();
			screen.commit();
			break;
	}

}

