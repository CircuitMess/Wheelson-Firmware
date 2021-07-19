#include <FS/CompressedFile.h>
#include <SPIFFS.h>
#include "AutonomousDriving.h"
#include <Loop/LoopManager.h>
#include <Input/Input.h>
#include <Wheelson.h>

#define paramPopupTime 3 //in seconds

AutonomousDriving::AutonomousDriving(Display& display, Driver* driver) : Context(display), screenLayout(new LinearLayout(&screen, VERTICAL)), driver(driver){
	buildUI();
	AutonomousDriving::pack();
}

AutonomousDriving::~AutonomousDriving(){
	delete driver;
	free(backgroundBuffer);
}

void AutonomousDriving::start(){
	LED.setHeadlight(255);
	driver->start();
	draw();
	screen.commit();
	LoopManager::addListener(this);
	Input::getInstance()->addListener(this);
	paramPopupActive = false;
	paramPopupMillis = millis();
}

void AutonomousDriving::stop(){
	LED.setHeadlight(0);
	driver->stop();
	Nuvo.getI2C().loop(0);
	LoopManager::removeListener(this);
	Input::getInstance()->removeListener(this);
	Motors.setMotor(MOTOR_FR, 0);
	Motors.setMotor(MOTOR_BR, 0);
	Motors.setMotor(MOTOR_FL, 0);
	Motors.setMotor(MOTOR_BL, 0);

}

void AutonomousDriving::draw(){
	screen.getSprite()->clear(TFT_BLACK);
	screen.getSprite()->drawIcon( driver->getProcessedImage(), 0, 4, 160, 120);
	screen.getSprite()->drawIcon(backgroundBuffer, 0, 0, 160, 128, 1, TFT_TRANSPARENT);
	if(paramPopupActive){
		screen.getSprite()->fillRoundRect(30, 95, 100, 27, 3, C_HEX(0x0082ff));
		screen.getSprite()->drawRoundRect(29, 94, 102, 29, 5, TFT_WHITE);
		driver->drawParamControl(*screen.getSprite(), 35, 110, 90, 8);
		screen.getSprite()->setTextColor(TFT_WHITE);
		screen.getSprite()->setTextSize(1);
		screen.getSprite()->setTextFont(1);
		screen.getSprite()->setCursor(105, 98);
		screen.getSprite()->printCenter(driver->getParamName());
	}
	screen.draw();
}

void AutonomousDriving::init(){
	backgroundBuffer = static_cast<Color*>(ps_malloc(160 * 128 * 2));
	if(backgroundBuffer == nullptr){
		Serial.printf("MainMenu background picture unpack error\n");
		return;
	}

	fs::File backgroundFile = CompressedFile::open(SPIFFS.open("/AutoDrive/raw_bg.raw.hs"), 14, 13);

	backgroundFile.read(reinterpret_cast<uint8_t*>(backgroundBuffer), 160 * 128 * 2);
	backgroundFile.close();
}

void AutonomousDriving::deinit(){
	free(backgroundBuffer);
	backgroundBuffer = nullptr;
}

void AutonomousDriving::buildUI(){
	screenLayout->setWHType(PARENT, PARENT);
	for(int i = 0; i < 4; i++){
		engines.push_back(new DrivingElement(screenLayout, MOTOR, "", true));
		screenLayout->addChild(engines[i]);
	}
	screenLayout->reflow();
	screen.addChild(screenLayout);
	screen.repos();
	engines[0]->setPos(screen.getTotalX() + 3, screen.getTotalY() + 15);
	engines[1]->setPos(screen.getTotalX() + 3, screen.getTotalY() + 80);
	engines[2]->setPos(screen.getTotalX() + 140, screen.getTotalY() + 15);
	engines[3]->setPos(screen.getTotalX() + 140, screen.getTotalY() + 80);
}

void AutonomousDriving::loop(uint micros){
	char buffer[4];
	for(int i = 0; i < 4; i++){
		int8_t percentage = (((float)driver->getMotorState(i))/127)*100;
		sprintf(buffer, "%d",percentage);
		engines[i]->setText(buffer);
	}
	if(paramPopupActive && millis() - paramPopupMillis >= paramPopupTime*1000){
		paramPopupActive = false;
	}
	draw();
	screen.commit();
}

void AutonomousDriving::buttonPressed(uint i){
	switch(i){
		case BTN_BACK:
			this->pop();
			break;
		case BTN_MID:
			driver->toggleDisplayMode();
			break;
		case BTN_LEFT:
			if(driver->getParamName() != nullptr){
				paramPopupMillis = millis();
				if(!paramPopupActive){
					paramPopupActive = true;
				}else{
					driver->setParam(max(driver->getParam() - 15, 0));
				}
			}
			break;
		case BTN_RIGHT:
			if(driver->getParamName() != nullptr){
				paramPopupMillis = millis();
				if(!paramPopupActive){
					paramPopupActive = true;
				}else{
					driver->setParam(min(driver->getParam() + 15, 255));
				}
			}
			break;
	}
}
