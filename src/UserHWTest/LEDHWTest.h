#ifndef WHEELSON_FIRMWARE_LEDHWTEST_H
#define WHEELSON_FIRMWARE_LEDHWTEST_H

#include <Arduino.h>
#include <Support/Context.h>
#include <Loop/LoopListener.h>
#include <Input/Input.h>
#include "HWTestPart.h"

class LEDHWTest : public HWTestPart,public LoopListener{
public:
	LEDHWTest(UserHWTest* userHwTest);

	virtual ~LEDHWTest();

	void loop(uint micros) override;

	void draw() override;

	void start();

	void stop() ;

	void buttonPressed(uint id) override;

private:

};

#endif //WHEELSON_FIRMWARE_LEDHWTEST_H
