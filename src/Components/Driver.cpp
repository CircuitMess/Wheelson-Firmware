#include <Nuvoton/WheelsonMotor.h>
#include <Loop/LoopManager.h>
#include "Driver.h"


Driver::Driver(){

}

uint16_t* Driver::getCameraImage(){
	CameraFeed().loadFrame();
	CameraFeed().decodeFrame();
	frameBuffer = reinterpret_cast<Color*>(CameraFeed().getFrame());
	CameraFeed().releaseFrame();
	return frameBuffer;
}

uint Driver::getMotorState(uint id){
	return WheelsonMotor().getMotor(id);
}

void Driver::loop(uint micros){

}
