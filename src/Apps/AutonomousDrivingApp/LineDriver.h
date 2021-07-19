#ifndef WHEELSON_FIRMWARE_LINEDRIVER_H
#define WHEELSON_FIRMWARE_LINEDRIVER_H


#include "../../Components/Driver.h"

class LineDriver : public Driver {
public:
	LineDriver();
	void toggleDisplayMode() override;
	const char* getParamName() override;

protected:
	void process() override;

private:
	enum DisplayMode {
		RAW,
		GRAY,
		THRESH_SIMPLE,
		//THRESH_ADAPTIVE,
		COUNT
	} displayMode = RAW;

	int lastx = -1;
	float lastAng = 0;

	void rotL();
	void rotR();
};


#endif //WHEELSON_FIRMWARE_LINEDRIVER_H
