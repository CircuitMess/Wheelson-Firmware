#ifndef WHEELSON_FIRMWARE_BALLDRIVER_H
#define WHEELSON_FIRMWARE_BALLDRIVER_H


#include "../../Components/Driver.h"

class BallDriver : public Driver {
protected:
	void process() override;
public:
	void toggleDisplayMode() override;

private:
	enum DisplayMode {
		RAW,
		BW,
		COUNT
	} displayMode = RAW;
	int lastX = -1;
	uint8_t noBallCounter = 0;
};


#endif //WHEELSON_FIRMWARE_BALLDRIVER_H
