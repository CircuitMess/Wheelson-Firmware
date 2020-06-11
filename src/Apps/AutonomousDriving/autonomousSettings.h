#ifndef AUTOCAR_AUTOSETTINGS_H
#define AUTOCAR_AUTOSETTINGS_H

#include <Arduino.h>
#include <Util/Settings.h>

struct SettingsStruct {
	double contrastSetting = 122;
	bool motorsStop = true;
};

SettingsStruct* settings();

#endif