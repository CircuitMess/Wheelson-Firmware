#include "CameraProcessor.h"

void CameraProcessor::skeletonize(){
	cameraFeed.loadFrame();
	contrast();
}

void CameraProcessor::contrast(){
	camera_fb_t* frame = cameraFeed.getFrame();
	uint16_t* buffer = cameraFeed.getRaw();
	for(int i = 0; i < max(min(frame->width * frame->height, 120 * 160) - 1, (size_t) 0); i++){
		uint16_t color = buffer[i];
		double luminance = 0.2126 * ((color & 0xF800) >> 8) + 0.7152 * ((color & 0x07E0) >> 3) + 0.0722 * ((color & 0x1F) << 3);
		buffer[i] = (luminance > 0.8) * 0xFFFF;
	}
}

bool CameraProcessor::thinningIteration(int iter) {
	camera_fb_t* frame = cameraFeed.getFrame();
	int H = frame->height;
	int W = frame->width;
	uint16_t* im = cameraFeed.getRaw();
	bool diff = false;
	for (int i = 1; i < H-1; i++){
		for (int j = 1; j < W-1; j++){
			int p2 = im[(i-1)*W+j]   & 1;
			int p3 = im[(i-1)*W+j+1] & 1;
			int p4 = im[(i)*W+j+1]   & 1;
			int p5 = im[(i+1)*W+j+1] & 1;
			int p6 = im[(i+1)*W+j]   & 1;
			int p7 = im[(i+1)*W+j-1] & 1;
			int p8 = im[(i)*W+j-1]   & 1;
			int p9 = im[(i-1)*W+j-1] & 1;

			int A  = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
					 (p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
					 (p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
					 (p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);
			int B  = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
			int m1 = iter == 0 ? (p2 * p4 * p6) : (p2 * p4 * p8);
			int m2 = iter == 0 ? (p4 * p6 * p8) : (p2 * p6 * p8);
			if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
				im[i*W+j] |= 2;
		}
	}
	for (int i = 0; i < H*W; i++){
		int marker = im[i]>>1;
		int old = im[i]&1;
		im[i] = old & (!marker);
		if ((!diff) && (im[i] != old)){
			diff = true;
		}
	}
	return diff;
};
void CameraProcessor::thinning(){
	bool diff = true;
	do {
		diff &= thinningIteration(0);
		diff &= thinningIteration(1);
	}while (diff);
}