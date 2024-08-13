#include "AddIcon.hpp"
#include <SPIFFS.h>
#include "../../../Fonts.h"

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
	auto canvas = getSprite();
	canvas->setFont(&u8g2_font_6x12_tr);
	canvas->setTextColor(TFT_WHITE);
	canvas->setTextDatum(textdatum_t::top_center);
	canvas->drawString("Hold BACK to delete,", canvas->width()/2, getTotalY() + 25);
	canvas->drawString("hold SELECT to play.", canvas->width()/2, getTotalY() + 35);

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
