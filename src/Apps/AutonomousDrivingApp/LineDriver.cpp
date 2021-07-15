#include <opencv2/core/mat.hpp>
#include <MarkerDetection/imgproc.h>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc.hpp>
#include <queue>
#include <Wheelson.h>
#include "LineDriver.h"

using namespace cv;

std::vector<Point2i> findLine(uint8_t* data, uint16_t w, uint16_t h){
	std::vector<Point2i> line;

	for(int y = h - 1; y >= 0; y--){
		int start = -1;

		struct Part {
			int start, stop;
			Part(int start, int stop) : start(start), stop(stop){ }
			int width() const { return stop - start; }
		};

		struct PartComparator {
			bool operator()(const Part& p1, const Part& p2){
				return p1.width() < p2.width();
			}
		};

		std::priority_queue<Part, std::vector<Part>, PartComparator> parts;

		for(int x = 0; x < w; x++){
			uint8_t pix = data[y*w + x];

			if(pix == 255 && start != -1){
				int stop = x;

				// process part
				parts.emplace(start, stop);

				start = -1;
			}else if(pix == 0 && start == -1){
				start = x;
			}
		}

		if(parts.empty() || parts.top().width() <= 6){
			line.emplace_back(-1, y);
		}else{
			line.emplace_back((parts.top().stop + parts.top().start) / 2, y);
		}
	}

	int numNeg = 0;
	int i = 0;
	for(const auto& p : line){
		if(p.x == -1) numNeg++;
		if(i++ >= line.size()/2) break;
	}

	if(numNeg >= line.size()/4){
		return std::vector<Point2i>();
	}

	return line;
}
LineDriver::LineDriver(){
	setParam(120); //default threshold
}

void LineDriver::rotL(){
	setMotor(MOTOR_FL, -60);
	setMotor(MOTOR_BL, -60);
	setMotor(MOTOR_FR, 60);
	setMotor(MOTOR_BR, 60);
}

void LineDriver::rotR(){
	setMotor(MOTOR_FL, 60);
	setMotor(MOTOR_BL, 60);
	setMotor(MOTOR_FR, -60);
	setMotor(MOTOR_BR, -60);
}

void LineDriver::process(){
	Mat frame(120, 160, CV_8UC3);
	memcpy(frame.data, getCameraImage888(), 160 * 120 * 3);

	resize(frame, frame, Size(), 0.5, 0.5);

	Mat gray;
	cvtColor(frame, gray, CV_RGB2GRAY);

	add(gray, Scalar(50), gray);
	rectangle(gray, Rect(cv::Point(0, 0), gray.size()), Scalar(255));

	Mat blur;
	//cv::boxFilter(gray, blur, gray.depth(), cv::Size(8, 8), cv::Point(-1,-1), true, BORDER_REPLICATE);

	Mat thresh;
	threshold(gray, thresh, getParam(), 255, THRESH_BINARY);

	/*rectangle(thresh1, Rect(cv::Point(0, 0), thresh1.size()), Scalar(0));

	Mat t1inv;
	bitwise_not(thresh1, t1inv);

	Mat thresh;
	adaptive(t1inv, thresh, frame.cols / 6);

	bitwise_not(thresh, thresh);*/

	Mat draw;
	if(displayMode == RAW){
		cvtColor(frame, draw, CV_RGB2BGR565);
	}else if(displayMode == GRAY){
		cvtColor(gray, draw, CV_GRAY2BGR565);
	}else if(displayMode == THRESH_SIMPLE){
		cvtColor(thresh, draw, CV_GRAY2BGR565);
	}/*else if(displayMode == THRESH_ADAPTIVE){
		cvtColor(thresh, draw, CV_GRAY2BGR565);
	}*/

	auto linePoints = findLine(thresh.data, thresh.cols, thresh.rows);
	if(linePoints.empty()){
		resize(draw, draw, Size(), 2, 2, INTER_NEAREST);
		memcpy(processedBuffer, draw.data, 120 * 160 * 2);

		if(lastx == -1) return;

		if(lastx < thresh.cols/2){
			rotL();
		}else{
			rotR();
		}

		return;
	}

	std::vector<Point2i> approx;
	approxPolyDP(linePoints, approx, 0.1, false);

#define window (frame.cols / 16)
#define weightFalloff 0.8

	float weightLookup[window+1];
	for(int i = 0; i < window+1; i++){
		weightLookup[i] = pow(weightFalloff, i);
	}

	std::vector<Point2i> smoothed;
	for(int i = 0; i < approx.size(); i++){

		float val = 0;
		float totalWeight = 0;

		for(int j = max(0, i-window); j < min(approx.size(), (size_t) i+window+1); j++){
			int d = abs(i - j);
			float weight = weightLookup[d];//pow(weightFalloff, d);

			val += (float) approx[j].x * weight;
			totalWeight += weight;
		}

		smoothed.emplace_back((int) (val / totalWeight), approx[i].y);
	}

	std::vector<Point2i> broken = smoothed;
	/*for(int i = 0; i < approx.size(); i += window){
		float val = 0;
		float totalWeight = 0;

		for(int j = max(0, i-window); j < min(approx.size(), (size_t) i+window+1); j++){
			int d = abs(i - j);
			float weight = weightLookup[d];//pow(weightFalloff, d);

			val += (float) approx[j].x * weight;
			totalWeight += weight;
		}

		broken.emplace_back((int) (val / totalWeight), approx[i].y);
	}*/

	for(int i = 1; i < broken.size(); i++){
		line(draw, broken[i-1], broken[i], C_RGB(0, 0, 255));
	}

	int midIndex = (int) ((float) broken.size() * 0.75f);
	line(draw, broken[1], broken[midIndex], Scalar(C_RGB(0, 0, 255) >> 8, C_RGB(0, 0, 255) & 0xff));

	if(broken[1].x == -1 || broken[1].x == 0 || broken[midIndex].x == -1 || broken[midIndex].x == 0){
		resize(draw, draw, Size(), 2, 2, INTER_NEAREST);
		memcpy(processedBuffer, draw.data, 120 * 160 * 2);

		if(lastx == -1) return;

		if(lastx < thresh.cols/2){
			rotL();
		}else{
			rotR();
		}

		return;
	}

	Point2i p1 = broken[1], p2 = broken[midIndex];
	float angT = ((float) p2.y - (float) p1.y) / ((float) p1.x - (float) p2.x);
	float ang = atan(angT);

	ang += M_PI_2;
	if(ang >= M_PI_2) ang -= M_PI;
	ang *= -1;
	// printf("tg: %.3f, ang: %.3f\n", angT, ang);

	ang = constrain(ang, -1, 1);

	auto map = [](float x, float in_min, float in_max, float out_min, float out_max){
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	};


	int xpos = broken[1].x;
	int xpos2 = broken[midIndex].x;

	if(abs(xpos - xpos2) < frame.cols/4){
		float midp = (float) (xpos + xpos2) / 2.0f;

		float amt = (float) abs(midp - frame.cols/2) / ((float) frame.cols/2.0f);

		float amtL, amtR;

		if(midp < frame.cols / 2){
			amtR = 60.0f * amt + 20.0f;
			amtL = 60.0f * (1.0f - amt) + 20.0f;
		}else{
			amtR = 60.0f * (1.0f - amt) + 20.0f;
			amtL = 60.0f * amt + 20.0f;
		}

		setMotor(MOTOR_FL, (int) amtL);
		setMotor(MOTOR_BL, (int) amtL);
		setMotor(MOTOR_FR, (int) amtR);
		setMotor(MOTOR_BR, (int) amtR);
	}else if(ang < M_PI_2 && ang >= -M_PI_2){
		float lAmtF = map(ang, -1, 1, 0, 1);
		float rAmtF = map(ang, 1, -1, 0, 1);

		if(lAmtF > rAmtF){
			lAmtF *= 60.0f;
			rAmtF *= 20.0f;
		}else{
			lAmtF *= 20.0f;
			rAmtF *= 60.0f;
		}

		int lAmt = (int) lAmtF + 20;
		int rAmt = (int) rAmtF + 20;

		setMotor(MOTOR_FL, lAmt);
		setMotor(MOTOR_BL, lAmt);
		setMotor(MOTOR_FR, rAmt);
		setMotor(MOTOR_BR, rAmt);

		printf("Motor L: %d, Motor R: %d\n", lAmt, rAmt);
	}

	lastx = xpos;

	resize(draw, draw, Size(), 2, 2, INTER_NEAREST);

	memcpy(processedBuffer, draw.data, 120 * 160 * 2);
}

void LineDriver::toggleDisplayMode(){
	displayMode = static_cast<DisplayMode>((displayMode+1) % DisplayMode::COUNT);
}

const char* LineDriver::getParamName(){
	return "Contrast";
}
