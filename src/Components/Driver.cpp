#include <Nuvoton/WheelsonMotor.h>
#include <Loop/LoopManager.h>
#include <Wheelson.h>
#include "Driver.h"


Driver::Driver(): task("Driver", taskFunc, 20000, this){
	frameBuffer = static_cast<Color*>(ps_malloc(160 * 120 * sizeof(Color)));
	frameBuffer888 = static_cast<Color*>(ps_malloc(160 * 120 * 3));
	processedBuffer = static_cast<Color*>(ps_malloc(160 * 120 * sizeof(Color)));
}

Driver::~Driver(){
	free(frameBuffer);
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
		driver->cam.loadFrame();
		memcpy(driver->frameBuffer, driver->cam.getRGB565(), 160 * 120 * sizeof(Color));
		memcpy(driver->frameBuffer888, driver->cam.getRGB888(), 160 * 120 * 3);

		driver->process();

		driver->cam.releaseFrame();
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
	return frameBuffer;
}

bool Driver::isRunning() const{
	return !task.isStopped();
}

const Color* Driver::getProcessedImage() const{
	return processedBuffer;
}

void Driver::toggleDisplayMode(){

}

const Color* Driver::getCameraImage888() const{
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
	for(uint8_t i = 0; i < 4; i++){
		setMotor(i, 0);
	}
	motorsStop = !motorsStop;
}

