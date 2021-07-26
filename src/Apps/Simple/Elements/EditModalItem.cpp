#include <U8g2_for_TFT_eSPI.h>
#include "EditModalItem.h"


Simple::EditModalItem::EditModalItem(ElementContainer* parent, String text, char unit, float value, float step) : CustomElement(parent, 100, 20), text(text), unit(unit), value(value), step(step){

}

Simple::EditModalItem::~EditModalItem(){

}

void Simple::EditModalItem::draw(){
	FontWriter u8f = getSprite()->startU8g2Fonts();
	u8f.setFont(u8g2_font_HelvetiPixel_tr);
	u8f.setForegroundColor(TFT_BLACK);
	u8f.setFontMode(1);
	u8f.setCursor(getTotalX(), getTotalY() + 15);
	u8f.print(text);
	u8f.setCursor(getTotalX() + 40, getTotalY() + 15);
	u8f.print(value);
	u8f.setCursor(getTotalX() + 80, getTotalY() + 15);
	u8f.print(unit);
	if(selected){
		getSprite()->drawRect(getTotalX(), getTotalY(), getWidth(), getHeight(), TFT_WHITE);
	}
}

bool Simple::EditModalItem::isSelected() const{
	return selected;
}

float Simple::EditModalItem::getValue() const{
	return value;
}

void Simple::EditModalItem::up(){
	if(text == "Speed"){
		value += step;
		value = min(value, 100.0f);
	}else{
		value += step;
		value = min(value, 3.0f);
	}


}

void Simple::EditModalItem::down(){
	if(text == "Speed"){
		value -= step;
		value = max(value, 5.0f);
	}else{
		value -= step;
		value = max(value, 0.5f);
	}
}

void Simple::EditModalItem::setSelected(bool selected){
	EditModalItem::selected = selected;
}


