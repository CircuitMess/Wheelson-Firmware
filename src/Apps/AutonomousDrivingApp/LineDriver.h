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

protected:
	void process() override;
	Color* thinningBuffer;

private:
	polyline_t* line;
	SkeletonTracer* tracer;
};


#endif //WHEELSON_FIRMWARE_LINEDRIVER_H
