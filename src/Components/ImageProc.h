#ifndef WHEELSON_FIRMWARE_IMAGEPROC_H
#define WHEELSON_FIRMWARE_IMAGEPROC_H
#include <Arduino.h>

class ImageProc {
public:
	static void contrast(uint16_t* inBuffer, uint width, uint height, uint16_t* outBuffer, uint8_t luminanceThreshold);
};


#endif //WHEELSON_FIRMWARE_IMAGEPROC_H
