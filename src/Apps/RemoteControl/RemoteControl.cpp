#include <SPIFFS.h>
#include <FS/CompressedFile.h>
#include <Wheelson.h>
#include <WiFi.h>
#include "RemoteControl.h"

RemoteControl::RemoteControl(Display &display) : Context(display){
	con.setListener(this);
	RemoteControl::pack();
}

RemoteControl::~RemoteControl(){
	RemoteControl::deinit();

	delete driver;
	driver = nullptr;
	con.stop();
}

void RemoteControl::draw(){
	Sprite* canvas = screen.getSprite();
	canvas->drawIcon(background, 0, 0, 160, 128);
	canvas->drawIcon(driver ? icon : crossedIcon, (160 - 70) / 2, 19, 70, 70, 1, TFT_TRANSPARENT);

	canvas->setTextFont(0);
	canvas->setTextSize(1);
	canvas->setTextColor(TFT_WHITE);
	canvas->setTextDatum(textdatum_t::top_center);

	canvas->drawString(driver ? "Connected" : "Connecting...", canvas->width()/2, 105);
}

void RemoteControl::start(){
	con.start();
	Input::getInstance()->addListener(this);
	draw();
	screen.commit();
}

void RemoteControl::stop(){
	delete driver;
	driver = nullptr;
	con.stop();
	Input::getInstance()->removeListener(this);

	WiFi.disconnect(true);
	WiFi.mode(WIFI_OFF);
}

void RemoteControl::init(){
	auto read = [](const char* path, bool bg = false) -> Color* {
		fs::File file;
		if(bg){
			file = CompressedFile::open(SPIFFS.open(path, "r"), 12, 10);
		}else{
			file = SPIFFS.open(path, "r");
		}

		if(!file){
			printf("Error opening SPIFFS file: %s\n", path);
			return nullptr;
		}

		size_t size = bg ? 160 * 128 * 2 : file.size();

		Color* buffer = static_cast<Color *>(ps_malloc(size));
		file.read(reinterpret_cast<uint8_t *>(buffer), size);
		file.close();

		return buffer;
	};

	icon = read("/RemoteControl/wheel.raw");
	crossedIcon = read("/RemoteControl/wheel-cross.raw");
	background = read("/RemoteControl/bg.raw.hs", true);
}

void RemoteControl::deinit(){
	free(icon);
	free(background);
	free(crossedIcon);
	icon = nullptr;
	background = nullptr;
	crossedIcon = nullptr;
}

void RemoteControl::connected(){
	driver = new RCDriver(&con);

	draw();
	screen.commit();
}

void RemoteControl::disconnected(){
	delete driver;
	driver = nullptr;

	con.start();

	draw();
	screen.commit();
}

void RemoteControl::buttonPressed(uint i){
	if(i == BTN_BACK){
		pop();
		return;
	}
}
