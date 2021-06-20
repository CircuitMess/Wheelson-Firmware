#ifndef WHEELSON_FIRMWARE_LINEDRIVER_H
#define WHEELSON_FIRMWARE_LINEDRIVER_H

#include "../../Components/Driver.h"
#include "../../Components/CameraFeed.h"
#include <Display/Display.h>

struct polyline_t;
class SkeletonTracer;


class LineDriver : public Driver {
public:
	LineDriver();
	virtual ~LineDriver();

	virtual ~LineDriver();



protected:
	void process() override;
	Color* thinningBuffer;

private:
	polyline_t* line;
	SkeletonTracer* tracer;
	void drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, Color* buffer, uint32_t color);
};


#endif //WHEELSON_FIRMWARE_LINEDRIVER_H
