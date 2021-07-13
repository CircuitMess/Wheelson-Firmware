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
	u8f.setCursor((160 - u8f.getUTF8Width("If all LEDs work good")) / 2, 53);
	u8f.print("If all LEDs work good");
	u8f.setCursor((130 - u8f.getUTF8Width("press any button")) / 2, 63);
	u8f.print("   press any button");
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

