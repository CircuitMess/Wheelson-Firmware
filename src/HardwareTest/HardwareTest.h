#ifndef WHEELSON_FIRMWARE_HARDWARETEST_H
#define WHEELSON_FIRMWARE_HARDWARETEST_H

#include <Arduino.h>
#include <Support/Context.h>
#include <Loop/LoopListener.h>
#include <Input/Input.h>
#include <UI/LinearLayout.h>
#include "BtnTestElement.h"
#include "../../Components/Driver.h"

class HardwareTest : public Context, public LoopListener, private InputListener {
public:
	HardwareTest(Display& display,Driver* driver);

	virtual ~HardwareTest();

	void loop(uint micros) override;

	void draw() override;

	void start() override;

	void stop() override;

private:
	static HardwareTest* instance;

	LinearLayout* screenLayout;
	LinearLayout* leftLayout;
	LinearLayout* midLayout;
	LinearLayout* rightLayout;

	std::vector<BtnTestElement*> leftBtnTest;
	std::vector<BtnTestElement*> midBtnTest;
	std::vector<BtnTestElement*> rightBtnTest;

	uint8_t doneCounter = 0;
	Driver* driver;
	bool inputIsDone = false;
	bool cameraIsDone = false;
	bool LEDisDone = false;

	Color* cameraBuffer = nullptr;

	void buildUI();

	void buttonPressed(uint id) override;


protected:
	void init() override;

	void deinit() override;
};

#endif //WHEELSON_FIRMWARE_HARDWARETEST_H
