#include "LEDHWTest.h"
#include <Wheelson.h>
#include <Loop/LoopManager.h>
#include "../Fonts.h"

LEDHWTest::LEDHWTest(UserHWTest* userHwTest) : HWTestPart(userHwTest){
}

LEDHWTest::~LEDHWTest(){

}

void LEDHWTest::draw(){
	auto canvas = userHwTest->getScreen().getSprite();
	canvas->setFont(&u8g2_font_HelvetiPixel_tr);
	canvas->setTextColor(TFT_WHITE);

	canvas->setTextDatum(textdatum_t::top_center);
	canvas->drawString("Check the headlights and", canvas->width()/2, 33);
	canvas->drawString("RGB LED under the screen.", canvas->width()/2, 43);
	canvas->drawString("Press any key", canvas->width()/2, 63);
	canvas->drawString("if everything is A-OK.", canvas->width()/2, 73);
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
	userHwTest->currentTestDone();
}

