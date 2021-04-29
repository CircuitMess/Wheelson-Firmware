#include "ActionEditor.h"
#include "../../defs.hpp"
#include "Timeline.h"
#include <Input/Input.h>

#include "Bitmaps/time.hpp"
#include "Bitmaps/color.hpp"
#include "Bitmaps/freq.hpp"
#include "Bitmaps/volume.hpp"
#include "../../Elements/ActionEditItem.h"


Vector<Vector<Setting>> settings = {
		{
			Setting(Setting::Type::NUMERIC, "ms", Setting::Icon::TIME,  new SettingNumeric(500, 10000, 500), offsetof(MoveParams, millis))
		},
		{
			Setting(Setting::Type::NUMERIC, "ms", Setting::Icon::TIME,  new SettingNumeric(500, 10000, 500), offsetof(MoveParams, millis))
		},
		{
			Setting(Setting::Type::NUMERIC, "ms", Setting::Icon::TIME,  new SettingNumeric(500, 10000, 500), offsetof(MoveParams, millis))
		},
		{
			Setting(Setting::Type::NUMERIC, "ms", Setting::Icon::TIME,  new SettingNumeric(500, 10000, 500), offsetof(MoveParams, millis))
		},
		{
			Setting(Setting::Type::OPTION, "",Setting::Icon::COLOR, new SettingOption({ "Red", "Blue", "Green" }), offsetof(MoveParams, millis))
		},
		{},
		{
			Setting(Setting::Type::NUMERIC, "ms", Setting::Icon::TIME,  new SettingNumeric(500, 10000, 500), offsetof(ToneParams, millis)),
			Setting(Setting::Type::NUMERIC, "Hz", Setting::Icon::FREQ,  new SettingNumeric(500, 10000, 200), offsetof(ToneParams, frequency)),
			Setting(Setting::Type::NUMERIC, "", Setting::Icon::VOLUME,  new SettingNumeric(1, 10, 1), offsetof(ToneParams, volume))
		},
		{
			Setting(Setting::Type::OPTION, "", Setting::Icon::COLOR,  new SettingOption({ "HONK", "CHONK", "NewFresh", "SimpleMini" }), offsetof(TuneParams, tune)),
			Setting(Setting::Type::NUMERIC, "", Setting::Icon::VOLUME,  new SettingNumeric(1, 10, 1), offsetof(TuneParams, volume))
		}
};


ActionEditor* ActionEditor::instance = nullptr;

ActionEditor::ActionEditor(Timeline* timeline) : Modal(*timeline, 90, 90), timeline(timeline),
												 layers(&screen), fleha(&layers, 90, 90), scroll(&layers), list(&scroll, VERTICAL){

	instance = this;
	buildUI();
}

void ActionEditor::initAction(AutoAction::Type type, AutoAction* action){
	this->action = action;

	for(auto& item : list.getChildren()){
		delete item;
	}

	list.getChildren().clear();

	for(const auto& setting : settings[type]){
		volatile byte* bptr = (byte*) action->params;
		volatile void* vptr = bptr + setting.offset;
		ActionEditItem* item = new ActionEditItem(&list, &setting, (void*) vptr);
		list.addChild(item);
	}

	reinterpret_cast<ActionEditItem*>(instance->list.getChildren()[0])->setSelected(true);

	list.reflow();
	list.repos();
}

void ActionEditor::draw(){
	screen.draw();
	screen.commit();
}

void ActionEditor::start(){
	auto ret = [](){
		if(instance == nullptr) return;
		instance->pop();
	};

	Input::getInstance()->setBtnPressCallback(BTN_A, ret);
	Input::getInstance()->setBtnPressCallback(BTN_B, ret);

	Input::getInstance()->setBtnPressCallback(BTN_RIGHT, [](){
		if(instance == nullptr) return;
		reinterpret_cast<ActionEditItem*>(instance->list.getChildren()[instance->selectedSetting])->trig();
		instance->draw();
	});

	Input::getInstance()->setBtnPressCallback(BTN_LEFT, [](){
		if(instance == nullptr) return;
		reinterpret_cast<ActionEditItem*>(instance->list.getChildren()[instance->selectedSetting])->trigAlt();
		instance->draw();
	});

	Input::getInstance()->setBtnPressCallback(BTN_UP, [](){
		if(instance == nullptr) return;

		uint selected;
		if(instance->selectedSetting == 0){
			selected = instance->list.getChildren().size() - 1;
		}else{
			selected = instance->selectedSetting - 1;
		}

		instance->selectSetting(selected);
		instance->scroll.scrollIntoView(instance->selectedSetting, 5);
		instance->draw();
	});

	Input::getInstance()->setBtnPressCallback(BTN_DOWN, [](){
		if(instance == nullptr) return;

		uint selected = (instance->selectedSetting + 1) % instance->list.getChildren().size();

		instance->selectSetting(selected);
		instance->scroll.scrollIntoView(selected, 5);
		instance->draw();
	});

	draw();
}

void ActionEditor::stop(){
	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnPressCallback(BTN_B);
	Input::getInstance()->removeBtnPressCallback(BTN_LEFT);
	Input::getInstance()->removeBtnPressCallback(BTN_RIGHT);
	Input::getInstance()->removeBtnPressCallback(BTN_UP);
	Input::getInstance()->removeBtnPressCallback(BTN_DOWN);

	action = nullptr;
}

void ActionEditor::unpack(){
	Context::unpack();
	selectSetting(0);
	scroll.setScroll(0, 0);
}

void ActionEditor::selectSetting(uint i){
	reinterpret_cast<ActionEditItem*>(list.getChildren()[selectedSetting])->setSelected(false);
	selectedSetting = i;
	reinterpret_cast<ActionEditItem*>(list.getChildren()[selectedSetting])->setSelected(true);
}

void ActionEditor::fillMenu(){

}

void ActionEditor::buildUI(){
	layers.setWHType(PARENT, PARENT);
	layers.addChild(&fleha);
	layers.addChild(&scroll);
	layers.reflow();

	scroll.setWHType(PARENT, PARENT);
	scroll.addChild(&list);
	scroll.reflow();

	list.setWHType(PARENT, CHILDREN);
	list.setPadding(5);
	list.setGutter(5);
	list.reflow();

	fleha.border = true;
	fleha.bgColor = TFT_DARKGREY;
	fleha.borderTopColor = fleha.borderBotColor = TFT_LIGHTGREY;

	screen.addChild(&layers);
}
