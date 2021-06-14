#ifndef WHEELSON_FIRMWARE_LINEDRIVER_H
#define WHEELSON_FIRMWARE_LINEDRIVER_H

#include "../../Components/Driver.h"
#include "../../Components/CameraFeed.h"
#include <Display/Display.h>
#include "../../Components/trace_skeleton.cpp"

class LineDriver : public Driver {
public:
	LineDriver();

	Color* getCameraImage() override;

protected:
	void process() override;
	Color* thinningBuffer;

private:
	skeleton_tracer_t::polyline_t line;

	void drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, Color* buffer, uint32_t color);

};


#endif //WHEELSON_FIRMWARE_LINEDRIVER_H
