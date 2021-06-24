#include "ImageProc.h"

void ImageProc::contrast(uint16_t* inBuffer, uint width, uint height, uint16_t* outBuffer, uint8_t luminanceThreshold){
	for(uint i = 0; i < width*height ; ++i){
		uint16_t color = inBuffer[i];
		double luminance = 0.2126 * ((color & 0xF800) >> 8) + 0.7152 * ((color & 0x07E0) >> 3) + 0.0722 * ((color & 0x1F) << 3);
		outBuffer[i] = (luminance <= luminanceThreshold) * 0xFFFF;
	}
}
