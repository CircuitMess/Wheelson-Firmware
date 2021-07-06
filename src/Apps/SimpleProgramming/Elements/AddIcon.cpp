#include "AddIcon.hpp"
#include <U8g2_for_TFT_eSPI.h>
#include <SPIFFS.h>


Simple::AddIcon::AddIcon(ElementContainer* parent) : CustomElement(parent, 15, 15){
	addBuffer = static_cast<Color*>(ps_malloc(15 * 15 * 2));
	if(addBuffer == nullptr){
		Serial.printf("App add_button picture unpack error\n");
		return;
	}
	fs::File addFile = SPIFFS.open("/Simple/add_button.raw");

	addFile.read(reinterpret_cast<uint8_t*>(addBuffer), 15 * 15 * 2);
	addFile.close();
}

Simple::AddIcon::~AddIcon(){
	free(addBuffer);
}

void Simple::AddIcon::draw(){
	getSprite()->drawIcon(addBuffer, getTotalX(), getTotalY()+3, getWidth(), getHeight(), 1, TFT_TRANSPARENT);
	if(selected && borderBuffer != nullptr){
		getSprite()->drawIcon(borderBuffer, getTotalX(), getTotalY()+3, getWidth(), getHeight(), 1, TFT_TRANSPARENT);
	}
	FontWriter u8f = getSprite()->startU8g2Fonts();
	u8f.setFont(u8g2_font_6x12_tr);
	u8f.setForegroundColor(TFT_WHITE);
	u8f.setFontMode(1);
	u8f.setCursor((160 - u8f.getUTF8Width("Hold BACK to delete,")) / 2, getTotalY() + 35);
	u8f.println("Hold BACK to delete,");
	u8f.setCursor((160 - u8f.getUTF8Width("hold SELECT to play")) / 2, getTotalY() + 45);
	u8f.println("hold SELECT to play");

}

void Simple::AddIcon::setSelected(bool selected){
	AddIcon::selected = selected;
	if(selected){
		borderBuffer = static_cast<Color*>(ps_malloc(15 * 15 * 2));
		if(borderBuffer == nullptr){
			Serial.printf("App add border picture unpack error\n");
			return;
		}
		fs::File addFile = SPIFFS.open("/Simple/border15x15.raw");

		addFile.read(reinterpret_cast<uint8_t*>(borderBuffer), 15 * 15 * 2);
		addFile.close();
	}else{
		free(borderBuffer);
		borderBuffer= nullptr;
	}
}
