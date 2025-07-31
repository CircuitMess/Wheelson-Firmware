#include "ActionSelector.h"
#include <Input/Input.h>
#include <Wheelson.h>
#include "../../Fonts.h"
#include <Util/HWRevision.h>

const char* const Simple::ActionSelector::ActionTitles[] = { "Forward", "Backward", "Left", "Right", "LED ON", "LED OFF", "Pause" };

Simple::ActionSelector* Simple::ActionSelector::instance = nullptr;

Simple::ActionSelector::ActionSelector(Context& context) :
		Modal(context, 100, 100), gridLayout(new GridLayout(&screen, Cols)),
		modalBg(&screen, 100, 100, C_HEX(0x00beff), C_HEX(0x00ffff), C_HEX(0x0082ff)){

	uint8_t rev = HWRevision::get();

	/**
	 * Revision 2 doesn't have headlights, so hide the LED ON and LED OFF elements
	 */
	if(rev == 2){
		actionsNum = 5;

		for(int i : { 0, 1, 2, 3, 6 }){
			actionElements.push_back(new ActionElement(gridLayout, static_cast<Action::Type>(i)));
		}
	}else{
		actionsNum = 7;

		for(int i = 0; i < 7; i++){
			actionElements.push_back(new ActionElement(gridLayout, static_cast<Action::Type>(i)));
		}
	}

	lastRowElements = actionsNum % Cols;

	actionElements[0]->setIsSelected(true);
	buildUI();
}

void Simple::ActionSelector::draw(){
	screen.getSprite()->clear(TFT_TRANSPARENT);
	modalBg.draw();

	auto canvas = screen.getSprite();
	canvas->setFont(&u8g2_font_HelvetiPixel_tr);
	canvas->setTextColor(TFT_WHITE);
	canvas->setTextDatum(textdatum_t::top_center);
	canvas->drawString(ActionTitles[actionElements[selection]->getAction()], canvas->width() / 2, 85);
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
	for(auto& action : actionElements){
		gridLayout->addChild(action);
	}
	gridLayout->reflow();
	screen.addChild(gridLayout);
	screen.repos();

	if(HWRevision::get() == 2){
		gridLayout->setX(screen.getTotalX() + 15);
		gridLayout->setY(screen.getTotalY() + 15);
	}else{
		actionElements[6]->setX(screen.getTotalX() + 26);
		gridLayout->setX(screen.getTotalX() + 15);
		gridLayout->setY(screen.getTotalY() + 15);
	}


}

void Simple::ActionSelector::selectApp(int8_t num){
	actionElements[selection]->setIsSelected(false);
	selection = num;
	actionElements[selection]->setIsSelected(true);
}

void Simple::ActionSelector::buttonPressed(uint id){
	/**
	 * This navigation logic needs a proper refactor.
	 */
	switch(id){
		case BTN_LEFT:
			if(selection == 0){
				selectApp(actionsNum - 1);
			}else{
				selectApp(selection - 1);
			}

			draw();
			screen.commit();
			break;

		case BTN_RIGHT:
			if(selection == actionsNum - 1){
				selectApp(0);
			}else{
				selectApp(selection + 1);
			}

			draw();
			screen.commit();
			break;

		case BTN_UP:
			if(selection < Cols){
				selectApp(constrain(actionsNum - lastRowElements + selection, actionsNum - lastRowElements, actionsNum - 1));
			}else if(selection >= actionsNum - lastRowElements){
				if(lastRowElements % 2 == 0){
					selectApp(selection - Cols);
				}else{
					selectApp(selection - Cols + lastRowElements);
				}
			}else{
				selectApp(selection - Cols);
			}
			draw();
			screen.commit();
			break;

		case BTN_DOWN:
			if(selection >= actionsNum - lastRowElements){
				if(lastRowElements % 2 == 0){
					selectApp(selection - (actionsNum - lastRowElements));
				}else{
					selectApp(selection - (actionsNum - lastRowElements) + lastRowElements);
				}
			}else if(selection > actionsNum - lastRowElements - Cols){
				selectApp(constrain(selection + Cols, actionsNum - lastRowElements, actionsNum - 1));
			}else{
				selectApp(selection + Cols);
			}
			draw();
			screen.commit();
			break;

		case BTN_MID:
			this->pop(new Action::Type((Action::Type) actionElements[selection]->getAction()));
			break;
		case BTN_BACK:
			this->pop();
			break;

	}
}
