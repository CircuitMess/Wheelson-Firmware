#include "BatteryPopupService.h"
#include <Wheelson.h>
#include <Support/ContextTransition.h>
#include <Support/ModalTransition.h>
#include "ShutdownPopup.h"
#include "WarningPopup.h"

BatteryPopupService BatteryPopup;
const uint16_t BatteryPopupService::checkInterval = 20; //in seconds
ShutdownPopup *BatteryPopupService::shutdownPopup = nullptr;
WarningPopup *BatteryPopupService::warningPopup = nullptr;

void BatteryPopupService::loop(uint time){
	checkMicros += time;
	if(checkMicros >= checkInterval * 1000000){

		//voltage not yet read
		if(Battery.getVoltage() == 0){
			checkMicros = 0;
			return;
		}

		uint8_t percentage = Battery.getPercentage();
		if(percentage <= 1){
			if(ContextTransition::isRunning() || Context::getCurrentContext() == shutdownPopup ||
			   Context::getCurrentContext() == warningPopup || ModalTransition::isRunning()) return;
			shutdownPopup = new ShutdownPopup(*Context::getCurrentContext());
			shutdownPopup->push(Context::getCurrentContext());
		}else if((percentage <= 15 || true) && !warningShown){
			if(ContextTransition::isRunning() || Context::getCurrentContext() == shutdownPopup ||
			   ModalTransition::isRunning()) return;


			warningShown = true;
			warningPopup = new WarningPopup(*Context::getCurrentContext());
			ModalTransition* transition;
			if(Modal::getCurrentModal() != nullptr){
				transition = Modal::getCurrentModal()->pop();
				transition->setDoneCallback([](Context*, Modal* prevModal){
					warningPopup->push(Context::getCurrentContext());
					warningPopup->returned(prevModal);
				});
			}
		}
		checkMicros = 0;
	}
}
