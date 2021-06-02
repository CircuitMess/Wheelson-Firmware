#include "HardwareTest.h"
#include <Wheelson.h>
#include <U8g2_for_TFT_eSPI.h>
#include <Loop/LoopManager.h>

HardwareTest::HardwareTest(Display& display) : Context(display), screenLayout(new LinearLayout(&screen, HORIZONTAL)), leftLayout(new LinearLayout(screenLayout, VERTICAL)),
											   midLayout(new LinearLayout(screenLayout, VERTICAL)), rightLayout(new LinearLayout(screenLayout, VERTICAL)){

	leftBtnTest.push_back(new BtnTestElement(leftLayout));
	for(int i = 0; i < 3; i++){
		midBtnTest.push_back(new BtnTestElement(midLayout));
	}
	for(int i = 0; i < 2; i++){
		rightBtnTest.push_back(new BtnTestElement(rightLayout));
	}
	buildUI();
	HardwareTest::pack();
}

HardwareTest::~HardwareTest(){

}

void HardwareTest::draw(){
	screen.getSprite()->clear(TFT_BLACK);
	if(doneCounter>=6){
		FontWriter u8f = screen.getSprite()->startU8g2Fonts();
		u8f.setFont(u8g2_font_HelvetiPixel_tr);
		u8f.setForegroundColor(TFT_WHITE);
		u8f.setFontMode(1);
		u8f.setCursor((160 - u8f.getUTF8Width("All OK! Press any button")) / 2, 13);
		u8f.print("All OK! Press any button");
	}
	screenLayout->draw();
}

void HardwareTest::start(){
	Input::getInstance()->addListener(this);
	LoopManager::addListener(this);
	draw();
	screen.commit();
}

void HardwareTest::stop(){
	Input::getInstance()->removeListener(this);
	LoopManager::removeListener(this);
}

void HardwareTest::buildUI(){
	screenLayout->setWHType(PARENT, PARENT);
	screenLayout->addChild(leftLayout);
	screenLayout->addChild(midLayout);
	screenLayout->addChild(rightLayout);

	leftLayout->setWHType(PARENT, PARENT);
	leftLayout->addChild(leftBtnTest[0]);
	leftLayout->reflow();

	midLayout->setWHType(PARENT, PARENT);
	midLayout->setGutter(3);
	for(int i = 0; i < 3; i++){
		midLayout->addChild(midBtnTest[i]);
	}
	midLayout->reflow();

	rightLayout->setWHType(PARENT, PARENT);
	rightLayout->setGutter(3);
	for(int i = 0; i < 2; i++){
		rightLayout->addChild(rightBtnTest[i]);
	}
	rightLayout->reflow();

	screen.addChild(screenLayout);
	screen.repos();

	leftLayout->setPos(30, 50);
	midLayout->setPos(60, 27);
	rightLayout->setPos(90, 27);

}

void HardwareTest::init(){
	Context::init();
}

void HardwareTest::deinit(){
	Context::deinit();
}

void HardwareTest::buttonPressed(uint id){
	switch(id){
		case BTN_LEFT:
			if(doneCounter>=6){
				isDone = true;
				draw();
				screen.commit();
				break;
			}
			if(leftBtnTest[0]->isBtnPressed()) return;
			leftBtnTest[0]->btnPress();
			if(leftBtnTest[0]->isBtnPressed()){
				doneCounter++;
			}
			draw();
			screen.commit();
			break;
		case BTN_RIGHT:
			if(doneCounter>=6){
				isDone = true;
				draw();
				screen.commit();
				break;
			}
			if(rightBtnTest[1]->isBtnPressed()) return;
			rightBtnTest[1]->btnPress();
			if(rightBtnTest[1]->isBtnPressed()){
				doneCounter++;
			}
			draw();
			screen.commit();
			break;
		case BTN_UP:
			if(doneCounter>=6){
				isDone = true;
				draw();
				screen.commit();
				break;
			}
			if(midBtnTest[0]->isBtnPressed()) return;
			midBtnTest[0]->btnPress();
			if(midBtnTest[0]->isBtnPressed()){
				doneCounter++;
			}
			draw();
			screen.commit();
			break;
		case BTN_DOWN:
			if(doneCounter>=6){
				isDone = true;
				draw();
				screen.commit();
				break;
			}
			if(midBtnTest[2]->isBtnPressed()) return;
			midBtnTest[2]->btnPress();
			if(midBtnTest[2]->isBtnPressed()){
				doneCounter++;
			}
			draw();
			screen.commit();
			break;
		case BTN_MID:
			if(doneCounter>=6){
				isDone = true;
				draw();
				screen.commit();
				break;
			}
			if(midBtnTest[1]->isBtnPressed()) return;
			midBtnTest[1]->btnPress();
			if(midBtnTest[1]->isBtnPressed()){
				doneCounter++;
			}
			draw();
			screen.commit();
			break;
		case BTN_BACK:
			if(doneCounter>=6){
				isDone = true;
				draw();
				screen.commit();
				break;
			}
			if(rightBtnTest[0]->isBtnPressed()) return;
			rightBtnTest[0]->btnPress();
			if(rightBtnTest[0]->isBtnPressed()){
				doneCounter++;
			}
			draw();
			screen.commit();
			break;
	}
}

void HardwareTest::loop(uint micros){
if(isDone){

}
