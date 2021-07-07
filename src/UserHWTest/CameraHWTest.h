#ifndef WHEELSON_FIRMWARE_CAMERAHWTEST_H
#define WHEELSON_FIRMWARE_CAMERAHWTEST_H

#include <Arduino.h>
#include <Support/Context.h>
#include <UI/Layout.h>
#include <Elements/GridMenu.h>
#include <Input/InputListener.h>
#include <Loop/LoopListener.h>
#include <Camera.h>
#include "UserHWTest.h"
#include "HWTestPart.h"

class CameraHWTest : public HWTestPart, public LoopListener{
public:
	CameraHWTest(UserHWTest* userHwTest);

	virtual ~CameraHWTest();

	void loop(uint micros) override;

	void draw() override;

	void start() override;

	void stop() override;

	void buttonPressed(uint id) override;

private:
	Camera cameraFeed;

};

#endif //WHEELSON_FIRMWARE_CAMERAHWTEST_H
