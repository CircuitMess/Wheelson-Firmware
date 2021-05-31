#ifndef WHEELSON_FIRMWARE_CAMERAPROCESSOR_H
#define WHEELSON_FIRMWARE_CAMERAPROCESSOR_H

#include "CameraFeed.h"

class CameraProcessor : public CameraFeed {
public:
	CameraProcessor();
	uint16_t *skeletonize();
	void contrast();

};


#endif //WHEELSON_FIRMWARE_CAMERAPROCESSOR_H
