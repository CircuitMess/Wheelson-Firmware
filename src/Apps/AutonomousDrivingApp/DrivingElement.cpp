#include "DrivingElement.h"
#include <U8g2_for_TFT_eSPI.h>
#include <SPIFFS.h>

const char* const DrivingElement::Icons[] = {"/AutoDrive/engine.bw"};
bool DrivingElement::motorStop = true;

DrivingElement::DrivingElement(ElementContainer* parent, DrivingIcon icon, String text, bool needPercentage) : CustomElement(parent, 17, 15),
																											   icon(icon), text(text),
																											   needPercentage(needPercentage){
	motorStop = true;
	iconBuffer = (uint8_t*)ps_malloc(39);
	if(iconBuffer == nullptr){
		Serial.printf("Driving Element icon %s, unpack error\n", Icons[icon]);
		return;
	}
	fs::File bgFile = SPIFFS.open(Icons[icon]);
	bgFile.read(iconBuffer, 39);
	bgFile.close();

	if(needPercentage){
		percentageBuffer = static_cast<Color*>(ps_malloc(6 * 6 * 2));
		if(percentageBuffer == nullptr){
			Serial.println("Percentage icon unpack error\n");
			return;
		}
		fs::File bgFile = SPIFFS.open("/AutoDrive/percentage.raw");
		bgFile.read(reinterpret_cast<uint8_t*>(percentageBuffer), 6 * 6 * 2);
		bgFile.close();
	}
}

DrivingElement::~DrivingElement(){
	free(iconBuffer);
	free(percentageBuffer);
}

void DrivingElement::draw(){
	uint xShift = 0;
	getSprite()->drawMonochromeIcon(iconBuffer, getTotalX(), getTotalY(), 17, 13, 1, motorStop ? TFT_RED : TFT_WHITE);
	FontWriter u8f = getSprite()->startU8g2Fonts();
	u8f.setFont(u8g2_font_6x10_tn);
	u8f.setForegroundColor(TFT_WHITE);
	u8f.setFontMode(1);
	if(text.length() == 1){
		xShift = 5;
	}else if(text.length() == 2){
		xShift = 4;
	}else if(text.length() == 3){
		xShift = 1;
	}
	u8f.setCursor(getTotalX() + xShift, getTotalY() + 25);
	u8f.print(text);
	if(needPercentage){
		getSprite()->drawIcon(percentageBuffer, (getTotalX() + 5), (getTotalY() + 16), 6, 6, 1, TFT_TRANSPARENT);
	}

}

void DrivingElement::setText(const String& text){
	DrivingElement::text = text;
}

void DrivingElement::toggleMotors(){
	motorStop = !motorStop;
}
