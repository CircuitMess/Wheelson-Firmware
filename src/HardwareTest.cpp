#include "HardwareTest.h"
#include <SPI.h>
#include <SD.h>
#include "Wire.h"
#include "HWTestSPIFFS.hpp"
#include <Wheelson.h>
#include <SPIFFS.h>
#include <Camera.h>
#include <Util/HWRevision.h>

HardwareTest *HardwareTest::test = nullptr;

HardwareTest::HardwareTest(Display &_display) : canvas(_display.getBaseSprite()), display(&_display){

	test = this;

	tests.push_back({HardwareTest::nuvotonTest, "Nuvoton"});
	tests.push_back({HardwareTest::psram, "PSRAM"});
	tests.push_back({HardwareTest::SPIFFSTest, "SPIFFS"});
	tests.push_back({HardwareTest::camera, "Camera"});
	tests.push_back({HardwareTest::hwRevision, "HW rev"});

	Wire.begin(I2C_SDA, I2C_SCL);
}

void HardwareTest::start(){
	Serial.println();
	uint64_t mac = ESP.getEfuseMac();
	uint32_t upper = mac >> 32;
	uint32_t lower = mac & 0xffffffff;
	Serial.printf("TEST:begin:%x%x\n", upper, lower);

	canvas->clear(TFT_BLACK);
	canvas->setTextColor(TFT_GOLD);
	canvas->setTextFont(2);
	canvas->setTextSize(1);
	canvas->setCursor(display->getWidth()/2, 0);
	canvas->printCenter("Wheelson Hardware Test");
	canvas->setCursor(display->getWidth()/2, 8);
	canvas->println();
	canvas->setTextFont(1);
	display->commit();
	canvas->setTextDatum(textdatum_t::top_center);

	bool pass = true;
	for(const Test &test : tests){

		currentTest = test.name;

		canvas->setTextColor(TFT_WHITE);
		canvas->printf("%s: ", test.name);
		display->commit();

		bool result = test.test();

		canvas->setTextColor(result ? TFT_SILVER : TFT_ORANGE);
		canvas->printf("%s\n", result ? "PASS" : "FAIL");
		display->commit();

		if(!(pass &= result)) break;
	}

	canvas->print("\n\n");
	canvas->setTextColor(pass ? TFT_BLUE : TFT_ORANGE);
	canvas->printCenter(pass ? "All OK!" : "FAIL!");
	display->commit();

	if(pass){
		Serial.println("TEST:pass");
		postTestPass();
	}else{
		Serial.printf("TEST:fail:%s\n", currentTest);
		postTestFail();
	}
}

void HardwareTest::postTestPass(){
	vTaskDelay(1000);

	Camera cam;

	canvas->printf("\n");

	if(!cam.isInited()){
		canvas->setTextColor(TFT_ORANGE);
		canvas->setTextDatum(textdatum_t::top_center);
		canvas->fillRect(0, 75, 128, 20, TFT_BLACK);
		canvas->drawString("Camera error!", canvas->width()/2, 80);
		display->commit();

		postTestFail();
	}else{
		canvas->clear(TFT_BLACK);

		for(;;){
			cam.loadFrame();
			canvas->drawIcon(cam.getRGB565(), 0, 4, 160, 120);
			cam.releaseFrame();

			canvas->setTextDatum(textdatum_t::top_center);
			canvas->setTextColor(TFT_GREEN);
			canvas->drawString("Test successful!", canvas->width()/2, 40);

			canvas->setTextDatum(textdatum_t::top_left);
			canvas->setTextColor(TFT_BLACK);
			canvas->drawString("HW revision:", 35, 60);
			canvas->setTextColor(TFT_PURPLE);
			canvas->drawString(String(HWRevision::get()), 120, 60);

			display->commit();
		}
	}
}

void HardwareTest::postTestFail(){
	bool painted = false;
	const auto color = TFT_RED;
	auto flashTime = 0;

	for(;;){
		if(millis() - flashTime < 500) continue;

		for(int x = 0; x < canvas->width(); x++){
			for(int y = 0; y <  canvas->height(); y++){
				if(!painted && canvas->readPixel(x, y) == TFT_BLACK){
					canvas->drawPixel(x, y, color);
				}else if(painted && canvas->readPixel(x, y) == color){
					canvas->drawPixel(x, y, TFT_BLACK);
				}
			}
		}

		flashTime = millis();
		painted = !painted;
		display->commit();
	}
}

bool HardwareTest::psram(){
	if(!(psramFound() && psramInit())) return false;

	uint32_t free = ESP.getFreePsram();

	if(free != 4152352){
		test->log("free", free);
		return false;
	}

	return true;
}

bool HardwareTest::nuvotonTest(){

	pinMode(WSNV_PIN_RESET, OUTPUT);
	//digitalWrite(WSNV_PIN_RESET, HIGH);

	/* Nuvoton reset */
	digitalWrite(WSNV_PIN_RESET, LOW);
	delay(50);
	digitalWrite(WSNV_PIN_RESET, HIGH);
	delay(500);
	LED.setBacklight(true);

	/* Test i2c transmission */
	Wire.beginTransmission(WSNV_ADDR);
	if(Wire.endTransmission() != 0){
		test->log("Wire Transmission","Failed");
		return false;
	}

	/* Nuvoton Identification */
	Wire.beginTransmission(WSNV_ADDR);
	Wire.write(IDENTIFY_BYTE);
	Wire.endTransmission();
	Wire.requestFrom(WSNV_ADDR, 1);
	if(Wire.available()){
		if(Wire.read() == WSNV_ADDR){
			return true;
		}else{
			test->log("Identification", "Failed -> Wrong data acquired.");
			return false;
		}
	}else{
		test->log("Identification", "Failed -> Wire not available.");
	}
}
bool HardwareTest::SPIFFSTest(){

	File file;

	/* SPIFFS begin test */
	if(!SPIFFS.begin()){
		test->log("Begin","Failed");
		return false;
	}

	for(const auto & check : SPIFFSChecksums){

		file = SPIFFS.open(check.name);

		if(!file){
			test->log("File open error", check.name);
			return false;
		}

		char buff;
		uint32_t fileBytesSum = 0;

		while(file.readBytes(&buff,1)){

			fileBytesSum+=buff;
		}

		if(fileBytesSum != check.sum){
			char logBuffer[100];
			sprintf(logBuffer, "%s - expected %d, got %d", check.name, check.sum, fileBytesSum);
			test->log("Checksum mismatch", logBuffer);
			file.close();
			return false;
		}

		file.close();
	}

	return true;
}

bool HardwareTest::hwRevision(){
	const auto rev = HWRevision::get();
	if(rev != 0){
		test->canvas->printf("Fused: ");
		test->canvas->setTextColor(TFT_GOLD);
		test->canvas->printf("%d ", rev);
		test->canvas->setTextColor(TFT_WHITE);

		return rev == CurrentVersion;
	}

	HWRevision::write(CurrentVersion);
	HWRevision::commit();

	return true;
}

bool HardwareTest::camera(){
	Camera cam;
	return cam.isInited();
}

void HardwareTest::log(const char *property, char *value){
	Serial.printf("\n%s:%s:%s\n", currentTest, property, value);
}

void HardwareTest::log(const char *property, float value){
	Serial.printf("\n%s:%s:%f\n", currentTest, property, value);
}

void HardwareTest::log(const char *property, uint32_t value){
	Serial.printf("\n%s:%s:%u\n", currentTest, property, value);
}

void HardwareTest::log(const char *property, double value){
	Serial.printf("\n%s:%s:%lf\n", currentTest, property, value);
}

void HardwareTest::log(const char *property, bool value){
	Serial.printf("\n%s:%s:%d\n", currentTest, property, value ? 1 : 0);
}

void HardwareTest::log(const char* property, String value){
	Serial.printf("\n%s:%s:%s\n", currentTest, property, value.c_str());
}
