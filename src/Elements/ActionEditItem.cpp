#include <sstream>
#include "ActionEditItem.h"
#include "../Apps/Timeline/Bitmaps/border.hpp"

ActionEditItem::ActionEditItem(ElementContainer* parent, const Setting* setting) : setting(setting),
		LinearLayout(parent, HORIZONTAL), icon(this, setting->icon, 18, 18), text(this, 0, 18){

	setWHType(PARENT, FIXED);
	setHeight(this->icon.getHeight());
	setGutter(5);

	switch(setting->type){
		case Setting::Type::NUMERIC:
			value = static_cast<SettingNumeric*>(setting->params)->min;
			break;
		case Setting::Type::OPTION:
		case Setting::Type::BOOLEAN:
			value = 0;
			break;
	}

	text.setFont(0).setColor(TFT_WHITE).setSize(1);
	setText();

	addChild(&this->icon);
	addChild(&this->text);
	reflow();
}

void ActionEditItem::trig(){
	switch(setting->type){
		case Setting::Type::NUMERIC: {
			const SettingNumeric* params = static_cast<const SettingNumeric*>(setting->params);
			value = min(value + params->step, params->max);
			break;
		}
		case Setting::Type::OPTION: {
			const SettingOption* params = static_cast<const SettingOption*>(setting->params);
			value = (value + 1) % params->options.size();
			break;
		}
		case Setting::Type::BOOLEAN: {
			value = !value;
			break;
		}
	}

	setText();
}

void ActionEditItem::trigAlt(){
	switch(setting->type){
		case Setting::Type::NUMERIC: {
			const SettingNumeric* params = static_cast<const SettingNumeric*>(setting->params);
			// value = max(value - params->step * 2, params->min);
			value = params->min;
			break;
		}
		case Setting::Type::OPTION: {
			const SettingOption* params = static_cast<const SettingOption*>(setting->params);
			if(value == 0){
				value = params->options.size() - 1;
			}else{
				value--;
			}
			break;
		}
		case Setting::Type::BOOLEAN:
			break;
	}

	setText();
}

void ActionEditItem::setText(){
	std::ostringstream stream;

	switch(setting->type){
		case Setting::Type::NUMERIC: {
			stream << value;
			break;
		}
		case Setting::Type::OPTION: {
			const SettingOption* params = static_cast<const SettingOption*>(setting->params);
			stream << params->options[value];
			break;
		}
		case Setting::Type::BOOLEAN:
			stream << (value ? "Y" : "N") << "ay";
			break;
	}

	stream << setting->ps;
	text.setText(stream.str());
}

void ActionEditItem::reflow(){
	LinearLayout::reflow();
	uint width = getAvailableWidth();
	text.setWidth(width - icon.getWidth() - 5);
}

void ActionEditItem::reposChildren(){
	LinearLayout::reposChildren();
	text.setY(5);
}

void ActionEditItem::setSelected(bool selected){
	ActionEditItem::selected = selected;

	text.setColor(selected ? TFT_RED : TFT_WHITE);
}

void ActionEditItem::draw(){
	LinearLayout::draw();

	if(selected){
		getSprite()->drawIcon(border, getTotalX(), getTotalY(), 18, 18, 1, TFT_TRANSPARENT);
	}
}