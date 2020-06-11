#ifndef AUTOCAR_AUTOSETTINGS_H
#define AUTOCAR_AUTOSETTINGS_H

#include <Arduino.h>
#include <Util/Settings.h>

struct SettingsStruct {
	double contrastSetting = 122;
};

SettingsStruct* settings();

#endif