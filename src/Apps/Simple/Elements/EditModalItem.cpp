#include "EditModalItem.h"
#include "../../../Fonts.h"

Simple::EditModalItem::EditModalItem(ElementContainer* parent, String text, char unit, float value, float step) : CustomElement(parent, 100, 20), text(text), unit(unit), value(value), step(step){

}

Simple::EditModalItem::~EditModalItem(){

}

void Simple::EditModalItem::draw(){
	auto canvas = getSprite();
	canvas->setFont(&u8g2_font_HelvetiPixel_tr);
	canvas->setTextColor(TFT_BLACK);
	canvas->setTextDatum(textdatum_t::top_left);
	canvas->drawString(text, getTotalX() + 2, getTotalY() + 5);
	canvas->drawString(String(value), getTotalX() + 40, getTotalY() + 5);
	canvas->drawString(String(unit), getTotalX() + 80, getTotalY() + 5);
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


