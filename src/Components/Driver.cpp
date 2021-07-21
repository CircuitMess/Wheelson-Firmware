#include <Nuvoton/WheelsonMotor.h>
#include <Loop/LoopManager.h>
#include <Wheelson.h>
#include "Driver.h"


Driver::Driver(): task("Driver", taskFunc, 20000, this){
	frameBuffer565 = static_cast<Color*>(ps_malloc(160 * 120 * sizeof(Color)));
	frameBuffer888 = static_cast<uint8_t*>(ps_malloc(160 * 120 * 3));
	processedBuffer = static_cast<Color*>(ps_malloc(160 * 120 * sizeof(Color)));
}

Driver::~Driver(){
	free(frameBuffer565);
	free(frameBuffer888);
	free(processedBuffer);
}

void Driver::start(){
	for(uint8_t i = 0; i < 4; i++){
		setMotor(i, 0);
	}
	task.start(1, 0);
}

void Driver::stop(){
	task.stop(true);
}

void Driver::taskFunc(Task* task){
	Driver* driver = static_cast<Driver*>(task->arg);

	while(task->running){
		if(!driver->cam.isInited()){
			delay(1);
			continue;
		}

		driver->process();
	}
}

void Driver::setMotor(uint8_t id, int8_t state){
	if(motorsStop) return;
	motors[id] = state;
	Motors.setMotor(id, state);
}

int8_t Driver::getMotorState(uint8_t id){
	return motors[id];
}

const Color* Driver::getCameraImage() const{
	return frameBuffer565;
}

bool Driver::isRunning() const{
	return !task.isStopped();
}

const Color* Driver::getProcessedImage() const{
	return processedBuffer;
}

void Driver::toggleDisplayMode(){

}

const uint8_t* Driver::getCameraImage888() const{
	return frameBuffer888;
}

uint8_t Driver::getParam() const{
	return param;
}

void Driver::setParam(uint8_t param){
	Driver::param = param;
}

const char* Driver::getParamName(){
	return nullptr;
}

void Driver::drawParamControl(Sprite &sprite, int x, int y, uint w, uint h){
	uint fill = w * param / 255;
	sprite.fillRoundRect(x, y, fill, h, 3, TFT_WHITE);
	sprite.drawRoundRect(x, y, w, h, 3, TFT_WHITE);
}

void Driver::toggleMotors(){
	motorsStop = !motorsStop;

	if(motorsStop){
		delay(10);
		Nuvo.getI2C().loop(0);

		for(uint8_t i = 0; i < 4; i++){
			motors[i] = 0;
		}

		Motors.stopAll();
	}
}

bool Driver::camWorks() const{
	return cam.isInited();
}

void Driver::prepareFrame(){
	cam.loadFrame();
	frameMutex.lock();
	memcpy(frameBuffer565, cam.getRGB565(), 160 * 120 * sizeof(Color));
	//memcpy(processedBuffer, cam.getRGB565(), 160 * 120 * sizeof(Color));
	memcpy(frameBuffer888, cam.getRGB888(), 160 * 120 * 3);
	frameMutex.unlock();
	cam.releaseFrame();
}
