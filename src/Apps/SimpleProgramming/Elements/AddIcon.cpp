#include "AddIcon.hpp"


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
	getSprite()->drawIcon(addBuffer, getTotalX(), getTotalY(), getWidth(), getHeight(), 1, TFT_TRANSPARENT);
	if(selected && borderBuffer != nullptr){
		getSprite()->drawIcon(borderBuffer, getTotalX(), getTotalY(), getWidth(), getHeight(), 1, TFT_TRANSPARENT);
	}

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
	}
}
