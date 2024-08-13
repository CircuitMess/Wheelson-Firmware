#include <FS/CompressedFile.h>
#include <SPIFFS.h>
#include "Autonomous.h"
#include <Loop/LoopManager.h>
#include <Input/Input.h>
#include <Wheelson.h>

#define paramPopupTime 3 //in seconds

Autonomous::Autonomous(Display& display, Driver* driver) : Context(display), screenLayout(new LinearLayout(&screen, VERTICAL)), driver(driver){
	buildUI();
	Autonomous::pack();
}

Autonomous::~Autonomous(){
	delete driver;
	free(backgroundBuffer);
}

void Autonomous::start(){
	LED.setHeadlight(255);
	driver->start();
	draw();
	screen.commit();
	LoopManager::addListener(this);
	Input::getInstance()->addListener(this);
	paramPopupActive = false;
	paramPopupMillis = millis();
}

void Autonomous::stop(){
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

void Autonomous::draw(){
	Sprite* canvas = screen.getSprite();

	canvas->clear(TFT_BLACK);

	if(driver->camWorks()){
		canvas->drawIcon( driver->getProcessedImage(), 0, 4, 160, 120);
	}else{
		canvas->setTextColor(TFT_WHITE);
		canvas->setTextSize(1);
		canvas->setTextFont(1);
		canvas->setTextDatum(textdatum_t::top_center);
		canvas->drawString("Camera error!", canvas->width()/2, 50);
	}
	canvas->drawIcon(backgroundBuffer, 0, 0, 160, 128, 1, TFT_TRANSPARENT);
	Battery.drawIcon(canvas);

	if(driver->camWorks() && (paramPopupActive || !firstStart)){
		canvas->fillRoundRect(30, 93, 100, 27, 3, C_HEX(0x0082ff));
		canvas->drawRoundRect(29, 92, 102, 29, 5, TFT_WHITE);

		canvas->setTextColor(TFT_WHITE);
		canvas->setTextSize(1);
		canvas->setTextFont(1);

		if(paramPopupActive){
			driver->drawParamControl(*screen.getSprite(), 35, 108, 90, 8);
			canvas->setTextDatum(textdatum_t::top_center);
			canvas->drawString(driver->getParamName(), canvas->width()/2, 96);
		}else if(!firstStart){
			canvas->setTextDatum(textdatum_t::top_center);
			canvas->drawString("Press down to", canvas->width()/2, 96);
			canvas->drawString("toggle motors",canvas->width()/2, 109);
		}
	}

	screen.draw();
}

void Autonomous::init(){
	backgroundBuffer = static_cast<Color*>(ps_malloc(160 * 128 * 2));
	if(backgroundBuffer == nullptr){
		Serial.printf("MainMenu background picture unpack error\n");
		return;
	}

	fs::File backgroundFile = CompressedFile::open(SPIFFS.open("/AutoDrive/raw_bg.raw.hs"), 14, 13);

	backgroundFile.read(reinterpret_cast<uint8_t*>(backgroundBuffer), 160 * 128 * 2);
	backgroundFile.close();
}

void Autonomous::deinit(){
	free(backgroundBuffer);
	backgroundBuffer = nullptr;
}

void Autonomous::buildUI(){
	screenLayout->setWHType(PARENT, PARENT);
	for(int i = 0; i < 4; i++){
		engines.push_back(new DrivingElement(screenLayout, MOTOR, "", false));
		screenLayout->addChild(engines[i]);
	}
	screenLayout->reflow();
	screen.addChild(screenLayout);
	screen.repos();
	engines[0]->setPos(screen.getTotalX() + 3, screen.getTotalY() + 15);
	engines[1]->setPos(screen.getTotalX() + 3, screen.getTotalY() + 85);
	engines[2]->setPos(screen.getTotalX() + 140, screen.getTotalY() + 15);
	engines[3]->setPos(screen.getTotalX() + 140, screen.getTotalY() + 85);
}

void Autonomous::loop(uint micros){
	driver->prepareFrame();
	driver->draw();
	char buffer[4];
	for(int i = 0; i < 4; i++){
		int8_t percentage = (((float)driver->getMotorState(i))/127)*100;
		percentage = map(percentage, -100, 100, -10, 10);
		sprintf(buffer, "%d",percentage);
		engines[i]->setText(buffer);
	}
	if(paramPopupActive && millis() - paramPopupMillis >= paramPopupTime*1000){
		paramPopupActive = false;
	}
	draw();
	screen.commit();
}

void Autonomous::buttonPressed(uint i){
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
		case BTN_DOWN:
			if(!driver->camWorks()) break;

			if(!firstStart){
				firstStart = true;
			}
			driver->toggleMotors();
			DrivingElement::toggleMotors();
			break;
		case BTN_UP:
			LED.setHeadlight(!LED.getHeadlight());
			break;
	}
}
