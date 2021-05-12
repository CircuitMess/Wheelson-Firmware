#include <FS/CompressedFile.h>
#include "Playback.h"

Simple::Playback* Simple::Playback::instance = nullptr;

Simple::Playback::Playback(Display& display) : Context(display), scrollLayout(new ScrollLayout(&screen)), layout(new LinearLayout(scrollLayout, VERTICAL)){
	instance = this;
	buildUI();

	item[itemNum]->setIsSelected(true);

}

Simple::Playback::~Playback(){
	instance = nullptr;
}

void Simple::Playback::start(){
	draw();
	screen.commit();
}

void Simple::Playback::draw(){
	screen.getSprite()->drawIcon(backgroundBuffer, 0, 0, 160, 128, 1);
	screen.draw();

}

void Simple::Playback::stop(){

}

void Simple::Playback::deinit(){
	free(backgroundBuffer);
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

