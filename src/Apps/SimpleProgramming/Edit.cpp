#include <FS/CompressedFile.h>
#include "Edit.h"
#include "Elements/ActionElement.h"
#include "ActionSelector.h"
#include <Wheelson.h>
#include <Input/Input.h>

Simple::Edit* Simple::Edit::instance = nullptr;

Simple::Edit::Edit(Display& display) : Context(display), scrollLayout(new ScrollLayout(&screen)), list(new GridLayout(scrollLayout, 5)){
	instance = this;
	actions.push_back(new ActionElement(list, static_cast<Action::Type>(Action::Type::COUNT)));
	actions[0]->setIsSelected(true);

	buildUI();
	Edit::pack();

}

Simple::Edit::~Edit(){
	instance = nullptr;
}

void Simple::Edit::start(){
	Input::getInstance()->addListener(this);
	draw();
	screen.commit();

}

void Simple::Edit::stop(){
	Input::getInstance()->removeListener(this);
}

void Simple::Edit::draw(){
	screen.getSprite()->drawIcon(backgroundBuffer, 0, 0, 160, 128, 1);
	screen.draw();
}

void Simple::Edit::init(){
	free(backgroundBuffer);
}

void Simple::Edit::deinit(){
	backgroundBuffer = static_cast<Color*>(ps_malloc(160 * 128 * 2));
	if(backgroundBuffer == nullptr){
		Serial.println("Edit background picture unpack error");
		return;
	}
	fs::File backgroundFile = CompressedFile::open(SPIFFS.open("/Simple/simple_edit_bg.raw.hs"), 12, 8);
	backgroundFile.read(reinterpret_cast<uint8_t*>(backgroundBuffer), 160 * 128 * 2);
	backgroundFile.close();

}

void Simple::Edit::buildUI(){
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

void Simple::Edit::loop(uint micros){

}

void Simple::Edit::selectAction(uint8_t num){
	actions[actionNum]->setIsSelected(false);
	actionNum = num;
	actions[actionNum]->setIsSelected(true);

}

void Simple::Edit::buttonPressed(uint id){
	uint8_t totalNumActions = actions.size();
	int16_t num;
	switch(id){
		case BTN_LEFT:
			if(actionNum == 0){
				selectAction(totalNumActions - 1);
			}else{
				selectAction(actionNum - 1);
			}

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

			scrollLayout->scrollIntoView(actionNum, 5);
			draw();
			screen.commit();
			break;

		case BTN_UP:
			Serial.printf("Pocetak:%d\n", totalNumActions);
			Serial.printf("Index prije: %d\n", actionNum);

			if(actionNum < 5){
				num = totalNumActions - (totalNumActions % 5) + actionNum;
				if(actionNum >= totalNumActions % 5){
					num -= 5;
				}
			}else{
				num = actionNum - 5;
			}
			Serial.printf("Index poslije: %d\n", num);
			selectAction(num);
			scrollLayout->scrollIntoView(actionNum, 5);
			draw();
			screen.commit();
			break;

		case BTN_DOWN:

			if(actionNum >= totalNumActions - 5){
				selectAction(actionNum % 5);
			}else{
				selectAction(actionNum + 5);
			}

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

void Simple::Edit::returned(void* data){
	Context::returned(data);
	uint8_t* podatakPtr = static_cast<uint8_t*>(data);

	uint8_t podatak = *podatakPtr;
	delete podatakPtr;

	actions.push_back(new ActionElement(list, static_cast<Action::Type>(podatak)));
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

