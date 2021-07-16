#include "BallDriver.h"
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <Wheelson.h>

using namespace cv;

BallDriver::BallDriver() : workingBuffer((uint8_t*)ps_malloc(160*120*3)){}

BallDriver::~BallDriver(){
	free(workingBuffer);
}

void BallDriver::process(){
	bufferMutex.lock();
	memcpy(workingBuffer, getCameraImage888(), 160*120*3);
	bufferMutex.unlock();

	DisplayMode mode = displayMode;
	if(mode == BW){
		resultsMutex.lock();
	}
	std::vector<Ball> balls = BallTracker::detect(workingBuffer, 160, 120, BallTracker::RGB888, displayMode == BW ? (Color*)workingBuffer : nullptr);
	if(mode == BW){
		resultsMutex.unlock();
	}
	ballsResult = balls;

	Ball* bestBall = nullptr;
	float maxFitness = 0;

	for(auto &ball : balls){
		if(ball.fitness > maxFitness){
			maxFitness = ball.fitness;
			bestBall = &ball;
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

void BallDriver::draw(){
	DisplayMode mode = displayMode;
	if(mode == RAW && !ballsResult.empty()){
		Mat draw(120, 160, CV_8UC2, processedBuffer);
		for(auto &ball : ballsResult){
			cv::circle(draw, ball.center, ball.radius, Scalar(255, 0, 255));
		}
	}else if(mode == BW){
		resultsMutex.lock();
		memcpy(processedBuffer, workingBuffer, 160*120*sizeof(Color));
		resultsMutex.unlock();
	}
}
