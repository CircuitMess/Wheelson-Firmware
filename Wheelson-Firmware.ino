#include <Arduino.h>
#include <CircuitOS.h>
#include <Support/Context.h>
#include <Loop/LoopManager.h>
#include <Input/InputGPIO.h>
#include <Input/InputI2C.h>
#include "src/Apps/MainMenu/MainMenu.h"
#include <Sync/Mutex.h>
#include "src/Components/Motors.h"
#include "src/Apps/AutonomousDriving/autonomousSettings.h"
#include <esp32-hal-psram.h>
//#include <NeoPixelBus.h>
//#include <NeoPixelAnimator.h>
//#include <NeoPixelBrightnessBus.h>
#define CAMERA_MODEL_AI_THINKER
#include "src/Components/ActionProcessor.h"

//#include <NeoPixelBus.h>
//#include <NeoPixelAnimator.h>
//#include <NeoPixelBrightnessBus.h>

#define blPin 25

Display display(160, 128, -1, -1);
InputI2C* input = nullptr;
I2cExpander i2c;
Context* menu = nullptr;
Motors motors(&i2c);
const uint16_t PixelCount = 4; // the sample images are meant for 144 pixels
const uint16_t PixelPin = 32;
const uint16_t AnimCount = 200;
//NeoPixelBrightnessBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

void setup(){
	Serial.begin(115200);
	//strip.Begin();
//	strip.ClearTo(RgbColor(255, 255, 255));
	//strip.SetBrightness(255);
	//strip.Show();
	if(!SPIFFS.begin()){
		Serial.println("SPIFFS error");
	}
	psramInit();
	Serial.print("PSRAM found: ");
	Serial.println(psramFound());

	pinMode(blPin, OUTPUT);
	digitalWrite(blPin, LOW);
	display.begin();

	Settings::init(new SettingsStruct, sizeof(SettingsStruct));
	//JayD.begin();
	ActionProcessor* processor = new ActionProcessor();
//	i2c.begin(I2C_EXPANDER_ADDRESS, 14, 15);
	i2c.pinMode(14, OUTPUT);
	i2c.pinWrite(14, 0);
	motors.begin(11, 10, 12, 13);
	input = new InputI2C(&i2c);

	menu = new MainMenu(display);
	menu->unpack();
	menu->start();

	Task::setPinned(true);
	LoopManager::addListener(input);
	LoopManager::startTask();

	vTaskDelete(NULL);
}

void loop(){
	//LoopManager::loop();
}