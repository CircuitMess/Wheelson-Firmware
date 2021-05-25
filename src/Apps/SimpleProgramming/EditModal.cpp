#include "EditModal.h"
#include <Input/Input.h>
#include <Wheelson.h>


Simple::EditModal::EditModal(Context& context, Action* action) : Modal(context, 100, 100), layout(new LinearLayout(&screen, VERTICAL)), speed(new EditModalItem(layout, "Speed", '%', action->speed, 5)),
																 time(new EditModalItem(layout, "Time", 's', action->time, 0.5)), action(action), modalBg(&screen,100,100){

	buildUI();
	speed->setSelected(true);
}

Simple::EditModal::~EditModal(){

}

void Simple::EditModal::draw(){
	modalBg.draw();
	screen.draw();
}

void Simple::EditModal::start(){
	Input::getInstance()->addListener(this);
	draw();
	screen.commit();

}

void Simple::EditModal::stop(){
	Input::getInstance()->removeListener(this);

}

void Simple::EditModal::init(){
	Context::init();

}

void Simple::EditModal::deinit(){
	Context::deinit();

}

void Simple::EditModal::buildUI(){
	layout->setWHType(CHILDREN, CHILDREN);
	layout->setGutter(8);
	if(action->type != Action::PAUSE){
		layout->addChild(speed);
	}
	layout->addChild(time);
	layout->reflow();
	screen.addChild(layout);
	screen.repos();

}

void Simple::EditModal::buttonPressed(uint id){
	switch(id){
		case BTN_LEFT:
			if(speed->isSelected()){
				speed->down();
			}else if(time->isSelected()){
				time->down();
			}

			draw();
			screen.commit();
			break;

		case BTN_RIGHT:
			if(speed->isSelected()){
				speed->up();
			}else if(time->isSelected()){
				time->up();
			}

			draw();
			screen.commit();
			break;

		case BTN_UP:
			if(speed->isSelected()){
				speed->setSelected(false);
				time->setSelected(true);
			}else if(time->isSelected()){
				time->setSelected(false);
				speed->setSelected(true);
			}

			draw();
			screen.commit();
			break;

		case BTN_DOWN:
			if(speed->isSelected()){
				speed->setSelected(false);
				time->setSelected(true);
			}else if(time->isSelected()){
				time->setSelected(false);
				speed->setSelected(true);
			}

			draw();
			screen.commit();
			break;

		case BTN_MID:
			this->pop();
			break;
		case BTN_BACK:
			this->pop();
			break;

	}
}

