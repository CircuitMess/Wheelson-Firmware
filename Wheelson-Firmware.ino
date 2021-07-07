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
#include "src/HardwareTest.h"
#include "src/UserHWTest/UserHWTest.h"

bool checkJig(){
	Serial.println(TFT_CS);
	pinMode(TFT_CS, INPUT_PULLUP);
	digitalWrite(TFT_CS, HIGH);

	return digitalRead(TFT_CS) == LOW;
}

void setup(){
	Serial.begin(115200);

	if(psramFound()){
		Serial.printf("PSRAM init: %s, free: %d B\n", psramInit() ? "Yes" : "No", ESP.getFreePsram());
	}else{
		Serial.println("No PSRAM");
	}

	if(checkJig()){
		Display display(160, 128, -1, -1);
		display.begin();

		Nuvo.begin();
		LED.setBacklight(true);

		HardwareTest test(display);
		test.start();

		for(;;);
	}

	if(!Nuvo.begin()){
		Serial.println("Nuvoton error");
		for(;;);
	}

	Display* display = new Display(160, 128, -1, -1);

	display->begin();
	display->getBaseSprite()->clear(TFT_BLACK);
	display->commit();

	if(!SPIFFS.begin()){
		Serial.println("SPIFFS error");
	}

	Settings.begin();
	Input* input = new WheelsonInput();
	input->preregisterButtons({ 0, 1, 2, 3, 4, 5 });
	LoopManager::addListener(input);

	Battery.disableShutdown(true);
	BatteryPopup.setTFT(display->getTft());
	LoopManager::addListener(&BatteryPopup);
	LoopManager::addListener(&Battery);

	Context::setDeleteOnPop(true);
if(!Settings.get().inputTested){
	UserHWTest* test = new UserHWTest(*display);
	test->setDoneCallback([](UserHWTest*test){
		Settings.get().inputTested = true;
		Settings.store();

		ESP.restart();
	});
	test->unpack();
	test->start();
}else{
	IntroScreen::IntroScreen* intro = new IntroScreen::IntroScreen(*display);
	intro->unpack();
	intro->start();
}
	LED.setBacklight(true);
}

void loop(){
	LoopManager::loop();
}