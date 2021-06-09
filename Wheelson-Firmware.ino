#include <Arduino.h>
#include <CircuitOS.h>
#include <Wheelson.h>
#include <Nuvoton/Nuvoton.h>
#include <Nuvoton/WheelsonInput.h>
#include <Loop/LoopManager.h>
#include <Display/Display.h>
#include <SPIFFS.h>
#include <esp32-hal-psram.h>
#include "src/IntroScreen.h"
#include "src/UserHWTest/UserHWTest.h"
#include <Settings.h>


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
	input->preregisterButtons({0, 1, 2, 3, 4, 5});
	LoopManager::addListener(input);

	display.begin();

	Context::setDeleteOnPop(true);

	if(!Settings.get().inputTested){
		UserHWTest* test = new UserHWTest(display);
		test->setDoneCallback([](UserHWTest* test){
			Settings.get().inputTested = true;
			Settings.store();
			Serial.println("Esp restart");
			ESP.restart();
		});

		test->unpack();
		test->start();
	}else{
		IntroScreen::IntroScreen* intro = new IntroScreen::IntroScreen(display);
		intro->unpack();
		intro->start();
	}

	LED.setBacklight(true);
}

void loop(){
	LoopManager::loop();
}