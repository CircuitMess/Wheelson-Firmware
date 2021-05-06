#include <FS/CompressedFile.h>
#include "SimpleEdit.h"
#include "SimpleAction.h"

SimpleEdit* SimpleEdit::instance = nullptr;

SimpleEdit::SimpleEdit(Display& display) : Context(display), scrollLayout(new ScrollLayout(&screen)), list(new GridLayout(scrollLayout, 5)){

	instance = this;
	actions.push_back(new SimpleAction(list, static_cast<Action>(0)));
	for(int i = 0; i < 2; i++){
		for(int j = 1; j < 7; j++){
			actions.push_back(new SimpleAction(list, static_cast<Action>(j)));
		}
	}
	buildUI();
    actions[1]->setIsSelected(true);

	SimpleEdit::pack();

}

SimpleEdit::~SimpleEdit(){
	instance = nullptr;
}

void SimpleEdit::start(){
	draw();
	screen.commit();

}

void SimpleEdit::stop(){

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
	for(int i = 1; i < actions.size(); i++){
		list->addChild(actions[i]);
		list->getChildren().swap(i,i-1);
	}

	scrollLayout->reflow();
	list->reflow();

	screen.addChild(scrollLayout);
	screen.repos();

	scrollLayout->setX(screen.getTotalX() + 15);
}

void SimpleEdit::loop(uint micros){

}







