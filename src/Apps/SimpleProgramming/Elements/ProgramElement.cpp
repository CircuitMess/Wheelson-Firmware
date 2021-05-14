#include "ProgramElement.h"
#include <SPIFFS.h>
#include <U8g2_for_TFT_eSPI.h>

Simple::ProgramElement::ProgramElement::ProgramElement(ElementContainer* parent, String name) : CustomElement(parent, 150, 20), name(name){

}

Simple::ProgramElement::ProgramElement::~ProgramElement(){

}

void Simple::ProgramElement::ProgramElement::draw(){
	getSprite()->fillRoundRect(getTotalX(), getTotalY(), getWidth(), getHeight(), 5, C_RGB(0, 132, 255));
	getSprite()->drawRoundRect(getTotalX(), getTotalY(), getWidth(), getHeight(), 5, TFT_WHITE);
	if(selected){
		getSprite()->drawRoundRect(getTotalX(), getTotalY(), getWidth(), getHeight(), 5, TFT_RED);
	}

	FontWriter u8f = getSprite()->startU8g2Fonts();
	u8f.setFont(u8g2_font_profont12_tf);
	u8f.setForegroundColor(TFT_WHITE);
	u8f.setFontMode(1);
	u8f.setCursor(getTotalX() + 5, getTotalY() + 13);
	u8f.println(name);


}

void Simple::ProgramElement::ProgramElement::setIsSelected(bool isSelected){
	ProgramElement::selected = isSelected;

}




