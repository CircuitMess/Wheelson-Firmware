#include <Arduino.h>
#include <CircuitOS.h>
#include <Support/Context.h>
#include <Update/UpdateManager.h>
#include <Input/InputGPIO.h>

#include "src/MainMenu.h"
#include "src/Components/ActionProcessor.h"

Display display(128, 160, 18, 2);
Input* input = nullptr;

Context* menu = nullptr;

void setup(){
	Serial.begin(115200);
	display.begin();

	input = new InputGPIO();
	UpdateManager::addListener(input);

	menu = new MainMenu(display);
	menu->unpack();
	menu->start();

	ActionProcessor* processor = new ActionProcessor();
}

void loop(){
	UpdateManager::update();
}