#include "ActionSelector.h"
#include <Input/Input.h>
#include "Timeline.h"
#include <Wheelson.h>

ActionSelector* ActionSelector::instance = nullptr;

const char* const ActionSelector::ModalActions[] = {"/Simple/arrow_up.raw", "/Simple/arrow_down.raw", "/Simple/arrow_left.raw", "/Simple/arrow_right.raw", "/Simple/light_off.raw", "/Simple/light_on.raw"};

ActionSelector::ActionSelector(Context& context) : Modal(context, 100, 80), gridLayout(new GridLayout(&screen, 3)){

	for(int i = 0; i < 6; i++){
		actions.push_back(new SimpleAction(gridLayout, static_cast<Action>(i)));
	}
	actions[0]->setIsSelected(true);
	buildUI();

}

ActionSelector::~ActionSelector(){

}

void ActionSelector::draw(){
	screen.getSprite()->clear(TFT_GREEN);
	screen.getSprite()->fillRoundRect(0, 0, screen.getWidth() - 1, screen.getHeight() - 1, 5, TFT_DARKGREY);
	screen.getSprite()->drawRoundRect(0, 0, screen.getWidth() - 1, screen.getHeight() - 1, 5, TFT_WHITE);
	screen.draw();
}

void ActionSelector::start(){
	Input::getInstance()->addListener(this);
	draw();
	screen.commit();

}

void ActionSelector::stop(){
	Input::getInstance()->removeListener(this);
}

void ActionSelector::pack(){
	Context::pack();
	for(int i = 0; i < 6; i++){
		free(actionBuffer[i]);
	}
	free(borderBuffer);

}

void ActionSelector::unpack(){
	Context::unpack();
	selectedAction = 0;
	for(int i = 0; i < 6; i++){
		actionBuffer[i] = static_cast<Color*>(ps_malloc(18 * 18 * 2));
		if(actionBuffer[i] == nullptr){
			Serial.printf("Action Selector picture %s unpack error\n", ModalActions[i]);
			return;
		}
		fs::File actionFile[i];
		actionFile[i] = SPIFFS.open(ModalActions[i]);
		actionFile[i].read(reinterpret_cast<uint8_t*>(actionBuffer[i]), 18 * 18 * 2);
		actionFile[i].close();
	}

}

void ActionSelector::buildUI(){
	gridLayout->setWHType(CHILDREN, CHILDREN);
	gridLayout->setGutter(8);
	for(int i = 0; i < actions.size(); i++){
		gridLayout->addChild(actions[i]);
	}
	gridLayout->reflow();
	screen.addChild(gridLayout);
	screen.repos();
	gridLayout->setX(screen.getTotalX() + 15);
	gridLayout->setY(screen.getTotalY() + 15);
}

void ActionSelector::selectApp(int8_t num){
	actions[selectedAction]->setIsSelected(false);
	selectedAction = num;
	actions[selectedAction]->setIsSelected(true);
}

void ActionSelector::buttonPressed(uint id){
	switch(id){
		case BTN_LEFT:
			if(selectedAction == 0){
				selectApp(5);
			}else{
				selectApp(selectedAction - 1);
			}

			draw();
			screen.commit();
			break;

		case BTN_RIGHT:
			if(selectedAction == 5){
				selectApp(0);
			}else{
				selectApp(selectedAction + 1);
			}

			draw();
			screen.commit();
			break;

		case BTN_UP:
			if(selectedAction == 0 || selectedAction < 3){
				selectApp(5);
			}else{
				selectApp(selectedAction - 3);
			}
			draw();
			screen.commit();
			break;

		case BTN_DOWN:
			if(selectedAction == 5 || selectedAction > 2){
				selectApp(0);
			}else{
				selectApp(selectedAction + 3);
			}
			draw();
			screen.commit();
			break;

		case BTN_MID:
			this->pop(new uint8_t(selectedAction));
			break;
		case BTN_BACK:
			this->pop();
			break;

	}
}



