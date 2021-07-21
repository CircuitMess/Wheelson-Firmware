#ifndef WHEELSON_FIRMWARE_MARKERDRIVER_H
#define WHEELSON_FIRMWARE_MARKERDRIVER_H


#include "../../Components/Driver.h"
#include <vector>
#include <Markers.h>
#include <Display/Sprite.h>

class MarkerDriver : public Driver {
public:
	MarkerDriver();
	~MarkerDriver() override;
	void toggleDisplayMode() override;
	void draw() override;

protected:
	void process() override;

private:
	Sprite resultSprite;
	Color* workingBuffer;
	Color* bwBuffer;
	std::vector<Aruco::Marker> resultMarkers;
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

	uint32_t actionMillis = 0;
};


#endif //WHEELSON_FIRMWARE_MARKERDRIVER_H
