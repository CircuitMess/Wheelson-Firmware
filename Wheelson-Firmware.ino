#include <Arduino.h>
#include <CircuitOS.h>
#include <Wheelson.h>
#include <Nuvoton/Nuvoton.h>
#include <Nuvoton/WheelsonInput.h>
#include <Loop/LoopManager.h>
#include <Display/Display.h>
#include <SPIFFS.h>
#include <esp32-hal-psram.h>
#include <Settings.h>
#include "src/IntroScreen.h"
#include "src/Services/BatteryPopupService/BatteryPopupService.h"


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

	if(!Nuvo.begin()){
		Serial.println("Nuvoton error");
		for(;;);
	}

	Settings.begin();
	Input* input = new WheelsonInput();
	input->preregisterButtons({ 0, 1, 2, 3, 4, 5 });
	LoopManager::addListener(input);

	display.begin();

	Context::setDeleteOnPop(true);

	IntroScreen::IntroScreen* intro = new IntroScreen::IntroScreen(display);
	intro->unpack();
	intro->start();

	LED.setBacklight(true);
	LoopManager::addListener(&BatteryPopup);
	LoopManager::addListener(&Battery);
	Battery.disableShutdown(true);
}

void loop(){
	LoopManager::loop();
}