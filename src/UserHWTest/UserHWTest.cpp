#include "UserHWTest.h"
#include "InputHWTest.h"
#include "CameraHWTest.h"
#include "LEDHWTest.h"


UserHWTest::UserHWTest(Display& display) : Context(display){
	hwTestPart = new InputHWTest(this);
}

UserHWTest::~UserHWTest(){
	delete hwTestPart;
}

void UserHWTest::draw(){
	hwTestPart->draw();
}

void UserHWTest::start(){
	hwTestPart->start();
}

void UserHWTest::stop(){
	hwTestPart->stop();
}

void UserHWTest::currentTestDone(){
	testCounter++;
	if(testCounter > 2){
		if(doneCallback){
			doneCallback(this);
		}else{
			this->pop();
		}
		return;
	}else{
		hwTestPart->stop();
		delete hwTestPart;
	}
	nextTest();
}

void UserHWTest::nextTest(){
	if(testCounter == 1){
		hwTestPart = new CameraHWTest(this);
	}else if(testCounter == 2){
		hwTestPart = new LEDHWTest(this);
	}else{
		if(doneCallback){
			doneCallback(this);
		}else{
			this->pop();
		}
		return;
	}
	hwTestPart->start();
}

void UserHWTest::setDoneCallback(void (* doneCallback)(UserHWTest*)){
	UserHWTest::doneCallback = doneCallback;
}


