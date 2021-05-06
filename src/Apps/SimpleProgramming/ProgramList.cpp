#include "ProgramList.h"
#include <SPIFFS.h>
#include <U8g2_for_TFT_eSPI.h>

ProgramList::ProgramList::ProgramList(ElementContainer* parent, String name) : CustomElement(parent, 150, 20), name(name){

}

ProgramList::ProgramList::~ProgramList(){

}

void ProgramList::ProgramList::draw(){
	getSprite()->fillRoundRect(getTotalX(), getTotalY(), getWidth(), getHeight(), 5, C_RGB(0, 132, 255));
	getSprite()->drawRoundRect(getTotalX(), getTotalY(), getWidth(), getHeight(), 5, TFT_WHITE);

	FontWriter u8f = getSprite()->startU8g2Fonts();
	u8f.setFont(u8g2_font_profont12_tf);
	u8f.setForegroundColor(TFT_WHITE);
	u8f.setFontMode(1);
	u8f.setCursor(getTotalX() + 5, getTotalY() + 13);
	u8f.println(name);

}

void ProgramList::ProgramList::setIsSelected(bool isSelected){
	ProgramList::selected = isSelected;

}




