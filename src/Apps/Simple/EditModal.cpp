#include "EditModal.h"
#include <Input/Input.h>
#include <Wheelson.h>


Simple::EditModal::EditModal(Context& context, Action* action) :
		Modal(context, 100, 80), layout(new LinearLayout(&screen, VERTICAL)),
		speed(new EditModalItem(layout, "Speed", '%', action->speed, 5)),
		time(new EditModalItem(layout, "Time", 's', action->time, 0.5)), action(action),
		modalBg(&screen, 100, 80, C_HEX(0x00beff), C_HEX(0x00ffff), C_HEX(0x0082ff)){

	buildUI();
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
	action->speed = (int8_t) speed->getValue();
	action->time = time->getValue();

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
		speed->setSelected(true);
	}else{
		time->setSelected(true);
	}
	layout->addChild(time);
	layout->reflow();
	screen.addChild(layout);
	screen.repos();

}

void Simple::EditModal::buttonPressed(uint id){
	if(id == Pins.get(Pin::BtnLeft)){
		if(speed->isSelected()){
			speed->down();
		}else if(time->isSelected()){
			time->down();
		}

		draw();
		screen.commit();
	}else if(id == Pins.get(Pin::BtnRight)){
		if(speed->isSelected()){
			speed->up();
		}else if(time->isSelected()){
			time->up();
		}

		draw();
		screen.commit();
	}else if(id == Pins.get(Pin::BtnUp) || id == Pins.get(Pin::BtnDown)){
		if(action->type == Action::PAUSE) return;
		if(speed->isSelected()){
			speed->setSelected(false);
			time->setSelected(true);
		}else if(time->isSelected()){
			time->setSelected(false);
			speed->setSelected(true);
		}

		draw();
		screen.commit();
	}else if(id == Pins.get(Pin::BtnMid) || id == Pins.get(Pin::BtnBack)){
		this->pop();
	}
}

