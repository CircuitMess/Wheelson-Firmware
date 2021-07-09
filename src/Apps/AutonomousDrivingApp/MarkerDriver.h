#ifndef WHEELSON_FIRMWARE_MARKERDRIVER_H
#define WHEELSON_FIRMWARE_MARKERDRIVER_H


#include "../../Components/Driver.h"
#include <vector>
#include <Markers.h>

class MarkerDriver : public Driver {
public:
	MarkerDriver();
	~MarkerDriver() override;
	void toggleDisplayMode() override;
	void draw() override;

protected:
	void process() override;

private:
	Color* workingBuffer;
	std::vector<Aruco::Marker> markers;
	enum {
		IDLE,
		FORWARD,
		BACKWARD,
		BURNOUT,
		DO360,
		LEDON,
		LEDOFF
	} state = IDLE;

	enum DisplayMode {
		RAW,
		BW,
		COUNT
	} displayMode = RAW;
};


#endif //WHEELSON_FIRMWARE_MARKERDRIVER_H
