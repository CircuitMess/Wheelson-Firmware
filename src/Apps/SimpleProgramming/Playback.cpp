#include <FS/CompressedFile.h>
#include "Playback.h"
#include <Wheelson.h>
#include <Loop/LoopManager.h>

Simple::Playback::Playback(Display& display, const Program* program) : Context(display), scrollLayout(new ScrollLayout(&screen)), layout(new LinearLayout(scrollLayout, VERTICAL)),
												program(program), player(program){
	buildUI();

	Playback::pack();
}

Simple::Playback::~Playback(){

}

void Simple::Playback::start(){


	if(program->numActions == 0){
		draw();

		FontWriter& writer = screen.getSprite()->startU8g2Fonts();

		writer.setForegroundColor(TFT_WHITE);
		writer.setFont(u8g2_font_profont12_tf);
		writer.setCursor((screen.getWidth() - writer.getUTF8Width("Program empty!")) / 2, 60);
		writer.print("Program empty!");
		screen.commit();

		delay(2000);
		pop();
		return;
	}
	Input::getInstance()->addListener(this);
	LoopManager::addListener(this);
	player.start();
	draw();
	screen.commit();
}

void Simple::Playback::draw(){
	screen.getSprite()->drawIcon(backgroundBuffer, 0, 0, 160, 128, 1);
	screen.draw();

}

void Simple::Playback::stop(){
	player.stop();
	LoopManager::removeListener(this);
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
	layout->setWHType(PARENT, PARENT);
	layout->setGutter(5);
	layout->setPadding(10);

	for(int i = 0; i < program->numActions; i++){
		ActionElement* item = new ActionElement(layout, program->actions[i].type, "Textttttt");
		items.push_back(item);
		layout->addChild(item);
	}

	if(!items.empty()){
		items.front()->setIsSelected(true);
	}

	layout->reflow();
	screen.addChild(layout);
	screen.repos();
}

void Simple::Playback::loop(uint micros){
	if(player.isDone()){
		pop();
		return;
	}

	if(player.getCurrent() != currentAction){
		selectAction(player.getCurrent());
		scrollLayout->scrollIntoView(currentAction);
		draw();
		screen.commit();
	}
}

void Simple::Playback::selectAction(uint8_t num){
	items[currentAction]->setIsSelected(false);
	currentAction = num;
	items[currentAction]->setIsSelected(true);
}

void Simple::Playback::buttonPressed(uint id){
	if(id == BTN_BACK){
		pop();
	}
}
