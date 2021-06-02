#ifndef WHEELSON_FIRMWARE_LINEDRIVER_H
#define WHEELSON_FIRMWARE_LINEDRIVER_H

#include "../../Components/Driver.h"
#include "../../Components/CameraFeed.h"
#include <Display/Display.h>
#include "../../Components/trace_skeleton.cpp"

class LineDriver : public Driver {
public:
	LineDriver();

protected:
	void process() override;
	Color* thinningBuffer;

private:
	skeleton_tracer_t::polyline_t line;
};


#endif //WHEELSON_FIRMWARE_LINEDRIVER_H
