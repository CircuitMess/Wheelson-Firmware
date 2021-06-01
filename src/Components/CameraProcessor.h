#ifndef WHEELSON_FIRMWARE_CAMERAPROCESSOR_H
#define WHEELSON_FIRMWARE_CAMERAPROCESSOR_H

#include "CameraFeed.h"

class CameraProcessor : public CameraFeed {
public:
	CameraProcessor();
	void skeletonize();
	void contrast();

};


#endif //WHEELSON_FIRMWARE_CAMERAPROCESSOR_H
