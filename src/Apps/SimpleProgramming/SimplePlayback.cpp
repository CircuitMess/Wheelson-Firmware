#include <FS/CompressedFile.h>
#include "SimplePlayback.hpp"

SimplePlayback* SimplePlayback::instance = nullptr;

SimplePlayback::SimplePlayback(Display& display) : Context(display), scrollLayout(new ScrollLayout(&screen)), layout(new LinearLayout(scrollLayout, VERTICAL)){
	instance = this;
	buildUI();

	item[itemNum]->setIsSelected(true);

}

SimplePlayback::~SimplePlayback(){
	instance = nullptr;
}

void SimplePlayback::start(){
	draw();
	screen.commit();
}

void SimplePlayback::draw(){
	screen.getSprite()->drawIcon(backgroundBuffer, 0, 0, 160, 128, 1);
	screen.draw();

}

void SimplePlayback::stop(){

}

void SimplePlayback::pack(){
	Context::pack();
	free(backgroundBuffer);
}

void SimplePlayback::unpack(){
	Context::unpack();
	backgroundBuffer = static_cast<Color*>(ps_malloc(160 * 128 * 2));
	if(backgroundBuffer == nullptr){
		Serial.printf("Playback background picture unpack error\n");
		return;
	}
	fs::File backgroundFile = CompressedFile::open(SPIFFS.open("/Simple/playback_bg.raw.hs"), 12, 8);

	backgroundFile.read(reinterpret_cast<uint8_t*>(backgroundBuffer), 160 * 128 * 2);
	backgroundFile.close();

}

void SimplePlayback::buildUI(){
	scrollLayout->setWHType(FIXED, PARENT);
	scrollLayout->setWidth(120);
	//scrollLayout->setBorder(2,TFT_RED);
	layout->setWHType(PARENT, PARENT);
	layout->setGutter(5);

	for(int i = 0; i < 6; i++){
		PlaybackItem* items = new PlaybackItem(layout, static_cast<PlaybackItems>(i), "Textttttt");
		item.push_back(items);
		layout->addChild(items);
	}

	layout->reflow();
	screen.addChild(layout);
	screen.repos();
	layout->setX(screen.getTotalX() + 30);
}

void SimplePlayback::loop(uint micros){

}

