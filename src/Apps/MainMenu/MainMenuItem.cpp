#include <SPIFFS.h>
#include "MainMenuItem.h"

const char* const MainMenuItem::AppIcons[] = {"/MainMenu/app_simple.raw", "/MainMenu/app_autonomous.raw", "/MainMenu/app_ball.raw", "/MainMenu/app_object.raw", "/MainMenu/app_settings.raw"};

MainMenuItem::MainMenuItem(ElementContainer* parent, MenuApp app) : CustomElement(parent, 40, 40), app(app){

	appIconBuffer = static_cast<Color*>(ps_malloc(40 * 40 * 2));
	if(appIconBuffer == nullptr){
		Serial.printf("MainMenuApp picture %s unpack error\n", AppIcons[app]);
		return;
	}

	fs::File bgFile = SPIFFS.open(AppIcons[app]);
	bgFile.read(reinterpret_cast<uint8_t*>(appIconBuffer), 40 * 40 * 2);
	bgFile.close();

}

MainMenuItem::~MainMenuItem(){
	free(appIconBuffer);

}

void MainMenuItem::draw(){
	getSprite()->drawIcon(appIconBuffer, getTotalX(), getTotalY(), 40, 40, 1, TFT_TRANSPARENT);
	if(selected && borderBuffer != nullptr){
		getSprite()->drawIcon(borderBuffer, getTotalX(), getTotalY(), 40, 40, 1, TFT_TRANSPARENT);
	}
}

void MainMenuItem::setSelected(bool selected){
	MainMenuItem::selected = selected;
	if(selected){
		borderBuffer = static_cast<Color*>(ps_malloc(40 * 40 * 2));
		if(borderBuffer == nullptr){
			Serial.printf("MainMenu border picture unpack error\n");
			return;
		}

		fs::File borderFile = SPIFFS.open("/MainMenu/mainmenu_border.raw");
		borderFile.read(reinterpret_cast<uint8_t*>(borderBuffer), 40 * 40 * 2);
		borderFile.close();
	}else{
		free(borderBuffer);
	}
}

bool MainMenuItem::isSelected() const{
	return selected;
}
