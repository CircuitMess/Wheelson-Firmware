#include "SimpleAction.h"

const char* const SimpleAction::AcionIcons[] = {"/Simple/arrow_up.raw", "/Simple/arrow_down.raw", "/Simple/arrow_left.raw", "/Simple/arrow_right.raw", "/Simple/light_off.raw", "/Simple/light_on.raw", "/Simple/add.raw"};

SimpleAction::SimpleAction(ElementContainer* parent, Action action) : CustomElement(parent, 18, 18), action(action){

	iconActionBuffer = static_cast<Color*>(ps_malloc(18 * 18 * 2));
	if(iconActionBuffer == nullptr){
		Serial.printf("SimpleAction picture %s unpack error\n", AcionIcons[action]);
		return;
	}

	fs::File actionFile = SPIFFS.open(AcionIcons[action]);
	actionFile.read(reinterpret_cast<uint8_t*>(iconActionBuffer), 18 * 18 * 2);
	actionFile.close();

}

SimpleAction::~SimpleAction(){
	free(iconActionBuffer);
}

void SimpleAction::draw(){
	getSprite()->drawIcon(iconActionBuffer, getTotalX(), getTotalY(), 18, 18, 1, TFT_TRANSPARENT);
	if(selected && borderBuffer != nullptr){
		getSprite()->drawIcon(borderBuffer, getTotalX(), getTotalY(), 18, 18, 1, TFT_BLACK);
	}
}

void SimpleAction::setIsSelected(bool selected){
	SimpleAction::selected = selected;
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
	}
}
