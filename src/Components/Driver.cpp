#include <Nuvoton/WheelsonMotor.h>
#include <Loop/LoopManager.h>
#include <Wheelson.h>
#include "Driver.h"


Driver::Driver(): task("Driver", taskFunc, 4096, this){
	frameBuffer = static_cast<Color*>(ps_malloc(160 * 120 * sizeof(Color)));
	processedBuffer = static_cast<Color*>(ps_malloc(160 * 120 * sizeof(Color)));
}

Driver::~Driver(){
	free(frameBuffer);
	free(processedBuffer);
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
		memcpy(driver->frameBuffer, driver->cam.getRGB565(), 160 * 120 * sizeof(Color));

		driver->process();

		driver->cam.releaseFrame();
	}
}

void Driver::setMotor(uint8_t id, int8_t state){
	motors[id] = state;
	Motors.setMotor(id, state);
}

uint Driver::getMotorState(uint id){
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
