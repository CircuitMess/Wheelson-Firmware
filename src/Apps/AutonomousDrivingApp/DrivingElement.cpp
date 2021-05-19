#include "DrivingElement.h"

const char* const DrivingElement::Icons[] = {"/AutoDrive/engine.raw"};

DrivingElement::DrivingElement(ElementContainer* parent, DrivingIcon icon) : CustomElement(parent, 17, 13), icon(icon){

	iconBuffer = static_cast<Color*>(ps_malloc(17 * 13 * 2));
	if(iconBuffer == nullptr){
		Serial.printf("Driving Element icon %s, unpack error\n", Icons[icon]);
		return;
	}
	fs::File bgFile = SPIFFS.open(Icons[icon]);
	bgFile.read(reinterpret_cast<uint8_t*>(iconBuffer), 17 * 13 * 2);
	bgFile.close();
}

DrivingElement::~DrivingElement(){
	free(iconBuffer);
}

void DrivingElement::draw(){
	getSprite()->drawIcon(iconBuffer,getTotalX(),getTotalY(),17,13,1,TFT_TRANSPARENT);
}
