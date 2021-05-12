#include <FS/CompressedFile.h>
#include "SimpleEdit.h"
#include "SimpleAction.h"
#include "../Timeline/ActionSelector.h"
#include <Wheelson.h>
#include <Input/Input.h>

SimpleEdit* SimpleEdit::instance = nullptr;

SimpleEdit::SimpleEdit(Display& display) : Context(display), scrollLayout(new ScrollLayout(&screen)), list(new GridLayout(scrollLayout, 5)){
	instance = this;
	actions.push_back(new SimpleAction(list, static_cast<Action>(6)));
	actions[0]->setIsSelected(true);

	buildUI();
	SimpleEdit::pack();

}

SimpleEdit::~SimpleEdit(){
	instance = nullptr;
}

void SimpleEdit::start(){
	Input::getInstance()->addListener(this);
	draw();
	screen.commit();

}

void SimpleEdit::stop(){
	Input::getInstance()->removeListener(this);
}

void SimpleEdit::draw(){
	screen.getSprite()->drawIcon(backgroundBuffer, 0, 0, 160, 128, 1);
	screen.draw();
}

void SimpleEdit::pack(){
	Context::pack();
	free(backgroundBuffer);
}

void SimpleEdit::unpack(){
	Context::unpack();
	backgroundBuffer = static_cast<Color*>(ps_malloc(160 * 128 * 2));
	if(backgroundBuffer == nullptr){
		Serial.println("SimpleEdit background picture unpack error");
		return;
	}
	fs::File backgroundFile = CompressedFile::open(SPIFFS.open("/Simple/simple_edit_bg.raw.hs"), 12, 8);
	backgroundFile.read(reinterpret_cast<uint8_t*>(backgroundBuffer), 160 * 128 * 2);
	backgroundFile.close();

}

void SimpleEdit::buildUI(){
	scrollLayout->setWHType(FIXED, PARENT);
	scrollLayout->setWidth(130);
	scrollLayout->addChild(list);
	//scrollLayout->setBorder(2, TFT_RED);

	list->setWHType(CHILDREN, CHILDREN);
	list->setY(100);
	list->setPadding(5);
	list->setGutter(8);
	list->addChild(actions[0]);

	scrollLayout->reflow();
	list->reflow();

	screen.addChild(scrollLayout);
	screen.repos();
	scrollLayout->setX(screen.getTotalX() + 15);

}

void SimpleEdit::loop(uint micros){

}

void SimpleEdit::selectAction(uint8_t num){
	actions[actionNum]->setIsSelected(false);
	actionNum = num;
	actions[actionNum]->setIsSelected(true);
}

void SimpleEdit::buttonPressed(uint id){
	uint8_t totalNumActions = actions.size();
	switch(id){
		case BTN_LEFT:
			if(actionNum == 0){
				selectAction(totalNumActions - 1);
			}else{
				selectAction(actionNum - 1);
			}
			Serial.println(actionNum);

			scrollLayout->scrollIntoView(actionNum, 5);
			draw();
			screen.commit();
			break;

		case BTN_RIGHT:
			if(actionNum == totalNumActions - 1){
				selectAction(0);
			}else{
				selectAction(actionNum + 1);
			}
			Serial.println(actionNum);

			scrollLayout->scrollIntoView(actionNum, 5);
			draw();
			screen.commit();
			break;

		case BTN_UP:
			if(actionNum < 5){
				selectAction(totalNumActions - 1);
			}else{
				selectAction(actionNum - 5);
			}
			Serial.println(actionNum);

			scrollLayout->scrollIntoView(actionNum, 5);
			draw();
			screen.commit();
			break;

		case BTN_DOWN:
			if(totalNumActions - actionNum <= 5){
				selectAction(0);
			}else{
				selectAction(actionNum + 5);
			}
			Serial.println(actionNum);

			scrollLayout->scrollIntoView(actionNum, 5);
			draw();
			screen.commit();
			break;

		case BTN_MID:
			if(totalNumActions == 1 || actionNum == totalNumActions - 1){
				ActionSelector* popUpModul = new ActionSelector(*this);
				popUpModul->push(this);
			}
			break;
	}
}

void SimpleEdit::returned(void* data){
	Context::returned(data);
	uint8_t* podatakPtr = static_cast<uint8_t*>(data);

	uint8_t podatak = *podatakPtr;
	delete podatakPtr;

	actions.push_back(new SimpleAction(list, static_cast<Action>(podatak)));
	Serial.println(podatak);
	list->addChild(actions.back());
	list->getChildren().swap(list->getChildren().size() - 1, list->getChildren().size() - 2);
	actions.swap(actions.size() - 1, actions.size() - 2);
	actionNum = actions.size() - 1;
	list->reflow();
	screen.repos();
	scrollLayout->scrollIntoView(list->getChildren().size() - 1, 5);
	scrollLayout->setX(screen.getTotalX() + 15);
}

