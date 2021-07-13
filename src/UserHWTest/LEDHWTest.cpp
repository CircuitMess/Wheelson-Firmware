#include "LEDHWTest.h"
#include <Wheelson.h>
#include <U8g2_for_TFT_eSPI.h>
#include <Loop/LoopManager.h>

LEDHWTest::LEDHWTest(UserHWTest* userHwTest) : HWTestPart(userHwTest){
}

LEDHWTest::~LEDHWTest(){

}

void LEDHWTest::draw(){
	FontWriter u8f = userHwTest->getScreen().getSprite()->startU8g2Fonts();
	u8f.setFont(u8g2_font_HelvetiPixel_tr);
	u8f.setForegroundColor(TFT_WHITE);
	u8f.setFontMode(1);
	u8f.setCursor((160 - u8f.getUTF8Width("Check the headlights and")) / 2, 43);
	u8f.print("Check the headlights and");
	u8f.setCursor((160 - u8f.getUTF8Width("RGB LED under the screen.")) / 2, 53);
	u8f.print("RGB LED under the screen.");
	u8f.setCursor((160 - u8f.getUTF8Width("Press any key")) / 2, 73);
	u8f.print("Press any key");
	u8f.setCursor((160 - u8f.getUTF8Width("if everything is A-OK.")) / 2, 83);
	u8f.print("if everything is A-OK.");
}

void LEDHWTest::start(){
	LoopManager::addListener(this);
	Input::getInstance()->addListener(this);

	userHwTest->draw();
	userHwTest->getScreen().commit();
}

void LEDHWTest::stop(){
	LED.setRGB(OFF);
	LED.setHeadlight(0);
	LoopManager::removeListener(this);
	Input::getInstance()->removeListener(this);
}

void LEDHWTest::loop(uint micros){
	if(millis() - previousTime >= 500){
		previousTime = millis();
		if(LED.getRGB() == OFF || LED.getBacklight() == 0){
			LED.setRGB(static_cast<WLEDColor>(random(1, 6)));
			LED.setHeadlight(255);
		}else{
			LED.setRGB(OFF);
			LED.setHeadlight(0);
		}
	}

}

void LEDHWTest::buttonPressed(uint id){
	switch(id){
		case BTN_UP:
		case BTN_DOWN:
		case BTN_LEFT:
		case BTN_RIGHT:
		case BTN_MID:
		case BTN_BACK:
			userHwTest->currentTestDone();
			break;
	}
}

