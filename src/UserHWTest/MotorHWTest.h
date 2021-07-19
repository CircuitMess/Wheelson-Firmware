#ifndef WHEELSON_FIRMWARE_MOTORHWTEST_HPP
#define WHEELSON_FIRMWARE_MOTORHWTEST_HPP

#include <Arduino.h>
#include <Support/Context.h>
#include <Loop/LoopListener.h>
#include <Input/Input.h>
#include "HWTestPart.h"

class MotorHWTest : public HWTestPart,public LoopListener{
public:
	MotorHWTest(UserHWTest* userHwTest);

	virtual ~MotorHWTest();

	void loop(uint micros) override;

	void draw() override;

	void start() override;

	void stop() override;

	void buttonPressed(uint id) override;

private:
	void buttonReleased(uint i) override;
};
#endif //WHEELSON_FIRMWARE_MOTORHWTEST_HPP
