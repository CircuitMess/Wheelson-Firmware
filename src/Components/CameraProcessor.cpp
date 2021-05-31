#include "CameraProcessor.h"
#include "trace_skeleton.cpp"

void CameraProcessor::contrast(){
	camera_fb_t* frame = getFrame();
	uint16_t* buffer = getRaw();
	for(int i = 0; i < max(min(frame->width * frame->height, size_t(120 * 160)) - 1, (size_t) 0); i++){
		uint16_t color = buffer[i];
		double luminance = 0.2126 * ((color & 0xF800) >> 8) + 0.7152 * ((color & 0x07E0) >> 3) + 0.0722 * ((color & 0x1F) << 3);
		buffer[i] = (luminance <= 122) * 0xFFFF;
	}
}

CameraProcessor::CameraProcessor() : CameraFeed(){

}

uint16_t * CameraProcessor::skeletonize(){
	skeleton_tracer_t* T = new skeleton_tracer_t();
	T->W = getFrame()->width; // width of image
	T->H = getFrame()->height; // height of image

	// allocate the input image
	T->im = getRaw();
/*	// draw something interesting on the input image here...

	T->thinning_zs(); // perform raster thinning

	// run the algorithm
	skeleton_tracer_t::polyline_t* p = (skeleton_tracer_t::polyline_t*)T->trace_skeleton(0, 0, T->W, T->H, 0);

	// print out points in every polyline
	skeleton_tracer_t::polyline_t* it = p; //iterator
	while(it){
		skeleton_tracer_t::point_t* jt = it->head;
		while(jt){
			Serial.printf("%d,%d ",jt->x,jt->y);
			jt = jt->next;
		}
		Serial.println();
		it = it->next;
	}

	// clean up
	T->destroy_polylines(p);
	T->destroy_rects();*/

	delete T;
	return T->trace(getRaw(), 160, 128);


}


