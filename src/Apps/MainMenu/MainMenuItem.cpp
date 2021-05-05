#include "MainMenuItem.h"
#include "../../mem.h"

const char* const MainMenuItem::appIcons[] = {"/mainMenu/app_autonomous.raw", "/mainMenu/app_action.raw", "/mainMenu/app_ball.raw", "/mainMenu/app_qr.raw", "/mainMenu/app_settings.raw"};

MainMenuItem::MainMenuItem(ElementContainer* parent, MenuApp app) : CustomElement(parent, 40, 40), app(app){

	bgBuffer = static_cast<Color*>(w_malloc(40 * 40 * 2));
	if(bgBuffer == nullptr){
		Serial.printf("MainMenuApp picture %s unpack error\n", appIcons[app]);
		return;
	}

	fs::File bgFile = SPIFFS.open(appIcons[app]);
	bgFile.read(reinterpret_cast<uint8_t*>(bgBuffer), 40 * 40 * 2);
	bgFile.close();

}

MainMenuItem::~MainMenuItem(){
	free(bgBuffer);
}

void MainMenuItem::draw(){
	getSprite()->drawIcon(bgBuffer, getTotalX(), getTotalY(), 40, 40, 1, TFT_BLACK);
}

void MainMenuItem::isSelected(bool selected){
	MainMenuItem::selected = selected;

}
