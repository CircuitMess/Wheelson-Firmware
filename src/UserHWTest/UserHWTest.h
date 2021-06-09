#ifndef WHEELSON_FIRMWARE_USERHWTEST_H
#define WHEELSON_FIRMWARE_USERHWTEST_H

#include <Arduino.h>
#include <Support/Context.h>
#include <Loop/LoopListener.h>
#include <Input/Input.h>
#include <UI/LinearLayout.h>

class HWTestPart;

class UserHWTest : public Context, public InputListener {
	friend HWTestPart;
public:
	UserHWTest(Display& display);

	virtual ~UserHWTest();

	void draw() override;

	void start() override;

	void stop() override;

	void buttonPressed(uint id) override;

	void currentTestDone();

private:
	HWTestPart* hwTestPart;

	uint8_t testCounter = 0;
	void nextTest();
};

#endif //WHEELSON_FIRMWARE_USERHWTEST_H
