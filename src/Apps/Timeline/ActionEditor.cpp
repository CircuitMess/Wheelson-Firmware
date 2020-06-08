#include "ActionEditor.h"
#include "../../defs.hpp"
#include "Timeline.h"

#include "Bitmaps/time.hpp"
#include "Bitmaps/color.hpp"
#include "Bitmaps/freq.hpp"
#include "Bitmaps/volume.hpp"
#include "../../Elements/ActionEditItem.h"


Vector<Vector<Setting>> settings = {
		{
			Setting(Setting::Type::NUMERIC, "ms", icon_time,  new SettingNumeric(500, 2000, 100))
		},
		{
			Setting(Setting::Type::NUMERIC, "ms", icon_time,  new SettingNumeric(500, 2000, 100))
		},
		{
			Setting(Setting::Type::NUMERIC, "ms", icon_time,  new SettingNumeric(500, 2000, 100))
		},
		{
			Setting(Setting::Type::NUMERIC, "ms", icon_time,  new SettingNumeric(500, 2000, 100))
		},
		{
			Setting(Setting::Type::OPTION, "", icon_color, new SettingOption({ "Red", "Blue", "Green" }))
		},
		{},
		{
			Setting(Setting::Type::NUMERIC, "ms", icon_time,  new SettingNumeric(500, 2000, 100)),
			Setting(Setting::Type::NUMERIC, "Hz", icon_freq,  new SettingNumeric(500, 5000, 500)),
			Setting(Setting::Type::NUMERIC, "", icon_volume,  new SettingNumeric(1, 10, 1))
		},
		{
			Setting(Setting::Type::OPTION, "", icon_color,  new SettingOption({ "HONK", "CHONK", "NewFresh", "SimpleMini" })),
			Setting(Setting::Type::NUMERIC, "", icon_volume,  new SettingNumeric(1, 10, 1))
		}
};


ActionEditor* ActionEditor::instance = nullptr;

ActionEditor::ActionEditor(Timeline* timeline) : Modal(*timeline, 90, 90), timeline(timeline),
												 layers(&screen), fleha(&layers, 90, 90, true), scroll(&layers), list(&scroll, VERTICAL){

	instance = this;
	buildUI();
}

void ActionEditor::initAction(AutoAction::Type type){
	for(auto& item : list.getChildren()){
		delete item;
	}

	list.getChildren().clear();

	for(const auto& setting : settings[type]){
		ActionEditItem* item = new ActionEditItem(&list, &setting);
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
	Input::getInstance()->setBtnPressCallback(BTN_A, [](){
		if(instance == nullptr) return;
		instance->pop();
	});

	Input::getInstance()->setBtnPressCallback(BTN_B, [](){
		if(instance == nullptr) return;
		reinterpret_cast<ActionEditItem*>(instance->list.getChildren()[instance->selectedSetting])->trig();
		instance->draw();
	});

	Input::getInstance()->setButtonHeldCallback(BTN_B, 300, [](){
		if(instance == nullptr) return;
		reinterpret_cast<ActionEditItem*>(instance->list.getChildren()[instance->selectedSetting])->trigAlt();
		instance->draw();
	});

	Input::getInstance()->setBtnPressCallback(BTN_C, [](){
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

	Input::getInstance()->setBtnPressCallback(BTN_D, [](){
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
	//Input::getInstance()->removeButtonHeldCallback(BTN_B);
	Input::getInstance()->removeBtnPressCallback(BTN_B);
	Input::getInstance()->removeBtnPressCallback(BTN_C);
	Input::getInstance()->removeBtnPressCallback(BTN_D);
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

	screen.addChild(&layers);
}
