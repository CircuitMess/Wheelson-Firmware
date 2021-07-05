#include <Wheelson.h>
#include <Loop/LoopManager.h>
#include "CameraHWTest.h"

CameraHWTest::CameraHWTest(UserHWTest* userHwTest) : HWTestPart(userHwTest){
	LoopManager::addListener(this);
	Input::getInstance()->addListener(this);

}

CameraHWTest::~CameraHWTest(){
	LoopManager::removeListener(this);
	Input::getInstance()->removeListener(this);
}

void CameraHWTest::draw(){
	userHwTest->getScreen().draw();
	cameraFeed.loadFrame();
	userHwTest->getScreen().getSprite()->drawIcon(reinterpret_cast<Color*>(cameraFeed.getRGB565()), 0, 0, 160, 120);
	cameraFeed.releaseFrame();
	FontWriter u8f = userHwTest->getScreen().getSprite()->startU8g2Fonts();
	u8f.setFont(u8g2_font_HelvetiPixel_tr);
	u8f.setForegroundColor(TFT_WHITE);
	u8f.setFontMode(1);
	u8f.setCursor((160 - u8f.getUTF8Width("If OK press any button")) / 2, 50);
	u8f.print("If OK press any button");

}

void CameraHWTest::start(){
	draw();
	userHwTest->getScreen().commit();
}

void CameraHWTest::stop(){

}

void CameraHWTest::loop(uint micros){
	draw();
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
			draw();
			userHwTest->getScreen().commit();
			break;
	}
}
