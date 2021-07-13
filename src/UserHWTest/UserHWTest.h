#ifndef WHEELSON_FIRMWARE_USERHWTEST_H
#define WHEELSON_FIRMWARE_USERHWTEST_H

#include <Arduino.h>
#include <Support/Context.h>
#include <Loop/LoopListener.h>
#include <Input/Input.h>
#include <UI/LinearLayout.h>

class HWTestPart;

class UserHWTest : public Context {
	friend HWTestPart;
public:
	UserHWTest(Display& display);

	virtual ~UserHWTest();

	void draw() override;

	void start() override;

	void stop() override;

	void currentTestDone();

	void setDoneCallback(void (* doneCallback)(UserHWTest*));

protected:
	void init() override;

	void deinit() override;

private:
	HWTestPart* hwTestPart;

	uint8_t testCounter = 0;
	void nextTest();
	Color* backgroundBuffer= nullptr;
	void (* doneCallback)(UserHWTest* test) = nullptr;
};

#endif //WHEELSON_FIRMWARE_USERHWTEST_H
