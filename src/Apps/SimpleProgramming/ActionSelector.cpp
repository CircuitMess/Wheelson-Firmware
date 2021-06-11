#include "ActionSelector.h"
#include <Input/Input.h>
#include <Wheelson.h>


Simple::ActionSelector* Simple::ActionSelector::instance = nullptr;

Simple::ActionSelector::ActionSelector(Context& context) : Modal(context, 100, 100), gridLayout(new GridLayout(&screen, 3)), modalBg(&screen, 100, 100){

	for(int i = 0; i < 7; i++){
		actions.push_back(new ActionElement(gridLayout, static_cast<Action::Type>(i)));
	}
	actions[0]->setIsSelected(true);
	buildUI();

}

Simple::ActionSelector::~ActionSelector(){

}

void Simple::ActionSelector::draw(){
	screen.getSprite()->clear(TFT_TRANSPARENT);
	modalBg.draw();
	screen.draw();
}

void Simple::ActionSelector::start(){
	Input::getInstance()->addListener(this);
	draw();
	screen.commit();

}

void Simple::ActionSelector::stop(){
	Input::getInstance()->removeListener(this);
}

void Simple::ActionSelector::init(){

}

void Simple::ActionSelector::deinit(){
}

void Simple::ActionSelector::buildUI(){
	gridLayout->setWHType(CHILDREN, CHILDREN);
	gridLayout->setGutter(8);
	for(int i = 0; i < actions.size(); i++){
		gridLayout->addChild(actions[i]);
	}
	gridLayout->reflow();
	screen.addChild(gridLayout);
	screen.repos();
	actions[6]->setX(screen.getTotalX() + 26);
	gridLayout->setX(screen.getTotalX() + 15);
	gridLayout->setY(screen.getTotalY() + 15);

}

void Simple::ActionSelector::selectApp(int8_t num){
	actions[selectedAction]->setIsSelected(false);
	selectedAction = num;
	actions[selectedAction]->setIsSelected(true);
}

void Simple::ActionSelector::buttonPressed(uint id){
	switch(id){
		case BTN_LEFT:
			if(selectedAction == 0){
				selectApp(6);
			}else{
				selectApp(selectedAction - 1);
			}

			draw();
			screen.commit();
			break;

		case BTN_RIGHT:
			if(selectedAction == 6){
				selectApp(0);
			}else{
				selectApp(selectedAction + 1);
			}

			draw();
			screen.commit();
			break;

		case BTN_UP:
			if(selectedAction < 3){
				selectApp(6);
			}else if(selectedAction == 6){
				selectApp(4);
			}else{
				selectApp(selectedAction - 3);
			}
			draw();
			screen.commit();
			break;

		case BTN_DOWN:
			if(selectedAction == 6){
				selectApp(0);
			}else if(selectedAction >= 3){
				selectApp(6);
			}else if(selectedAction > 2){
				selectApp(selectedAction - 3);
			}else{
				selectApp(selectedAction + 3);
			}
			draw();
			screen.commit();
			break;

		case BTN_MID:
			this->pop(new Action::Type((Action::Type) selectedAction));
			break;
		case BTN_BACK:
			this->pop();
			break;

	}
}



