#ifndef WHEELSON_FIRMWARE_BALLDRIVER_H
#define WHEELSON_FIRMWARE_BALLDRIVER_H


#include "../../Components/Driver.h"
#include <BallTracker.h>

class BallDriver : public Driver {
public:
	BallDriver();
	~BallDriver() override;
	const char* getParamName() override;
	void toggleDisplayMode() override;
	void draw() override;
	void drawParamControl(Sprite &sprite, int x, int y, uint w, uint h) override;

protected:
	void process() override;

private:
	uint8_t* workingBuffer;
	std::vector<Ball> ballsResult;
	enum DisplayMode {
		RAW,
		BW,
		COUNT
	} displayMode = RAW;
	int lastX = -1;
	uint8_t noBallCounter = 0;
};


#endif //WHEELSON_FIRMWARE_BALLDRIVER_H
