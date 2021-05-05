#include "ActionSelector.h"
#include <Input/Input.h>
#include "../../Wheelson.h"
#include "Timeline.h"

//#include "Bitmaps/actions.hpp"

const AutoAction::Type types[] = {
		AutoAction::Type::FORWARD, AutoAction::Type::BACKWARD, AutoAction::Type::LEFT, AutoAction::Type::RIGHT,
		AutoAction::Type::LIGHT_ON, AutoAction::Type::LIGHT_OFF, AutoAction::Type::TONE, AutoAction::Type::TUNE
};

const char* const SelectorActionSprites[] = {
		"/arrow_up.raw", "/arrow_down.raw", "/arrow_left.raw", "/arrow_right.raw", "/light_on.raw", "/light_off.raw", "/tone.raw", "/tune.raw"
};

ActionSelector* ActionSelector::instance = nullptr;

ActionSelector::ActionSelector(Timeline* timeline) : timeline(timeline), Modal(*timeline, 74, 74),
													 layers(&screen), fleha(&layers, 74, 74), actionGrid(&layers, 3),
													 selectedBorder(&layers, borderBuffer, 18, 18){

	instance = this;
	buildUI();
}

void ActionSelector::draw(){
	getScreen().draw();
}

void ActionSelector::selectAction(){
	Element* selected = actionGrid.getChild(selectedAction);
	//selectedBorder.setPos(selected->getX(), selected->getY());
}

void ActionSelector::start(){
	draw();
	screen.commit();
	Input::getInstance()->setBtnPressCallback(BTN_B, [](){
		if(instance == nullptr) return;
		instance->pop(new int(-1));
	});

	Input::getInstance()->setBtnPressCallback(BTN_A, [](){
		if(instance == nullptr) return;
		instance->pop(new int(instance->selectedAction));
	});

	Input::getInstance()->setBtnPressCallback(BTN_LEFT, [](){
		if(instance == nullptr) return;

		if(instance->selectedAction == 0){
			instance->selectedAction = sizeof(types) / sizeof(AutoAction::type) - 1;
		}else{
			instance->selectedAction--;
		}

		instance->selectAction();
		instance->draw();
	});

	Input::getInstance()->setBtnPressCallback(BTN_RIGHT, [](){
		if(instance == nullptr) return;

		instance->selectedAction = (instance->selectedAction + 1) % (sizeof(types) / sizeof(AutoAction::type));
		instance->selectAction();
		instance->draw();
	});

	draw();
}

void ActionSelector::stop(){
	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnPressCallback(BTN_B);
	Input::getInstance()->removeBtnPressCallback(BTN_LEFT);
	Input::getInstance()->removeBtnPressCallback(BTN_RIGHT);
	for(int i = 0; i < 8; i++){
		free(buffer[i]);
		buffer[i] = {nullptr};
		iconFile->close();
	}
}

void ActionSelector::unpack(){
	Context::unpack();
	selectedAction = 0;
	selectAction();
	for(int i = 0; i < 8; i++){
		buffer[i] = static_cast<Color*>(malloc(18 * 18 * 2));
		if(buffer[i] == nullptr){
			Serial.printf("ActionEditor picture %s unpack error\n", SelectorActionSprites[types[i]]);
			return;
		}
		iconFile[i] = SPIFFS.open(SelectorActionSprites[types[i]]);
		iconFile[i].seek(0);
		iconFile[i].read(reinterpret_cast<uint8_t*>(buffer), 18 * 18 * 2);
		iconFile[i].close();
	}
	borderBuffer = static_cast<Color*>(malloc(18 * 18 * 2));
	if(borderBuffer == nullptr){
		Serial.println("ActionSelector picture /border.raw unpack error");
		return;
	}
	borderFile = SPIFFS.open("/border.raw");
	borderFile.seek(0);
	borderFile.read(reinterpret_cast<uint8_t*>(borderBuffer), 18 * 18 * 2);
	borderFile.close();
}

void ActionSelector::pack(){
	Context::pack();
	for(int i = 0; i < 8; i++){
		free(buffer[i]);
		buffer[i] = nullptr;
	}
	free(borderBuffer);
	borderBuffer= nullptr;
}

void ActionSelector::fillMenu(){
	for(const auto& type : types){
		actionGrid.addChild(new BitmapElement(&actionGrid, buffer[type], 18, 18));
	}
}

void ActionSelector::buildUI(){
	fillMenu();

	layers.setWHType(FIXED, FIXED);
	layers.setWidth(74);
	layers.setHeight(74);

	layers.addChild(&fleha);
	layers.addChild(&actionGrid);
	//layers.addChild(&selectedBorder);
	layers.reflow();

	actionGrid.setWHType(PARENT, PARENT);
	actionGrid.setPadding(5);
	actionGrid.setGutter(5);
	actionGrid.reflow();
	actionGrid.repos();

	fleha.border = true;
	fleha.bgColor = TFT_DARKGREY;
	fleha.borderTopColor = fleha.borderBotColor = TFT_LIGHTGREY;

	screen.addChild(&layers);
}

