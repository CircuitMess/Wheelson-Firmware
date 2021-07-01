#include <BallTracker.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include "BallDriver.h"

using namespace cv;

void BallDriver::process(){
	const Color* frameData = getCameraImage();

	std::vector<Ball> balls = BallTracker::detect((uint8_t*) getCameraImage888(), 160, 120, BallTracker::RGB888, displayMode == BW ? processedBuffer : nullptr);

	if(displayMode == RAW && balls.empty()){
		memcpy(processedBuffer, getCameraImage(), 160 * 120 * 2);
		return;
	}

	Mat draw(120, 160, CV_8UC2);
	memcpy(draw.data, getCameraImage(), 160 * 120 * 2);

	for(const auto& ball : balls){
		if(displayMode == RAW){
			cv::circle(draw, ball.center, ball.radius, Scalar(255, 0, 255));
		}
	}

	if(displayMode == RAW){
		memcpy(processedBuffer, draw.data, 160 * 120 * 2);
	}
}

void BallDriver::toggleDisplayMode(){
	displayMode = static_cast<DisplayMode>((displayMode+1) % DisplayMode::COUNT);
}
