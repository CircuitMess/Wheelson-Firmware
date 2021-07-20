#include <Wheelson.h>
#include <Loop/LoopManager.h>
#include "CameraHWTest.h"

CameraHWTest::CameraHWTest(UserHWTest* userHwTest) : HWTestPart(userHwTest){

}

CameraHWTest::~CameraHWTest(){

}

void CameraHWTest::draw(){
	userHwTest->getScreen().getSprite()->clear(TFT_BLACK);
	FontWriter u8f = userHwTest->getScreen().getSprite()->startU8g2Fonts();
	u8f.setFont(u8g2_font_HelvetiPixel_tr);
	u8f.setForegroundColor(TFT_WHITE);
	u8f.setFontMode(1);
	if(cameraFeed.isInited()){
		cameraFeed.loadFrame();
		userHwTest->getScreen().getSprite()->drawIcon(reinterpret_cast<Color*>(cameraFeed.getRGB565()), 0, 4, 160, 120);
		cameraFeed.releaseFrame();
		
		u8f.setCursor((160 - u8f.getUTF8Width("If camera works good")) / 2, 53);
		u8f.print("If camera works good");
		u8f.setCursor((130 - u8f.getUTF8Width("press any button.")) / 2, 63);
		u8f.print("    press any button.");
	}else{
		u8f.setCursor((160 - u8f.getUTF8Width("Camera is not detected!")) / 2, 53);
		u8f.print("Camera is not detected!");
	}
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
