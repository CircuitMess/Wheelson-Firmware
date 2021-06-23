#ifndef WHEELSON_FIRMWARE_BATTERYPOPUPSERVICE_H
#define WHEELSON_FIRMWARE_BATTERYPOPUPSERVICE_H

#include <Loop/LoopListener.h>

class WarningPopup;
class ShutdownPopup;

class BatteryPopupService : public LoopListener {
public:
	void loop(uint time) override;

private:
	static const uint16_t checkInterval;
	uint checkMicros = 0;
	uint blinkMicros = 0;
	static const uint8_t longBlinkInterval;
	static const uint8_t shortBlinkInterval;

	static ShutdownPopup *shutdownPopup;
	static WarningPopup *warningPopup;
	bool warningShown = false;

	bool blinkActive = false;

};

extern BatteryPopupService BatteryPopup;

#endif //WHEELSON_FIRMWARE_BATTERYPOPUPSERVICE_H
