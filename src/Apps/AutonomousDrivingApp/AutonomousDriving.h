#ifndef WHEELSON_FIRMWARE_AUTONOMOUSDRIVING_H
#define WHEELSON_FIRMWARE_AUTONOMOUSDRIVING_H

#include <Arduino.h>
#include <Support/Context.h>
#include <UI/Layout.h>
#include <Elements/GridMenu.h>
#include <Input/InputListener.h>
#include <Loop/LoopListener.h>
#include "DrivingElement.h"
#include "../../Components/Driver.h"

class AutonomousDriving : public Context, public LoopListener {
public:
	AutonomousDriving(Display& display, Driver* driver);

	virtual ~AutonomousDriving();

	void start() override;

	void stop() override;

	void draw() override;

	void loop(uint micros) override;

protected:
	void init() override;

	void deinit() override;

private:
	static AutonomousDriving* instance;

	LinearLayout* screenLayout;
	std::vector<DrivingElement*> engines;
	Color* backgroundBuffer = nullptr;
	const Color* cameraBuffer = nullptr;
	Driver* driver;
	uint8_t motorPower[4] = {0};
	char buffer[4];

	void buildUI();

};

#endif //WHEELSON_FIRMWARE_AUTONOMOUSDRIVING_H
