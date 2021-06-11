#include "WarningPopup.h"
#include <Loop/LoopManager.h>
#include "../../Wheelson.h"
#include <WiFi.h>
#include <Support/ModalTransition.h>
const uint8_t WarningPopup::warningTime = 5;
WarningPopup* WarningPopup::instance = nullptr;

WarningPopup::WarningPopup(Context &context) : Modal(context, 70, 30){
	instance = this;
}

void WarningPopup::draw(){
	screen.getSprite()->setTextColor(TFT_WHITE);
	screen.getSprite()->setTextSize(1);
	screen.getSprite()->setTextFont(1);
	screen.getSprite()->setCursor(0, screen.getTotalY() + 2);
	screen.getSprite()->printCenter("Warning!");
	screen.getSprite()->setCursor(0, screen.getTotalY() + 20);
	screen.getSprite()->printCenter("Battery low");
}

void WarningPopup::start(){
	LoopManager::addListener(this);
	draw();
	screen.commit();
}

void WarningPopup::stop(){
	LoopManager::removeListener(this);
}

void WarningPopup::loop(uint micros){
	warningTimer += micros;
	if(warningTimer >= warningTime * 1000000){
		warningTimer = 0;
		ModalTransition* transition = static_cast<ModalTransition*>((void*)pop());
		if(prevModal == nullptr) return;
		transition->setDoneCallback([](Context* currentContext, Modal*){
			instance->prevModal->push(currentContext);
		});
	}
}

void WarningPopup::returned(void *data){
	prevModal = (Modal*)data;
}
