#include "BatteryPopupService.h"
#include <Wheelson.h>
#include <Support/ContextTransition.h>
#include <Support/ModalTransition.h>
#include "ShutdownPopup.h"
#include "WarningPopup.h"

BatteryPopupService BatteryPopup;
const uint16_t BatteryPopupService::checkInterval = 20; //in seconds
const uint8_t BatteryPopupService::shortBlinkInterval = 1; //in seconds
const uint8_t BatteryPopupService::longBlinkInterval = 5; //in seconds
ShutdownPopup *BatteryPopupService::shutdownPopup = nullptr;
WarningPopup *BatteryPopupService::warningPopup = nullptr;

void BatteryPopupService::loop(uint time){
	checkMicros += time;
	blinkMicros += time;
	uint8_t percentage = Battery.getPercentage();

	if(blinkActive){
		if(blinkMicros >= 100000){
			LED.setRGB(OFF);
			blinkActive = false;
			blinkMicros = 0;
		}
	}else{
		if(blinkMicros >= shortBlinkInterval*1000000 && percentage <= 5){
			LED.setRGB(RED);
			blinkActive = true;
			blinkMicros = 0;
		}else if(blinkMicros >= longBlinkInterval*1000000 && percentage <= 15){
			LED.setRGB(RED);
			blinkActive = true;
			blinkMicros = 0;
		}
	}

	if(checkMicros >= checkInterval * 1000000){

		//voltage not yet read
		if(Battery.getVoltage() == 0){
			checkMicros = 0;
			return;
		}

		if(percentage <= 1){
			if(ContextTransition::isRunning() ||
			   (Modal::getCurrentModal() != nullptr && (Modal::getCurrentModal() == shutdownPopup || Modal::getCurrentModal() == warningPopup))
			   || ModalTransition::isRunning()) return;

			shutdownPopup = new ShutdownPopup(*Context::getCurrentContext());
			shutdownPopup->push(Context::getCurrentContext());
		}else if(percentage <= 15 && !warningShown){
			if(ContextTransition::isRunning() ||
			   (Modal::getCurrentModal() != nullptr && Modal::getCurrentModal() == shutdownPopup) ||
			   ModalTransition::isRunning()) return;

			warningShown = true;
			ModalTransition *transition;

			if(Modal::getCurrentModal() != nullptr){
				ModalTransition::setDeleteOnPop(false);
				transition = static_cast<ModalTransition *>((void *)Modal::getCurrentModal()->pop());
				transition->setDoneCallback([](Context *currentContext, Modal *prevModal){
					warningPopup = new WarningPopup(*currentContext);
					warningPopup->push(currentContext);
					warningPopup->returned(prevModal);
					ModalTransition::setDeleteOnPop(true);
				});
			}else{
				warningPopup = new WarningPopup(*Context::getCurrentContext());
				warningPopup->push(Context::getCurrentContext());
			}
		}
		checkMicros = 0;
	}
}
