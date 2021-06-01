#include <FS/CompressedFile.h>
#include "AutonomousDriving.h"
#include "../../Components/CameraFeed.h"
#include <string>


AutonomousDriving::AutonomousDriving(Display& display, Driver* driver) : Context(display), screenLayout(new LinearLayout(&screen, VERTICAL)), driver(driver){
	buildUI();
	AutonomousDriving::pack();
}

AutonomousDriving::~AutonomousDriving(){

}

void AutonomousDriving::start(){
	driver->start();
	draw();
	screen.commit();
}

void AutonomousDriving::stop(){
	driver->stop();
}

void AutonomousDriving::draw(){
	screen.getSprite()->drawIcon(cameraBuffer, 0, 4, 160, 120);
	screen.getSprite()->drawIcon(backgroundBuffer, 0, 0, 160, 128, 1, TFT_TRANSPARENT);
	screen.draw();
}

void AutonomousDriving::init(){
	Context::init();
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
	Context::deinit();
	free(backgroundBuffer);
}

void AutonomousDriving::buildUI(){
	screenLayout->setWHType(PARENT, PARENT);
	for(int i = 0; i < 4; i++){
		motorPower[i] = driver->getMotorState(i);
		sprintf(buffer, "%s", motorPower);
		engines.push_back(new DrivingElement(screenLayout, MOTOR, buffer, true));
		screenLayout->addChild(engines[i]);
	}
	screenLayout->reflow();
	screen.addChild(screenLayout);
	screen.repos();
	engines[0]->setPos(screen.getTotalX() + 3, screen.getTotalY() + 20);
	engines[1]->setPos(screen.getTotalX() + 3, screen.getTotalY() + 100);
	engines[2]->setPos(screen.getTotalX() + 140, screen.getTotalY() + 20);
	engines[3]->setPos(screen.getTotalX() + 140, screen.getTotalY() + 100);
}

void AutonomousDriving::loop(uint micros){
	cameraBuffer = driver->getCameraImage();
	for(int i = 0; i < 4; i++){
		motorPower[i] = driver->getMotorState(i);
		sprintf(buffer, "%s", motorPower);
	}
	draw();
	screen.commit();
}
