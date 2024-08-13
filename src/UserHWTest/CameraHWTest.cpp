#include <Wheelson.h>
#include <Loop/LoopManager.h>
#include "CameraHWTest.h"
#include "../Fonts.h"

CameraHWTest::CameraHWTest(UserHWTest* userHwTest) : HWTestPart(userHwTest){

}

CameraHWTest::~CameraHWTest(){

}

void CameraHWTest::draw(){
	userHwTest->getScreen().getSprite()->clear(TFT_BLACK);

	auto canvas = userHwTest->getScreen().getSprite();
	canvas->setFont(&u8g2_font_HelvetiPixel_tr);
	canvas->setTextColor(TFT_WHITE);
	canvas->setTextDatum(textdatum_t::top_center);

	if(cameraFeed.isInited()){
		cameraFeed.loadFrame();
		userHwTest->getScreen().getSprite()->drawIcon(reinterpret_cast<Color*>(cameraFeed.getRGB565()), 0, 4, 160, 120);
		cameraFeed.releaseFrame();

		canvas->drawString("If camera works good", canvas->width()/2, 43);
		canvas->drawString("    press any button.", canvas->width()/2, 53);
	}else{
		canvas->drawString("Camera is not detected!", canvas->width()/2, 43);
	}
	userHwTest->getScreen().getSprite()->fillRect(0,0,160,5,TFT_BLACK);
}

void CameraHWTest::start(){
	LoopManager::addListener(this);
	Input::getInstance()->addListener(this);

	userHwTest->draw();
	userHwTest->getScreen().commit();
}

void CameraHWTest::stop(){
	LoopManager::removeListener(this);
	Input::getInstance()->removeListener(this);
}

void CameraHWTest::loop(uint micros){
	userHwTest->draw();
	userHwTest->getScreen().commit();
}

void CameraHWTest::buttonPressed(uint id){
	switch(id){
		case BTN_LEFT:
		case BTN_RIGHT:
		case BTN_UP:
		case BTN_DOWN:
		case BTN_MID:
		case BTN_BACK:
			userHwTest->currentTestDone();
			break;
	}
}
