#include <Arduino.h>
#include <CircuitOS.h>
#include <Loop/LoopManager.h>
#include <esp32-hal-psram.h>
#include <Display/Display.h>
#include <SPIFFS.h>
#include "src/IntroScreen.h"


Display display(160, 128, -1, -1);

void setup(){
	Serial.begin(115200);

	if(psramFound()){
		Serial.printf("PSRAM init: %s, free: %d B\n", psramInit() ? "Yes" : "No", ESP.getFreePsram());
	}else{
		Serial.println("No PSRAM");
	}

	if(!SPIFFS.begin()){
		Serial.println("SPIFFS error");
	}

	display.begin();

	IntroScreen::IntroScreen* intro = new IntroScreen::IntroScreen(display);
	intro->unpack();
	intro->start();
}

void loop(){
	LoopManager::loop();
}