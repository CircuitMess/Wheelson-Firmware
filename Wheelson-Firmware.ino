#include <Arduino.h>
#include <CircuitOS.h>
#include <Wheelson.h>
#include <Loop/LoopManager.h>
#include <Support/Context.h>
#include <Settings.h>
#include "src/Services/BatteryPopupService/BatteryPopupService.h"
#include "src/UserHWTest/UserHWTest.h"
#include "src/HardwareTest.h"
#include "src/IntroScreen.h"


bool checkJig(){
	pinMode(TFT_CS, INPUT_PULLUP);
	digitalWrite(TFT_CS, HIGH);
	delay(100);
	return digitalRead(TFT_CS) == LOW;
}

void setup(){
	Serial.begin(115200);

	if(checkJig()){
		Display display(160, 128, -1, -1);
		display.begin();

		Nuvo.begin();
		LED.setBacklight(true);

		HardwareTest test(display);
		test.start();

		for(;;);
	}

	Wheelson.begin();

	Battery.disableShutdown(true);
	BatteryPopup.setTFT(Wheelson.getDisplay().getTft());
	LoopManager::addListener(&BatteryPopup);
	Input::getInstance()->addListener(&BatteryPopup);

	Context::setDeleteOnPop(true);

	if(!Settings.get().inputTested){
		UserHWTest* test = new UserHWTest(Wheelson.getDisplay());
		test->setDoneCallback([](UserHWTest* test){
			Settings.get().inputTested = true;
			Settings.store();

			ESP.restart();
		});
		test->unpack();
		test->start();
	}else{
		IntroScreen::IntroScreen* intro = new IntroScreen::IntroScreen(Wheelson.getDisplay());
		intro->unpack();
		intro->start();
	}

	LED.setBacklight(true);
}

void loop(){
	LoopManager::loop();
}