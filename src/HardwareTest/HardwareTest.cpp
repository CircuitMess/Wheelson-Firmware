#include "HardwareTest.h"
#include "../IntroScreen.h"
#include <Wheelson.h>
#include <U8g2_for_TFT_eSPI.h>
#include <Loop/LoopManager.h>
#include "../Components/CameraFeed.h"

HardwareTest::HardwareTest(Display& display,Driver* driver) : Context(display), screenLayout(new LinearLayout(&screen, HORIZONTAL)), leftLayout(new LinearLayout(screenLayout, VERTICAL)),
											   midLayout(new LinearLayout(screenLayout, VERTICAL)), rightLayout(new LinearLayout(screenLayout, VERTICAL)),driver(driver){

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
	if(!inputIsDone){
		screenLayout->draw();
	}
	if(doneCounter >= 6){
		FontWriter u8f = screen.getSprite()->startU8g2Fonts();
		u8f.setFont(u8g2_font_HelvetiPixel_tr);
		u8f.setForegroundColor(TFT_WHITE);
		u8f.setFontMode(1);
		u8f.setCursor((160 - u8f.getUTF8Width("All OK! Press any button")) / 2, 13);
		u8f.print("All OK! Press any button");
	}
	if(inputIsDone){
		screen.draw();
		screen.getSprite()->clear(TFT_BLACK);
		screen.getSprite()->drawIcon(cameraBuffer,0,4,160,120);
		FontWriter u8f = screen.getSprite()->startU8g2Fonts();
		u8f.setFont(u8g2_font_HelvetiPixel_tr);
		u8f.setForegroundColor(TFT_WHITE);
		u8f.setFontMode(1);
		u8f.setCursor((160 - u8f.getUTF8Width("If OK press any button")) / 2, 50);
		u8f.print("If OK press any button");
	}else if(cameraIsDone){
		screen.getSprite()->clear(TFT_BLACK);
		FontWriter u8f = screen.getSprite()->startU8g2Fonts();
		u8f.setFont(u8g2_font_HelvetiPixel_tr);
		u8f.setForegroundColor(TFT_WHITE);
		u8f.setFontMode(1);
		u8f.setCursor((160 - u8f.getUTF8Width("If all LED's works good")) / 2, 53);
		u8f.print("If all LED's works good");
		u8f.setCursor((130 - u8f.getUTF8Width("press any key")) / 2, 63);
		u8f.print("   press any key");
	}else if(LEDisDone){
		screen.getSprite()->clear(TFT_BLACK);
	}

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
	free(cameraBuffer);
}

void HardwareTest::buttonPressed(uint id){
	switch(id){
		case BTN_LEFT:
			if(doneCounter >= 6){
				inputIsDone = true;
				doneCounter = 0;
				draw();
				screen.commit();
				break;
			}else if(inputIsDone){
				inputIsDone = false;
				cameraIsDone = true;
				draw();
				screen.commit();
				break;
			}else if(cameraIsDone){
				cameraIsDone = false;
				LEDisDone = true;
				draw();
				screen.commit();
				break;
			}else if(LEDisDone){
				this->pop();
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
			if(doneCounter >= 6){
				inputIsDone = true;
				doneCounter = 0;
				draw();
				screen.commit();
				break;
			}else if(inputIsDone){
				inputIsDone = false;
				cameraIsDone = true;
				draw();
				screen.commit();
				break;
			}else if(cameraIsDone){
				cameraIsDone = false;
				LEDisDone = true;
				draw();
				screen.commit();
				break;
			}else if(LEDisDone){
				this->pop();
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
			if(doneCounter >= 6){
				inputIsDone = true;
				doneCounter = 0;
				draw();
				screen.commit();
				break;
			}else if(inputIsDone){
				inputIsDone = false;
				cameraIsDone = true;
				draw();
				screen.commit();
				break;
			}else if(cameraIsDone){
				cameraIsDone = false;
				LEDisDone = true;
				draw();
				screen.commit();
				break;
			}else if(LEDisDone){
				this->pop();
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
			if(doneCounter >= 6){
				inputIsDone = true;
				doneCounter = 0;
				draw();
				screen.commit();
				break;
			}else if(inputIsDone){
				inputIsDone = false;
				cameraIsDone = true;
				draw();
				screen.commit();
				break;
			}else if(cameraIsDone){
				cameraIsDone = false;
				LEDisDone = true;
				draw();
				screen.commit();
				break;
			}else if(LEDisDone){
				this->pop();
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
			if(doneCounter >= 6){
				inputIsDone = true;
				doneCounter = 0;
				draw();
				screen.commit();
				break;
			}else if(inputIsDone){
				inputIsDone = false;
				cameraIsDone = true;
				draw();
				screen.commit();
				break;
			}else if(cameraIsDone){
				cameraIsDone = false;
				LEDisDone = true;
				draw();
				screen.commit();
				break;
			}else if(LEDisDone){
				this->pop();
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
			if(doneCounter >= 6){
				inputIsDone = true;
				doneCounter = 0;
				draw();
				screen.commit();
				break;
			}else if(inputIsDone){
				inputIsDone = false;
				cameraIsDone = true;
				draw();
				screen.commit();
				break;
			}else if(cameraIsDone){
				cameraIsDone = false;
				LEDisDone = true;
				draw();
				screen.commit();
				break;
			}else if(LEDisDone){
				this->pop();
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
	if(inputIsDone){
		cameraBuffer = driver->getCameraImage();
		draw();
		screen.commit();
	}else if(cameraIsDone){
		LED.setHeadlight(255);
		LED.setRGB(RED);
		delay(500);
		LED.setRGB(GREEN);
		delay(500);
		LED.setRGB(YELLOW);
		delay(500);
		LED.setRGB(BLUE);
		delay(500);
		LED.setRGB(MAGENTA);
		delay(500);
		LED.setRGB(CYAN);
		delay(500);
		LED.setRGB(WHITE);
		delay(500);
		LED.setRGB(OFF);
		LED.setHeadlight(0);
		delay(1000);

	}
}
