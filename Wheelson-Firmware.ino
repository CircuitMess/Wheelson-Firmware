#include <Arduino.h>
#include <CircuitOS.h>
#include <Wheelson.h>
#include <Loop/LoopManager.h>
#include <Support/Context.h>
#include <Settings.h>
#include "src/BatteryPopup/BatteryPopupService.h"
#include "src/UserHWTest/UserHWTest.h"
#include "src/HardwareTest.h"
#include "src/IntroScreen.h"


bool checkJig(){
	char buf[7];
	int wp = 0;

	uint32_t start = millis();
	int c;
	while(millis() - start < 500){
		vTaskDelay(1);
		c = getchar();
		if(c == EOF) continue;
		buf[wp] = (char) c;
		wp = (wp + 1) % 7;

		for(int i = 0; i < 7; i++){
			int match = 0;
			static const char* target = "JIGTEST";

			for(int j = 0; j < 7; j++){
				match += buf[(i + j) % 7] == target[j];
			}

			if(match == 7) return true;
		}
	}

	return false;
}

void setup(){
	Serial.begin(115200);

	if(checkJig() || true){

		Wheelson.initVer(2);
		Wheelson.begin();

		LED.setBacklight(true);

		HardwareTest test(Wheelson.getDisplay());
		test.start();

		for(;;);
	}

	Wheelson.begin();

	disableCore0WDT();
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
		IntroScreen* intro = new IntroScreen(Wheelson.getDisplay());
		intro->unpack();
		intro->start();
	}

	LED.setBacklight(true);
}

void loop(){
	LoopManager::loop();
}