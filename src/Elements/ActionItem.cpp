#include "ActionItem.h"
#include "../mem.h"
//#include "../Apps/Timeline/Bitmaps/border.hpp"

ActionItem::ActionItem(ElementContainer* parent, const uint16_t* icon, const std::string& text)
		: LinearLayout(parent, HORIZONTAL), icon(this, icon, 18, 18), text(this, 0, 18){

	borderBuffer = static_cast<Color*>(w_malloc(18 * 18 * 2));
	if(borderBuffer == nullptr){
		Serial.println("ActionItem picture /border.raw unpack error");
		return;
	}
	borderFile = SPIFFS.open("/border.raw");
	borderFile.seek(0);
	borderFile.read(reinterpret_cast<uint8_t*>(borderBuffer), 18 * 18 * 2);
	borderFile.close();

	setWHType(PARENT, FIXED);
	setHeight(this->icon.getHeight());
	setGutter(5);
	this->text.setFont(0).setColor(TFT_WHITE).setSize(1).setText(text);

	addChild(&this->icon);
	addChild(&this->text);
	reflow();
}

void ActionItem::reflow(){
	LinearLayout::reflow();
	text.setWidth(getAvailableWidth() - icon.getWidth() - 5);
}

void ActionItem::reposChildren(){
	LinearLayout::reposChildren();
	text.setY(5);
}

void ActionItem::setSelected(bool selected){
	ActionItem::selected = selected;

	text.setColor(selected ? TFT_RED : TFT_WHITE);
}

void ActionItem::draw(){
	LinearLayout::draw();

	if(selected){
		getSprite()->drawIcon(borderBuffer, getTotalX(), getTotalY(), 18, 18, 1, TFT_TRANSPARENT);
	}
}
