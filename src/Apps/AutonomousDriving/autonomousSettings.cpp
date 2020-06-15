#include "autonomousSettings.h"


SettingsStruct* settings(){
	return static_cast<SettingsStruct*>(Settings::data());
}