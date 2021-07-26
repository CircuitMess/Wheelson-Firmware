#include <FS/CompressedFile.h>
#include "Edit.h"
#include "Elements/ActionElement.h"
#include "ActionSelector.h"
#include "EditModal.h"
#include <Wheelson.h>
#include <Input/Input.h>
#include <Loop/LoopManager.h>
#include <SPIFFS.h>

Simple::Edit* Simple::Edit::instance = nullptr;

Simple::Edit::Edit(Display& display, Storage* storage, int16_t programIndex) : Context(display),
																			   scrollLayout(new ScrollLayout(&screen)),
																			   list(new GridLayout(scrollLayout, 5)), storage(storage), programIndex(programIndex){

	const Program* program = storage->getProg(programIndex);

	actions = std::vector<Action>(program->actions, program->actions + program->numActions);
	for(uint8_t i = 0; i < program->numActions; i++){
		list->addChild(new ActionElement(list, actions[i].type));
	}
	ActionElement* add = new ActionElement(list, static_cast<Action::Type>(Action::Type::COUNT));
	list->addChild(add);
	add->setIsSelected(true);
	actionNum = program->numActions;

	buildUI();
	scrollLayout->scrollIntoView(actionNum,10);
	Edit::pack();

}

Simple::Edit::~Edit(){

}

void Simple::Edit::start(){
	Input::getInstance()->addListener(this);
	draw();
	screen.commit();

}

void Simple::Edit::stop(){
	Input::getInstance()->removeListener(this);
	storage->updateProg(programIndex, actions.data(), actions.size());
}

void Simple::Edit::draw(){
	screen.getSprite()->drawIcon(backgroundBuffer, 0, 0, 160, 128, 1);
	screen.draw();
	screen.getSprite()->drawIcon(backgroundBuffer, 0, 113, 160, 113, 1);
	screen.getSprite()->drawLine(0, 113, screen.getWidth(), 113, TFT_WHITE);
	Battery.drawIcon(screen.getSprite());

	FontWriter u8f = screen.getSprite()->startU8g2Fonts();
	u8f.setFont(u8g2_font_profont12_tf);
	u8f.setForegroundColor(TFT_WHITE);
	u8f.setFontMode(1);
	u8f.setCursor((160 - u8f.getUTF8Width("Hold BACK to delete action")) / 2, screen.getTotalY() + 124);
	u8f.println("Hold BACK to delete action");

}

void Simple::Edit::init(){
	backgroundBuffer = static_cast<Color*>(ps_malloc(160 * 128 * 2));
	if(backgroundBuffer == nullptr){
		Serial.println("Edit background picture unpack error");
		return;
	}
	fs::File backgroundFile = CompressedFile::open(SPIFFS.open("/Simple/simple_edit_bg.raw.hs"), 12, 8);
	backgroundFile.read(reinterpret_cast<uint8_t*>(backgroundBuffer), 160 * 128 * 2);
	backgroundFile.close();

}

void Simple::Edit::deinit(){
	free(backgroundBuffer);

}

void Simple::Edit::buildUI(){
	scrollLayout->setWHType(FIXED, FIXED);
	scrollLayout->setWidth(130);
	scrollLayout->setHeight(115);
	scrollLayout->addChild(list);
	//scrollLayout->setBorder(2, TFT_RED);

	list->setWHType(CHILDREN, CHILDREN);
	list->setY(100);
	list->setPadding(5);
	list->setGutter(8);

	scrollLayout->reflow();
	list->reflow();

	screen.addChild(scrollLayout);
	screen.repos();
	scrollLayout->setX(screen.getTotalX() + 15);

}

void Simple::Edit::loop(uint micros){
	if(delPressStart == 0){
		LoopManager::removeListener(this);
		return;
	}

	if(millis() - delPressStart >= 1000){
		delPressStart = 0;
		LoopManager::removeListener(this);

		if(actionNum >= actions.size()) return;

		actions.erase(actions.begin() + actionNum);
		list->getChildren().erase(list->getChildren().begin() + actionNum);
		list->reflow();
		list->repos();
		if(actionNum>1){
			selectAction(actionNum - 1);
		}else{
			selectAction(0);
		}
		draw();
		screen.commit();
	}
}

void Simple::Edit::selectAction(uint8_t num){
	reinterpret_cast<ActionElement*>(list->getChildren()[actionNum])->setIsSelected(false);
	actionNum = num;
	reinterpret_cast<ActionElement*>(list->getChildren()[actionNum])->setIsSelected(true);

}

void Simple::Edit::buttonPressed(uint id){
	uint8_t totalNumActions = list->getChildren().size();
	int16_t num;
	switch(id){
		case BTN_LEFT:
			if(actionNum == 0){
				selectAction(totalNumActions - 1);
			}else{
				selectAction(actionNum - 1);
			}
			if(totalNumActions > 24){
				scrollLayout->scrollIntoView(actionNum, 2);
			}
			draw();
			screen.commit();
			break;

		case BTN_RIGHT:
			if(actionNum == totalNumActions - 1){
				selectAction(0);
			}else{
				selectAction(actionNum + 1);
			}
			if(totalNumActions > 24){
				scrollLayout->scrollIntoView(actionNum, 2);
			}
			draw();
			screen.commit();
			break;

		case BTN_UP:
			if(actionNum < 5){
				num = totalNumActions - (totalNumActions % 5) + actionNum;
				if(actionNum >= totalNumActions % 5){
					num -= 5;
				}
			}else{
				num = actionNum - 5;
			}

			selectAction(num);
			scrollLayout->scrollIntoView(actionNum, 2);
			draw();
			screen.commit();
			break;

		case BTN_DOWN:

			if(actionNum >= totalNumActions - 5){
				selectAction(actionNum % 5);
			}else{
				selectAction(actionNum + 5);
			}

			scrollLayout->scrollIntoView(actionNum, 2);
			draw();
			screen.commit();
			break;

		case BTN_MID:
			if(totalNumActions == 1 || actionNum == totalNumActions - 1){
				ActionSelector* popUpModul = new ActionSelector(*this);
				popUpModul->push(this);
			}else{
				if(actions[actionNum].type == Action::LED_OFF || actions[actionNum].type == Action::LED_ON) return;
				EditModal* editModal = new EditModal(*this, &actions[actionNum]);
				editModal->push(this);
			}
			break;

		case BTN_BACK:
			delPressStart = millis();
			LoopManager::addListener(this);
			break;
	}
}

void Simple::Edit::buttonReleased(uint id){
	if(id != BTN_BACK) return;

	uint32_t elapsed = millis() - delPressStart;

	delPressStart = 0;
	LoopManager::removeListener(this);

	if(elapsed < 1000){
		pop();
	}
}

void Simple::Edit::returned(void* data){
	Context::returned(data);
	Action::Type* podatakPtr = static_cast<Action::Type*>(data);

	Action::Type type = *podatakPtr;
	delete podatakPtr;

	actions.push_back({type});
	switch(type){
		case Action::LEFT:
		case Action::RIGHT:
			actions.back().time = 0.5;
			actions.back().speed = 100;
			break;
		case Action::FORWARD:
		case Action::BACKWARD:
		case Action::PAUSE:
			actions.back().time = 1;
			actions.back().speed = 100;
			break;
		case Action::LED_OFF:
		case Action::LED_ON:
			actions.back().time = 0;
			break;
	}

	ActionElement* newAction = new ActionElement(list, type);

	list->addChild(newAction);
	list->getChildren().swap(list->getChildren().size() - 1, list->getChildren().size() - 2);
	actionNum = list->getChildren().size() - 1;
	list->reflow();
	screen.repos();
	scrollLayout->scrollIntoView(list->getChildren().size() - 1, 5);
	scrollLayout->setX(screen.getTotalX() + 15);
}



