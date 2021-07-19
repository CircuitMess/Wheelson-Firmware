#include <BallTracker.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include "BallDriver.h"
#include <Wheelson.h>

using namespace cv;

typedef struct {
	double h;       // angle in degrees
	double s;       // a fraction between 0 and 1
	double v;       // a fraction between 0 and 1
} hsv;

typedef struct {
	double r;       // a fraction between 0 and 1
	double g;       // a fraction between 0 and 1
	double b;       // a fraction between 0 and 1
} rgb;

rgb hsv2rgb(hsv in)
{
	double      hh, p, q, t, ff;
	long        i;
	rgb         out;

	if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
		out.r = in.v;
		out.g = in.v;
		out.b = in.v;
		return out;
	}
	hh = in.h;
	if(hh >= 360.0) hh = 0.0;
	hh /= 60.0;
	i = (long)hh;
	ff = hh - i;
	p = in.v * (1.0 - in.s);
	q = in.v * (1.0 - (in.s * ff));
	t = in.v * (1.0 - (in.s * (1.0 - ff)));

	switch(i) {
		case 0:
			out.r = in.v;
			out.g = t;
			out.b = p;
			break;
		case 1:
			out.r = q;
			out.g = in.v;
			out.b = p;
			break;
		case 2:
			out.r = p;
			out.g = in.v;
			out.b = t;
			break;

		case 3:
			out.r = p;
			out.g = q;
			out.b = in.v;
			break;
		case 4:
			out.r = t;
			out.g = p;
			out.b = in.v;
			break;
		case 5:
		default:
			out.r = in.v;
			out.g = p;
			out.b = q;
			break;
	}
	return out;

}

void BallDriver::process(){
	const Color* frameData = getCameraImage();

	std::vector<Ball> balls = BallTracker::detect((uint8_t*) getCameraImage888(), 160, 120, param > 0 ? param*180/255 : 0,
												  BallTracker::RGB888, displayMode == BW ? processedBuffer : nullptr);

	Ball* bestBall = nullptr;
	float maxFitness = 0;

	if(displayMode == RAW && balls.empty()){
		memcpy(processedBuffer, getCameraImage(), 160 * 120 * 2);
	}else{
		Mat draw(120, 160, CV_8UC2);
		memcpy(draw.data, getCameraImage(), 160 * 120 * 2);



		for(auto &ball : balls){
			if(ball.fitness > maxFitness){
				maxFitness = ball.fitness;
				bestBall = &ball;
			}
			if(displayMode == RAW){
				cv::circle(draw, ball.center, ball.radius, Scalar(255, 0, 255));
			}
		}
		if(displayMode == RAW){
			memcpy(processedBuffer, draw.data, 160 * 120 * 2);
		}
	}
	int currentX = 0;

	if(bestBall == nullptr || maxFitness == 0){
		if(lastX == -1 || noBallCounter > 4){
			setMotor(MOTOR_FL, 0);
			setMotor(MOTOR_BL, 0);
			setMotor(MOTOR_FR, 0);
			setMotor(MOTOR_BR, 0);
			return;
		}else{
			noBallCounter++;
			currentX = lastX;
		}
	}else{
		currentX = bestBall->center.x;
		noBallCounter = 0;
	}


	float amt = abs(80.0 - (float)currentX) / 80.0;
	float amtR, amtL;
	if(amt <= 0.1){
		amtL = 40;
		amtR = 40;
	}else{
		if(currentX < 80){
			amtR = 40.0f * amt + 40.0f;
			amtL = 0;
		}else{
			amtR = 0;
			amtL = 40.0f * amt + 40.0f;
		}
	}
	 lastX = currentX;

	setMotor(MOTOR_FL, (int8_t)amtL);
	setMotor(MOTOR_BL, (int8_t)amtL);
	setMotor(MOTOR_FR, (int8_t)amtR);
	setMotor(MOTOR_BR, (int8_t)amtR);

}

void BallDriver::toggleDisplayMode(){
	displayMode = static_cast<DisplayMode>((displayMode+1) % DisplayMode::COUNT);
}

const char* BallDriver::getParamName(){
	return "Ball color";
}

void BallDriver::drawParamControl(Sprite &sprite, int x, int y, uint w, uint h){
	float step = 360.0 / (float)max(0U, (w-3));
	int fill = max(0U, (w-3)) * param / 255;

	for (int i=0; i < w; i++){
		hsv in = {(double)step * i, 1.0, 1.0};
		rgb c = hsv2rgb(in);
		sprite.drawFastVLine(i + x, y+3, h-3, C_RGB(c.r * 255.0, c.g * 255.0, c.b * 255.0));
	}

	sprite.drawRect(x, y+2, w, h-1, TFT_WHITE);

	fill += 1;
	//sprite.fillTriangle(max(fill-2 + x, x), y, min(fill + 2 + x, x+(int)w), y, fill + x, y+2, TFT_BLACK);
	sprite.fillTriangle(max(x - 2, x + fill - 3), y, min((int) (x + w + 1), x + fill + 3), y, x + fill, y+3, TFT_BLACK);
}
