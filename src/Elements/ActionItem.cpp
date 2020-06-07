#include "ActionItem.h"
#include "../Apps/Timeline/Bitmaps/border.hpp"

ActionItem::ActionItem(ElementContainer* parent, const uint16_t* icon, const std::string& text)
		: LinearLayout(parent, HORIZONTAL), icon(this, icon, 18, 18), text(this, 0, 18){

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
		getSprite()->drawIcon(border, getTotalX(), getTotalY(), 18, 18, 1, TFT_TRANSPARENT);
	}
}
