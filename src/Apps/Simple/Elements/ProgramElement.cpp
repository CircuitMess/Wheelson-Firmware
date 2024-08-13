#include "ProgramElement.h"
#include <SPIFFS.h>
#include "../../../Fonts.h"

Simple::ProgramElement::ProgramElement::ProgramElement(ElementContainer* parent, String name) : CustomElement(parent, 150, 20), name(name){

}

Simple::ProgramElement::ProgramElement::~ProgramElement(){

}

void Simple::ProgramElement::ProgramElement::draw(){
	getSprite()->fillRoundRect(getTotalX(), getTotalY(), getWidth(), getHeight(), 5, C_RGB(0, 132, 255));

	if(touchStartTime != 0){
		float d = (float) (millis() - touchStartTime) / 1000.0f;
		d = min(d, 1.0f);
		if(d > 0.1){
			d = (d - 0.1) * (1.0 / 0.9);
			getSprite()->fillRoundRect(getTotalX(), getTotalY(), (getWidth()+2) * d, getHeight(), 5, touchColor);
		}
	}

	getSprite()->drawRoundRect(getTotalX()-1, getTotalY(), getWidth()+2, getHeight(), 5, selected ? TFT_RED : TFT_WHITE);

	auto canvas = getSprite();
	canvas->setFont(&u8g2_font_profont12_tf);
	canvas->setTextColor(TFT_WHITE);
	canvas->setTextDatum(textdatum_t::top_left);
	canvas->drawString(name, getTotalX() + 5, getTotalY() + 3);
}

void Simple::ProgramElement::ProgramElement::setIsSelected(bool isSelected){
	ProgramElement::selected = isSelected;

}

void Simple::ProgramElement::touchStart(Color color){
	touchColor = color;
	touchStartTime = millis();
}

void Simple::ProgramElement::touchEnd(){
	touchStartTime = 0;
}
