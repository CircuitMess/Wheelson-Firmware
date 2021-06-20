#include "LineDriver.h"
#include "../../Components/ImageProc.h"
#include "../../Components/SkeletonTracer.h"

LineDriver::LineDriver() : Driver(), thinningBuffer((Color*)ps_malloc(160 * 120 * sizeof(Color))){
	tracer = new SkeletonTracer();
	tracer->W = 160;
	tracer->H = 120;
	tracer->im = thinningBuffer;
}

void LineDriver::process(){

	memcpy(thinningBuffer, getCameraImage(), 160 * 120 * sizeof(Color));

	//B/W contrasting
	ImageProc::contrast(thinningBuffer, 160, 120, thinningBuffer, 122);



	//algorithm expects binary image
	for(uint i = 0; i < tracer->H * tracer->H; i++){
		thinningBuffer[i] &= 1;
	}
	tracer->thinning_zs();

	polyline_t* p = (polyline_t*)tracer->trace_skeleton(0, 0, tracer->W, tracer->H, 0);

	//iterate through polylines and find the longest
	polyline_t* pl = p;
	float maxLength = 0;
	polyline_t* longestPolyPtr;
	while(pl){
		float polylineLength = 0;
		point_t* point = pl->head;
		while(point->next){
			polylineLength += sqrt(pow(point->x - point->next->x, 2) + pow(point->y - point->next->y, 2));
			point = point->next;
		}
		if(maxLength < polylineLength){
			maxLength = polylineLength;
			longestPolyPtr = pl;
		}
		pl = pl->next;
	}

	//copy longest polyline, point by point
	point_t* copyPoint = new point_t(*longestPolyPtr->head);
	line->head = copyPoint;
	for (point_t* point = longestPolyPtr->head->next; point != nullptr; point = point->next){
		copyPoint->next = new point_t(*point);
		copyPoint = copyPoint->next;
	}
	copyPoint->next = nullptr;
	line->tail = copyPoint;
}

LineDriver::~LineDriver(){
	delete tracer;
	free(thinningBuffer);
}

