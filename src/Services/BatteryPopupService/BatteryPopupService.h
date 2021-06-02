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

	ShutdownPopup *shutdownPopup = nullptr;
	WarningPopup *warningPopup = nullptr;
	bool warningShown = false;
};

extern BatteryPopupService BatteryPopup;

#endif //WHEELSON_FIRMWARE_BATTERYPOPUPSERVICE_H
