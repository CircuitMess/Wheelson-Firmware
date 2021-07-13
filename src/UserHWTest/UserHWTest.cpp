#include <FS/CompressedFile.h>
#include "UserHWTest.h"
#include "InputHWTest.h"
#include "CameraHWTest.h"
#include "MotorHWTest.h"
#include "LEDHWTest.h"


UserHWTest::UserHWTest(Display& display) : Context(display){
	hwTestPart = new InputHWTest(this);
	Context::pack();
}

UserHWTest::~UserHWTest(){
	delete hwTestPart;
}

void UserHWTest::draw(){
	screen.getSprite()->drawIcon(backgroundBuffer, 0, 0, 160, 128, 1);
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
	if(testCounter > 3){
		if(doneCallback){
			doneCallback(this);
		}else{
			this->pop();
		}
		return;
	}else{
		hwTestPart->stop();
		delete hwTestPart;
		hwTestPart = nullptr;
	}
	nextTest();
}

void UserHWTest::nextTest(){
	if(testCounter == 1){
		hwTestPart = new CameraHWTest(this);
	}else if(testCounter == 2){
		hwTestPart = new LEDHWTest(this);
	}else if(testCounter == 3){
		hwTestPart = new MotorHWTest(this);
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

void UserHWTest::init(){
	backgroundBuffer = static_cast<Color*>(ps_malloc(160 * 128 * 2));
	if(backgroundBuffer == nullptr){
		Serial.println("HWTest background unpack error");
		return;
	}

	fs::File bgFile = CompressedFile::open(SPIFFS.open("/Setts/settings_bg.raw.hs"), 9, 8);
	bgFile.read(reinterpret_cast<uint8_t*>(backgroundBuffer), 160 * 128 * 2);
	bgFile.close();
}

void UserHWTest::deinit(){
	free(backgroundBuffer);

}


