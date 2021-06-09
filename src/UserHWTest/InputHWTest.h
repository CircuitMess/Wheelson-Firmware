#ifndef WHEELSON_FIRMWARE_INPUTHWTEST_H
#define WHEELSON_FIRMWARE_INPUTHWTEST_H

#include <Arduino.h>
#include <Support/Context.h>
#include <Loop/LoopListener.h>
#include <Input/Input.h>
#include <UI/LinearLayout.h>
#include "BtnTestElement.h"
#include "HWTestPart.h"
#include "UserHWTest.h"

class InputHWTest : public HWTestPart{
public:
	InputHWTest(UserHWTest* userHwTest);

	virtual ~InputHWTest();

	void draw() override;

	void start();

	void stop();

	void buttonPressed(uint id) override;

private:
	LinearLayout* screenLayout;
	LinearLayout* leftLayout;
	LinearLayout* midLayout;
	LinearLayout* rightLayout;

	std::vector<BtnTestElement*> leftBtnTest;
	std::vector<BtnTestElement*> midBtnTest;
	std::vector<BtnTestElement*> rightBtnTest;

	uint8_t doneCounter = 0;
	bool inputIsDone = false;

	void buildUI();

};

#endif //WHEELSON_FIRMWARE_INPUTHWTEST_H
