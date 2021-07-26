#ifndef WHEELSON_FIRMWARE_BATTERYPOPUPSERVICE_H
#define WHEELSON_FIRMWARE_BATTERYPOPUPSERVICE_H

#include <Loop/LoopListener.h>
#include <Input/InputListener.h>
class WarningPopup;
class ShutdownPopup;
class TFT_eSPI;

class BatteryPopupService : public LoopListener, public InputListener{
public:
	void loop(uint time) override;
	void setTFT(TFT_eSPI* _tft);
private:
	static const uint16_t checkInterval;
	uint checkMicros = 0;
	uint blinkMicros = 0;
	uint autoShutdownMicros = 0;
	uint8_t lastShutdownTime = 0xFF;
	static const uint8_t longBlinkInterval;
	static const uint8_t shortBlinkInterval;

	static ShutdownPopup *shutdownPopup;
	static WarningPopup *warningPopup;
	bool warningShown = false;

	bool blinkActive = false;

	TFT_eSPI* tft = nullptr;

	void anyKeyPressed() override;

};

extern BatteryPopupService BatteryPopup;

#endif //WHEELSON_FIRMWARE_BATTERYPOPUPSERVICE_H
