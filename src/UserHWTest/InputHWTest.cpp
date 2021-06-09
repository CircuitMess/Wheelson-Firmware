#include <Loop/LoopManager.h>
#include <Wheelson.h>
#include "InputHWTest.h"

InputHWTest::InputHWTest(UserHWTest* userHwTest) : HWTestPart(userHwTest), screenLayout(new LinearLayout(&userHwTest->getScreen(), HORIZONTAL)), leftLayout(new LinearLayout(screenLayout, VERTICAL)),
																midLayout(new LinearLayout(screenLayout, VERTICAL)), rightLayout(new LinearLayout(screenLayout, VERTICAL)){

	leftBtnTest.push_back(new BtnTestElement(leftLayout));
	for(int i = 0; i < 3; i++){
		midBtnTest.push_back(new BtnTestElement(midLayout));
	}
	for(int i = 0; i < 2; i++){
		rightBtnTest.push_back(new BtnTestElement(rightLayout));
	}
	buildUI();
}

InputHWTest::~InputHWTest(){

}

void InputHWTest::draw(){
	screenLayout->draw();
	if(inputIsDone){
		FontWriter u8f = userHwTest->getScreen().getSprite()->startU8g2Fonts();
		u8f.setFont(u8g2_font_HelvetiPixel_tr);
		u8f.setForegroundColor(TFT_WHITE);
		u8f.setFontMode(1);
		u8f.setCursor((160 - u8f.getUTF8Width("All OK! Press any button")) / 2, 13);
		u8f.print("All OK! Press any button");
	}
}

void InputHWTest::start(){
	Input::getInstance()->addListener(this);
	draw();
	userHwTest->getScreen().commit();
}

void InputHWTest::stop(){
	Input::getInstance()->removeListener(this);
}

void InputHWTest::buildUI(){
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

	userHwTest->getScreen().addChild(screenLayout);
	userHwTest->getScreen().repos();

	leftLayout->setPos(30, 50);
	midLayout->setPos(60, 27);
	rightLayout->setPos(90, 27);
}

void InputHWTest::buttonPressed(uint id){
	if(doneCounter >= 6){
		userHwTest->currentTestDone();
		return;
	}
	switch(id){
		case BTN_LEFT:
			if(leftBtnTest[0]->isBtnPressed()) return;
			leftBtnTest[0]->btnPress();
			if(leftBtnTest[0]->isBtnPressed()){
				doneCounter++;
			}
			if(doneCounter >=6){
				inputIsDone = true;
				draw();
				userHwTest->getScreen().commit();
				break;
			}
			draw();
			userHwTest->getScreen().commit();
			break;
		case BTN_RIGHT:
			if(rightBtnTest[1]->isBtnPressed()) return;
			rightBtnTest[1]->btnPress();
			if(rightBtnTest[1]->isBtnPressed()){
				doneCounter++;
			}
			if(doneCounter >=6){
				inputIsDone = true;
				draw();
				userHwTest->getScreen().commit();
				break;
			}
			draw();
			userHwTest->getScreen().commit();
			break;
		case BTN_UP:
			if(midBtnTest[0]->isBtnPressed()) return;
			midBtnTest[0]->btnPress();
			if(midBtnTest[0]->isBtnPressed()){
				doneCounter++;
			}
			if(doneCounter >=6){
				inputIsDone = true;
				draw();
				userHwTest->getScreen().commit();
				break;
			}
			draw();
			userHwTest->getScreen().commit();
			break;
		case BTN_DOWN:
			if(midBtnTest[2]->isBtnPressed()) return;
			midBtnTest[2]->btnPress();
			if(midBtnTest[2]->isBtnPressed()){
				doneCounter++;
			}
			if(doneCounter >=6){
				inputIsDone = true;
				draw();
				userHwTest->getScreen().commit();
				break;
			}
			draw();
			userHwTest->getScreen().commit();
			break;
		case BTN_MID:
			if(midBtnTest[1]->isBtnPressed()) return;
			midBtnTest[1]->btnPress();
			if(midBtnTest[1]->isBtnPressed()){
				doneCounter++;
			}
			if(doneCounter >=6){
				inputIsDone = true;
				draw();
				userHwTest->getScreen().commit();
				break;
			}
			draw();
			userHwTest->getScreen().commit();
			break;
		case BTN_BACK:
			if(rightBtnTest[0]->isBtnPressed()) return;
			rightBtnTest[0]->btnPress();
			if(rightBtnTest[0]->isBtnPressed()){
				doneCounter++;
			}
			if(doneCounter >=6){
				inputIsDone = true;
				draw();
				userHwTest->getScreen().commit();
				break;
			}
			draw();
			userHwTest->getScreen().commit();
			break;
	}
}

