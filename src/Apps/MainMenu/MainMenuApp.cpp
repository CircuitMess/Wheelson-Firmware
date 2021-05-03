#include "MainMenuApp.h"
#include "../../mem.h"

const char* const MainMenuApp::appIcons[] = {"/mainMenu/app_autonomous.raw", "/mainMenu/app_action.raw", "/mainMenu/app_ball.raw", "/mainMenu/app_qr.raw", "/mainMenu/app_settings.raw"};

MainMenuApp::MainMenuApp(ElementContainer* parent, MenuApp app) : CustomElement(parent, 40, 40), app(app){

	bgBuffer = static_cast<Color*>(w_malloc(40 * 40 * 2));
	if(bgBuffer == nullptr){
		Serial.printf("MainMenuApp picture %s unpack error\n", appIcons[app]);
		return;
	}

	fs::File bgFile = SPIFFS.open(appIcons[app]);
	bgFile.read(reinterpret_cast<uint8_t*>(bgBuffer), 40 * 40 * 2);
	bgFile.close();

}

MainMenuApp::~MainMenuApp(){
	free(bgBuffer);
}

void MainMenuApp::draw(){
	getSprite()->drawIcon(bgBuffer, getTotalX(), getTotalY(), 40, 40, 1, TFT_BLACK);
}

void MainMenuApp::isSelected(bool selected){
	MainMenuApp::selected = selected;

}
