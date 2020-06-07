#include <UI/BitmapElement.h>
#include "ActionEditor.h"
#include "../../defs.hpp"
#include "PatternEditor.h"
#include "Bitmaps/actions.hpp"

ActionEditor* ActionEditor::instance = nullptr;

ActionEditor::ActionEditor(PatternEditor* editor) : Modal(*editor, 50, 50), editor(editor), fleha(&getScreen(), 50, 50){

	instance = this;
	buildUI();
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
	});

	Input::getInstance()->setBtnPressCallback(BTN_C, [](){
		if(instance == nullptr) return;
	});

	Input::getInstance()->setBtnPressCallback(BTN_D, [](){
		if(instance == nullptr) return;
	});

	draw();
}

void ActionEditor::stop(){
	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnPressCallback(BTN_B);
	Input::getInstance()->removeBtnPressCallback(BTN_C);
	Input::getInstance()->removeBtnPressCallback(BTN_D);
}


void ActionEditor::fillMenu(){

}

void ActionEditor::buildUI(){
	fillMenu();

	screen.addChild(&fleha);
}
