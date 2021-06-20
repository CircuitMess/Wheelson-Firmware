#include "LineDriver.h"
#include "../../Components/ImageProc.h"
#include "../../Components/SkeletonTracer.h"

LineDriver::LineDriver() : Driver(), thinningBuffer((Color*)ps_malloc(160 * 120 * sizeof(Color))){
	tracer = new SkeletonTracer();
	tracer->W = 160;
	tracer->H = 120;
	tracer->im = thinningBuffer;
}

LineDriver::~LineDriver(){
	delete tracer;
	free(thinningBuffer);
	delete line;
}

void LineDriver::process(){
	if(getCameraImage() == nullptr) return;
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
	line = new polyline_t();
	point_t* copyPoint = new point_t(*longestPolyPtr->head);
	line->head = copyPoint;
	memcpy(processedBuffer, Driver::getCameraImage(), 160 * 120 * sizeof(Color));
	for(point_t* point = longestPolyPtr->head->next; point != nullptr; point = point->next){
		copyPoint->next = new point_t(*point);
		copyPoint = copyPoint->next;
		if(point->next != nullptr){
			drawLine(point->x, point->y, point->next->x, point->next->y, processedBuffer, TFT_RED);
		}
	}
	copyPoint->next = nullptr;
	line->tail = copyPoint;

	tracer->destroy_polylines(p);
	tracer->destroy_polylines(line);

}

void LineDriver::drawLine(int x1, int y1, int x2, int y2, Color* buffer,uint32_t color){

        // Bresenham's line algorithm
  const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
  if(steep)
  {
    std::swap(x1, y1);
    std::swap(x2, y2);
  }

  if(x1 > x2)
  {
    std::swap(x1, x2);
    std::swap(y1, y2);
  }

  const float dx = x2 - x1;
  const float dy = fabs(y2 - y1);

  float error = dx / 2.0f;
  const int ystep = (y1 < y2) ? 1 : -1;
  int y = (int)y1;

  const int maxX = (int)x2;

	for(int x = (int) x1; x <= maxX; x++){
		if(steep){//x*120+y
			buffer[x * 160 + y] = color;
		}else{//y*160+x
			buffer[y * 160 + x] = color;
		}

    error -= dy;
    if(error < 0)
    {
        y += ystep;
        error += dx;
    }
  }
}
