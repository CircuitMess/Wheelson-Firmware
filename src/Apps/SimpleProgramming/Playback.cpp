#include <FS/CompressedFile.h>
#include "Playback.h"
#include <Wheelson.h>

Simple::Playback* Simple::Playback::instance = nullptr;

Simple::Playback::Playback(Display& display, Action* action, uint8_t numActions) : Context(display), scrollLayout(new ScrollLayout(&screen)), layout(new LinearLayout(scrollLayout, VERTICAL)), action(action), numActions(numActions),
																				   newPlayer(action, numActions){
	instance = this;
	buildUI();

	item[itemNum]->setIsSelected(true);
	Playback::pack();
}

Simple::Playback::~Playback(){
	instance = nullptr;
}

void Simple::Playback::start(){
	Input::getInstance()->addListener(this);
	newPlayer.start();
	if(newPlayer.isDone()){
		newPlayer.stop();
	}
	draw();
	screen.commit();
}

void Simple::Playback::draw(){
	screen.getSprite()->drawIcon(backgroundBuffer, 0, 0, 160, 128, 1);
	screen.draw();

}

void Simple::Playback::stop(){
	Input::getInstance()->removeListener(this);
}

void Simple::Playback::init(){
	backgroundBuffer = static_cast<Color*>(ps_malloc(160 * 128 * 2));
	if(backgroundBuffer == nullptr){
		Serial.printf("Playback background picture unpack error\n");
		return;
	}
	fs::File backgroundFile = CompressedFile::open(SPIFFS.open("/Simple/playback_bg.raw.hs"), 12, 8);

	backgroundFile.read(reinterpret_cast<uint8_t*>(backgroundBuffer), 160 * 128 * 2);
	backgroundFile.close();

}

void Simple::Playback::deinit(){
	free(backgroundBuffer);
}

void Simple::Playback::buildUI(){
	scrollLayout->setWHType(FIXED, PARENT);
	scrollLayout->setWidth(120);
	//scrollLayout->setBorder(2,TFT_RED);
	layout->setWHType(PARENT, PARENT);
	layout->setGutter(5);

	for(int i = 0; i < 6; i++){
		ActionElement* items = new ActionElement(layout, static_cast<Action::Type>(i), "Textttttt");
		item.push_back(items);
		layout->addChild(items);
	}

	layout->reflow();
	screen.addChild(layout);
	screen.repos();
	layout->setX(screen.getTotalX() + 30);
}

void Simple::Playback::loop(uint micros){

}

void Simple::Playback::selectAction(uint8_t num){
	item[itemNum]->setIsSelected(false);
	itemNum = num;
	item[itemNum]->setIsSelected(true);
}

void Simple::Playback::buttonPressed(uint id){
	uint8_t numItems = item.size();
	switch(id){
		case BTN_UP:
			if(itemNum == 0){
				selectAction(numItems - 1);
			}else{
				selectAction(itemNum - 1);
			}

			scrollLayout->scrollIntoView(itemNum, 5);
			draw();
			screen.commit();
			break;

		case BTN_DOWN:
			if(itemNum == numItems - 1){
				selectAction(0);
			}else{
				selectAction(itemNum + 1);
			}

			scrollLayout->scrollIntoView(itemNum, 5);
			draw();
			screen.commit();
			break;
		case BTN_BACK:
			this->pop();
			break;
	}
}
