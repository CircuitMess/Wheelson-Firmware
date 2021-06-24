#ifndef WHEELSON_FIRMWARE_HARDWARETEST_H
#define WHEELSON_FIRMWARE_HARDWARETEST_H
#include <Display/Display.h>
#include <Util/Vector.h>
#include <Input/Input.h>
#include <FS.h>
#include <Wheelson.h>
#include <Util/Task.h>

struct Test {
	bool (*test)();
	const char* name;
};

class HardwareTest
{
public:
	HardwareTest(Display& display);
	void start();

private:
	Sprite *canvas;
	Display *display;

	static HardwareTest* test;
	Vector<Test> tests;
	const char* currentTest;

	void log(const char* property, char* value);
	void log(const char* property, float value);
	void log(const char* property, double value);
	void log(const char* property, bool value);
	void log(const char *property, uint32_t value);
	void log(const char *property, String value);

	static bool psram();
	static bool nuvotonTest();
	static bool SPIFFSTest();


};


#endif //WHEELSON_FIRMWARE_HARDWARETEST_H
