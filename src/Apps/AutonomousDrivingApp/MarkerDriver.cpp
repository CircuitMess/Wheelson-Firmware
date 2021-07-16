#include "MarkerDriver.h"
#include <Util/Vector.h>
#include <Wheelson.h>

Vector<uint16_t> actions({
								750, // forward
								686, // backward
								636, // LED on
								576, // LED off
								5, // 360
								785 // burnout
						});

#define setAll(v) for(int i = 0; i < 4; i++) setMotor(i, v);

void drawLine(int x1, int y1, int x2, int y2, Color* buffer,uint32_t color){

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

MarkerDriver::MarkerDriver() : workingSprite(Sprite(Wheelson.getDisplay(), 160, 120)), workingBuffer((Color*)workingSprite.getPointer()),
							   resultBuffer((Color*)ps_malloc(160*120*sizeof(Color))){}

MarkerDriver::~MarkerDriver(){
	free(resultBuffer);
}

void MarkerDriver::process(){
	bufferMutex.lock();
	memcpy(workingBuffer, getCameraImage(), 160*120*sizeof(Color));
	bufferMutex.unlock();

	if(displayMode == BW){
		resultsMutex.lock();
	}
	markers = Markers::detect((uint8_t*) workingBuffer, 160, 120, Markers::RGB565, displayMode == BW ? workingBuffer : nullptr);

	if(displayMode == BW){
		memcpy(resultBuffer, workingBuffer, 160 * 120 * 2);
		resultsMutex.unlock();
	}

	if(markers.empty() || actions.indexOf(markers[0].id) == (uint) -1){
		if(state != IDLE){
			setAll(0);
		}

		state = IDLE;
		return;
	}

	uint16_t id = markers[0].id;
	uint8_t action = actions.indexOf(id);

	if(action == 0 && state != FORWARD){
		setAll(127);
		state = FORWARD;
	}else if(action == 1 && state != BACKWARD){
		setAll(-127);
		state = BACKWARD;
	}else if(action == 2 && state != LEDON){
		LED.setHeadlight(255);
		state = LEDON;
	}else if(action == 3 && state != LEDOFF){
		LED.setHeadlight(0);
		state = LEDOFF;
	}else if(action == 4 && state != DO360){
		// do 360
		bool direction = random(0, 2);
		int8_t intensity = 127;
		if(direction){
			intensity *= -1;
		}
		Motors.setMotor(MOTOR_FR, intensity);
		Motors.setMotor(MOTOR_BR, intensity);
		intensity *= -1;
		Motors.setMotor(MOTOR_FL, intensity);
		Motors.setMotor(MOTOR_BL, intensity);
		delay(1250);
		state = DO360;
	}
	else if(action == 5 && state != BURNOUT){
		// do burnout
		setMotor(MOTOR_FL, -40);
		setMotor(MOTOR_FR, -40);
		setMotor(MOTOR_BL, 80);
		setMotor(MOTOR_BR, 80);
		state = BURNOUT;
	}
}

void MarkerDriver::toggleDisplayMode(){
	displayMode = static_cast<DisplayMode>((displayMode+1) % DisplayMode::COUNT);
}

void MarkerDriver::draw(){
	if(displayMode == BW){
		resultsMutex.lock();
		memcpy(processedBuffer, workingBuffer, 160*120*sizeof(Color));
		resultsMutex.unlock();
	}
	if(markers.empty()) return;
	for(const auto& marker : markers){
		auto points = marker.projected;
		for(int i = 1; i < 4; i++){
			drawLine(points[i-1].x * 2, points[i-1].y * 2, points[i].x * 2, points[i].y * 2, processedBuffer, TFT_RED);
		}
		drawLine(points[3].x * 2, points[3].y * 2, points[0].x * 2, points[0].y * 2, processedBuffer, TFT_RED);
	}
}
