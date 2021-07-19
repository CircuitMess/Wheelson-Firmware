#include <FS/CompressedFile.h>
#include <SPIFFS.h>
#include "AutonomousDriving.h"
#include <Loop/LoopManager.h>
#include <Input/Input.h>
#include <Wheelson.h>


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

	if(!firstStart){
		screen.getSprite()->drawRoundRect(29, 91, 102, 32, 7, TFT_WHITE);
		screen.getSprite()->fillRoundRect(30, 92, 100, 30, 5, C_HEX(0x0082ff));
		screen.getSprite()->setTextColor(TFT_WHITE);
		screen.getSprite()->setTextSize(1);
		screen.getSprite()->setTextFont(1);
		screen.getSprite()->setCursor(105, 96);
		screen.getSprite()->printCenter("Press down to");
		screen.getSprite()->setCursor(105, 109);
		screen.getSprite()->printCenter("toggle motors");
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
	engines[1]->setPos(screen.getTotalX() + 3, screen.getTotalY() + 95);
	engines[2]->setPos(screen.getTotalX() + 140, screen.getTotalY() + 15);
	engines[3]->setPos(screen.getTotalX() + 140, screen.getTotalY() + 95);
}

void AutonomousDriving::loop(uint micros){
	char buffer[4];
	for(int i = 0; i < 4; i++){
		int8_t percentage = (((float)driver->getMotorState(i))/127)*100;
		sprintf(buffer, "%d",percentage);
		engines[i]->setText(buffer);
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
		case BTN_DOWN:
			if(!firstStart){
				firstStart = true;
			}
			driver->toggleMotors();
			DrivingElement::toggleMotors();
			break;
	}
}
