#ifndef WHEELSON_FIRMWARE_CAMERAPROCESSOR_H
#define WHEELSON_FIRMWARE_CAMERAPROCESSOR_H

#include "CameraFeed.h"

class CameraProcessor {
public:
	void skeletonize();
private:
	CameraFeed cameraFeed;
	void contrast();

	bool thinningIteration(int iter);
	void thinning();
};


#endif //WHEELSON_FIRMWARE_CAMERAPROCESSOR_H
