#include <SPIFFS.h>
#include "ActionElement.h"

const char* const Simple::ActionElement::ActionIcons[] = {"/Simple/arrow_up.raw", "/Simple/arrow_down.raw", "/Simple/arrow_left.raw", "/Simple/arrow_right.raw", "/Simple/light_off.raw", "/Simple/light_on.raw", "/Simple/pause.raw" , "/Simple/add.raw"};

Simple::ActionElement::ActionElement(ElementContainer* parent, Action::Type action, String text) : CustomElement(parent, 18, 18),action(action){

	iconActionBuffer = static_cast<Color*>(ps_malloc(18 * 18 * 2));
	if(iconActionBuffer == nullptr){
		Serial.printf("ActionElement picture %s unpack error\n", ActionIcons[action]);
		return;
	}

	fs::File actionFile = SPIFFS.open(ActionIcons[action]);
	actionFile.read(reinterpret_cast<uint8_t*>(iconActionBuffer), 18 * 18 * 2);
	actionFile.close();

}

Simple::ActionElement::~ActionElement(){
	free(iconActionBuffer);
}

void Simple::ActionElement::draw(){
	getSprite()->drawIcon(iconActionBuffer, getTotalX(), getTotalY(), 18, 18, 1, TFT_TRANSPARENT);
	if(selected && borderBuffer != nullptr){
		getSprite()->drawIcon(borderBuffer, getTotalX(), getTotalY(), 18, 18, 1, TFT_BLACK);
	}
}

void Simple::ActionElement::setIsSelected(bool selected){
	ActionElement::selected = selected;
	if(selected){
		borderBuffer = static_cast<Color*>(ps_malloc(18 * 18 * 2));
		if(borderBuffer == nullptr){
			Serial.println("Border buffer action selector unpack error");
			return;
		}
		fs::File borderFile = SPIFFS.open("/Simple/actionBorder.raw");
		borderFile.read(reinterpret_cast<uint8_t*>(borderBuffer), 18 * 18 * 2);
		borderFile.close();
	}else{
		free(borderBuffer);
		borderBuffer = nullptr;
	}
}
