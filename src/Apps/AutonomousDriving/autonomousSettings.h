#ifndef AUTOCAR_AUTOSETTINGS_H
#define AUTOCAR_AUTOSETTINGS_H

#include <Arduino.h>
#include <Util/Settings.h>

struct SettingsStruct {
	uint8_t contrastSetting;
	bool motorsStop;
};

SettingsStruct* settings();
#endif