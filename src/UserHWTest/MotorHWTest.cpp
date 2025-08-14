#include <Loop/LoopManager.h>
#include <Wheelson.h>
#include "MotorHWTest.h"
#include "../Fonts.h"

MotorHWTest::MotorHWTest(UserHWTest* userHwTest) : HWTestPart(userHwTest){

}

MotorHWTest::~MotorHWTest(){

}

void MotorHWTest::draw(){
	auto canvas = userHwTest->getScreen().getSprite();
	canvas->setFont(&u8g2_font_profont12_tf);
	canvas->setTextColor(TFT_WHITE);
	canvas->setTextDatum(textdatum_t::top_center);

	canvas->drawString("Motor test", canvas->width() / 2, 0);

	canvas->setTextColor(TFT_RED);
	canvas->drawString("Grab your Wheelson!", canvas->width() / 2, 10);

	canvas->setTextColor(TFT_GREEN);
	canvas->drawString("Motors should go forward", canvas->width() / 2, 22);

	canvas->setTextColor(TFT_WHITE);
	canvas->setTextDatum(textdatum_t::top_left);
	canvas->drawString("Hold UP for FR motor", 5, 37);
	canvas->drawString("Hold RIGHT for BR motor", 5, 52);
	canvas->drawString("Hold DOWN for BL motor", 5, 67);
	canvas->drawString("Hold LEFT for FL motor", 5, 82);
	canvas->drawString("Hold MID for all motors", 5, 97);
	canvas->drawString("If they work,press BACK!", 5, 115);
}

void MotorHWTest::start(){
	LoopManager::addListener(this);
	Input::getInstance()->addListener(this);

	userHwTest->draw();
	userHwTest->getScreen().commit();
}

void MotorHWTest::stop(){
	LoopManager::removeListener(this);
	Input::getInstance()->removeListener(this);
	Motors.setMotor(MOTOR_FR, 0);
	Motors.setMotor(MOTOR_BR, 0);
	Motors.setMotor(MOTOR_FL, 0);
	Motors.setMotor(MOTOR_BL, 0);
}

void MotorHWTest::buttonPressed(uint id){
	if(id == Pins.get(Pin::BtnUp)){
		Motors.setMotor(MOTOR_FR, 127);
	}else if(id == Pins.get(Pin::BtnDown)){
		Motors.setMotor(MOTOR_BL, 127);
	}else if(id == Pins.get(Pin::BtnLeft)){
		Motors.setMotor(MOTOR_FL, 127);
	}else if(id == Pins.get(Pin::BtnRight)){
		Motors.setMotor(MOTOR_BR, 127);
	}else if(id == Pins.get(Pin::BtnMid)){
		Motors.setMotor(MOTOR_FR, 127);
		Motors.setMotor(MOTOR_BL, 127);
		Motors.setMotor(MOTOR_FL, 127);
		Motors.setMotor(MOTOR_BR, 127);
	}else if(id == Pins.get(Pin::BtnBack)){
		userHwTest->currentTestDone();
	}
}

void MotorHWTest::buttonReleased(uint i){
	if(i == Pins.get(Pin::BtnUp)){
		Motors.setMotor(MOTOR_FR, 0);
	}else if(i == Pins.get(Pin::BtnDown)){
		Motors.setMotor(MOTOR_BL, 0);
	}else if(i == Pins.get(Pin::BtnLeft)){
		Motors.setMotor(MOTOR_FL, 0);
	}else if(i == Pins.get(Pin::BtnRight)){
		Motors.setMotor(MOTOR_BR, 0);
	}else if(i == Pins.get(Pin::BtnMid)){
		Motors.setMotor(MOTOR_FR, 0);
		Motors.setMotor(MOTOR_BL, 0);
		Motors.setMotor(MOTOR_FL, 0);
		Motors.setMotor(MOTOR_BR, 0);
	}else if(i == Pins.get(Pin::BtnBack)){
		userHwTest->currentTestDone();
	}
}

void MotorHWTest::loop(uint micros){

}


