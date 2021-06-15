#include <Nuvoton/WheelsonMotor.h>
#include <Loop/LoopManager.h>
#include <Wheelson.h>
#include "Driver.h"


Driver::Driver(): task("Driver", taskFunc, 4096, this){
	frameBuffer = static_cast<Color*>(ps_malloc(160 * 120 * sizeof(Color)));
	processedBuffer = static_cast<Color*>(ps_malloc(160 * 120 * sizeof(Color)));
}

void Driver::start(){
	task.start(1, 0);
}

void Driver::stop(){
	task.stop(true);
}

void Driver::taskFunc(Task* task){
	Driver* driver = static_cast<Driver*>(task->arg);

	while(task->running){
		driver->cam.loadFrame();
		driver->cam.decodeFrame();
		memcpy(driver->frameBuffer, driver->cam.getRaw(), 160 * 120 * sizeof(Color));
		driver->cam.releaseFrame();

		driver->process();
	}
}

void Driver::setMotor(uint8_t id, int8_t state){
	motors[id] = state;
	Motors.setMotor(id, state);
}

uint Driver::getMotorState(uint id){
	return motors[id];
}

Color* Driver::getCameraImage(){
	return frameBuffer;
}

bool Driver::isRunning() const{
	return !task.isStopped();
}

Color* Driver::getProcessedImage() const{
	return processedBuffer;
}
