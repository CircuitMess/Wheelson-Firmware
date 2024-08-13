#include "BatteryPopupService.h"
#include <Wheelson.h>
#include <Support/ContextTransition.h>
#include <Support/ModalTransition.h>
#include "ShutdownPopup.h"
#include "WarningPopup.h"
#include <Settings.h>
#include <Display/Display.h>
#include <WiFi.h>

BatteryPopupService BatteryPopup;
const uint16_t BatteryPopupService::checkInterval = 20; //in seconds
const uint8_t BatteryPopupService::shortBlinkInterval = 1; //in seconds
const uint8_t BatteryPopupService::longBlinkInterval = 5; //in seconds
ShutdownPopup *BatteryPopupService::shutdownPopup = nullptr;
WarningPopup *BatteryPopupService::warningPopup = nullptr;
const uint8_t shutdownTimes[5] = {0, 1, 5, 15, 30};

void BatteryPopupService::loop(uint time){
	checkMicros += time;
	blinkMicros += time;

	if(Settings.get().shutdownTime != 0){
		autoShutdownMicros += time;
		if(autoShutdownMicros >= shutdownTimes[Settings.get().shutdownTime]*60000000){
			if(tft != nullptr){
				tft->writecommand(16);
			}
			Nuvo.shutdown();
			WiFi.mode(WIFI_OFF);
			btStop();
			esp_deep_sleep_start();
			return;
		}
	}
	if(lastShutdownTime == 0xFF){
		autoShutdownMicros = 0;
		lastShutdownTime = Settings.get().shutdownTime;
	}
	if(lastShutdownTime != Settings.get().shutdownTime){
		autoShutdownMicros = 0;
	}


	//voltage not yet read
	if(Battery.getVoltage() == 0){
		return;
	}
	uint8_t percentage = Battery.getPercentage();

	if(blinkActive){
		if(blinkMicros >= 100000){
			LED.setRGB(OFF);
			blinkActive = false;
			blinkMicros = 0;
		}
	}else{
		if(blinkMicros >= shortBlinkInterval*1000000 && percentage <= 5){
			LED.setRGB(WLEDColor::RED);
			blinkActive = true;
			blinkMicros = 0;
		}else if(blinkMicros >= longBlinkInterval*1000000 && percentage <= 15){
			LED.setRGB(WLEDColor::RED);
			blinkActive = true;
			blinkMicros = 0;
		}
	}

	if(checkMicros >= checkInterval * 1000000){

		if(percentage <= 1){
			if(ContextTransition::isRunning() ||
			   (Modal::getCurrentModal() != nullptr && (Modal::getCurrentModal() == shutdownPopup || Modal::getCurrentModal() == warningPopup))
			   || ModalTransition::isRunning()) return;

			Motors.stopAll();

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

void BatteryPopupService::setTFT(TFT_eSPI* _tft){
	tft=_tft;
}

void BatteryPopupService::anyKeyPressed(){
	autoShutdownMicros = 0;
}
