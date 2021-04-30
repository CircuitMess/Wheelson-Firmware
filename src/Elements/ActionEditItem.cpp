#include <sstream>
#include "../mem.h"
#include "ActionEditItem.h"
//#include "../Apps/Timeline/Bitmaps/border.hpp"

const char* const ActionEditItem::SettingsSprites[] = {
		"/time.raw", "/color.raw", "/freq.raw", "/volume.raw"
};

ActionEditItem::ActionEditItem(ElementContainer* parent, const Setting* setting, void* valptr) : setting(setting), value((int*) valptr),
																								 LinearLayout(parent, HORIZONTAL), text(this, 0, 18){

	buffer = static_cast<Color*>(w_malloc(18 * 18 * 2));
	if(buffer == nullptr){
		Serial.printf("ActionEditor picture %s unpack error\n", SettingsSprites[setting->iconType]);
		return;
	}
	iconFile = SPIFFS.open(SettingsSprites[setting->iconType]);
	iconFile.seek(0);
	iconFile.read(reinterpret_cast<uint8_t*>(buffer), 18 * 18 * 2);
	iconFile.close();

	borderBuffer = static_cast<Color*>(w_malloc(18 * 18 * 2));
	if(borderBuffer == nullptr){
		Serial.println("ActionEditor picture /border.raw unpack error");
		return;
	}
	borderFile = SPIFFS.open("/border.raw");
	borderFile.seek(0);
	borderFile.read(reinterpret_cast<uint8_t*>(borderBuffer), 18 * 18 * 2);
	borderFile.close();

	setWHType(PARENT, FIXED);
	setHeight(18);
	setGutter(5);

	/*switch(setting->type){
		case Setting::Type::NUMERIC:
			*value = static_cast<SettingNumeric*>(setting->params)->min;
			break;
		case Setting::Type::OPTION:
		case Setting::Type::BOOLEAN:
			*value = 0;
			break;
	}*/

	text.setFont(0).setColor(TFT_WHITE).setSize(1);
	setText();

	addChild(reinterpret_cast<Element*>(&this->buffer));
	addChild(&this->text);
	reflow();
}

ActionEditItem::~ActionEditItem(){
	free(buffer);
	buffer = nullptr;
}

void ActionEditItem::trig(){
	switch(setting->type){
		case Setting::Type::NUMERIC:{
			const SettingNumeric* params = static_cast<const SettingNumeric*>(setting->params);
			*value = min(*value + params->step, params->max);
			break;
		}
		case Setting::Type::OPTION:{
			const SettingOption* params = static_cast<const SettingOption*>(setting->params);
			*value = (*value + 1) % params->options.size();
			break;
		}
		case Setting::Type::BOOLEAN:{
			*value = !*value;
			break;
		}
	}

	setText();
}

void ActionEditItem::trigAlt(){
	switch(setting->type){
		case Setting::Type::NUMERIC:{
			const SettingNumeric* params = static_cast<const SettingNumeric*>(setting->params);
			*value = max(*value - params->step, params->min);
			break;
		}
		case Setting::Type::OPTION:{
			const SettingOption* params = static_cast<const SettingOption*>(setting->params);
			if(*value == 0){
				*value = params->options.size() - 1;
			}else{
				(*value)--;
			}
			break;
		}
		case Setting::Type::BOOLEAN:
			*value = !*value;
			break;
	}

	setText();
}

void ActionEditItem::setText(){
	std::ostringstream stream;

	switch(setting->type){
		case Setting::Type::NUMERIC:{
			stream << *value;
			break;
		}
		case Setting::Type::OPTION:{
			const SettingOption* params = static_cast<const SettingOption*>(setting->params);
			stream << params->options[*value];
			break;
		}
		case Setting::Type::BOOLEAN:
			stream << (*value ? "Y" : "N") << "ay";
			break;
	}

	stream << setting->ps;
	text.setText(stream.str());
}

void ActionEditItem::reflow(){
	LinearLayout::reflow();
	uint width = getAvailableWidth();
	text.setWidth(width - 18 - 5);
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
		getSprite()->drawIcon(borderBuffer, getTotalX(), getTotalY(), 18, 18, 1, TFT_TRANSPARENT);
	}
}

