#include "UserHWTest.h"
#include "HWTestPart.h"
#include "InputHWTest.h"
#include "CameraHWTest.h"
#include "LEDHWTest.h"


UserHWTest::UserHWTest(Display& display) : Context(display){
	hwTestPart = new InputHWTest(this);
}

UserHWTest::~UserHWTest(){

}

void UserHWTest::draw(){
	hwTestPart->draw();
}

void UserHWTest::start(){
	Input::getInstance()->addListener(this);
	draw();
	screen.commit();
}

void UserHWTest::stop(){
	Input::getInstance()->removeListener(this);
}

void UserHWTest::buttonPressed(uint id){
	hwTestPart->buttonPressed(id);
}

void UserHWTest::currentTestDone(){
	delete hwTestPart;
	testCounter++;
	if(testCounter > 2){
		if(doneCallback){
			doneCallback(this);
		}else{
			this->pop();
		}
		return;
	}
	nextTest();
}

void UserHWTest::nextTest(){
	if(testCounter == 1){
		hwTestPart = new CameraHWTest(this);
	}
	if(testCounter == 2){
		hwTestPart = new LEDHWTest(this);
	}
	hwTestPart->draw();
	screen.commit();
}

void UserHWTest::setDoneCallback(void (* doneCallback)(UserHWTest*)){
	UserHWTest::doneCallback = doneCallback;
}


