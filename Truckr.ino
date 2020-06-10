#include <Arduino.h>
#include <CircuitOS.h>
#include <Support/Context.h>
#include <Update/UpdateManager.h>
#include <Input/InputGPIO.h>
#include <Input/InputI2c.h>
#include "src/MainMenu.h"
#include "src/defs.hpp"
#include <Sync/Mutex.h>
#include "src/Components/Motors.h"
#include "src/Apps/AutonomousDriving/autonomousSettings.h"
#include <esp32-hal-psram.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include <NeoPixelBrightnessBus.h>
#define CAMERA_MODEL_AI_THINKER

Display display(160, 128, -1, 3);
InputI2C* input = nullptr;
I2cExpander i2c;
Mutex i2cMutex;
Context* menu = nullptr;
Motors motors(&i2c, &i2cMutex);
const uint16_t PixelCount = 4; // the sample images are meant for 144 pixels
const uint16_t PixelPin = 32;
const uint16_t AnimCount = 200;
NeoPixelBrightnessBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

void setup(){
	Serial.begin(115200);
	strip.Begin();
	strip.ClearTo(RgbColor(0,0,0));
    strip.Show();
	strip.SetBrightness(0);
	
	psramInit();
	Serial.print("PSRAM found: ");
	Serial.println(psramFound());

	display.begin();

	Settings::init(new SettingsStruct, sizeof(SettingsStruct));

	i2c.begin(I2C_EXPANDER_ADDRESS, 14, 15);
	i2c.pinMode(14, OUTPUT);
	i2c.pinWrite(14, 0);
	motors.begin(11, 10, 12, 13);
	input = new InputI2C(&i2c, &i2cMutex);

	menu = new MainMenu(display);
	menu->unpack();
	menu->start();
	
	UpdateManager::addListener(input);
	UpdateManager::startTask();
	vTaskDelete(NULL);
}

void loop(){
	// UpdateManager::update();
}