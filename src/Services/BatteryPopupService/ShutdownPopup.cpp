#include "ShutdownPopup.h"
#include <Loop/LoopManager.h>
#include <Wheelson.h>
#include <WiFi.h>
const uint8_t ShutdownPopup::shutdownTime = 5;

ShutdownPopup::ShutdownPopup(Context &context) : Modal(context, 70, 30){
}

void ShutdownPopup::draw(){
	screen.getSprite()->setTextColor(TFT_WHITE);
	screen.getSprite()->setTextSize(1);
	screen.getSprite()->setTextFont(1);
	screen.getSprite()->setCursor(0, screen.getTotalY() + 2);
	screen.getSprite()->printCenter("Shutting down");
}

void ShutdownPopup::start(){
	LoopManager::addListener(this);
	draw();
	screen.commit();
}

void ShutdownPopup::stop(){
	LoopManager::removeListener(this);
}

void ShutdownPopup::loop(uint micros){
	shutdownTimer += micros;
	if(shutdownTimer >= shutdownTime * 1000000){
		Nuvo.shutdown();
		WiFi.mode(WIFI_OFF);
		btStop();
		esp_deep_sleep_start();
	}
}

