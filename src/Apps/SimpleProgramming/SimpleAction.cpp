#include "SimpleAction.h"

const char* const SimpleAction::AcionIcons[] = {"/Simple/add.raw", "/Simple/arrow_up.raw", "/Simple/arrow_down.raw", "/Simple/arrow_left.raw", "/Simple/arrow_right.raw", "/Simple/light_off.raw", "/Simple/light_on.raw"};

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
	if(selected){
		getSprite()->drawRect(getTotalX(), getTotalY(), 18, 18, TFT_RED);
	}
}

void SimpleAction::setIsSelected(bool selected){
	SimpleAction::selected = selected;
}
