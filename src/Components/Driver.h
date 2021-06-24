#ifndef WHEELSON_FIRMWARE_DRIVER_HPP
#define WHEELSON_FIRMWARE_DRIVER_HPP

#include <Loop/LoopListener.h>
#include <Display/Color.h>
#include <Util/Task.h>
#include "CameraFeed.h"

class Driver {
public:
	Driver();
	virtual ~Driver();

	void start();
	void stop();

	virtual uint getMotorState(uint id);
	virtual Color* getCameraImage();

	Color* getProcessedImage() const;

	bool isRunning() const;

	static void taskFunc(Task* task);

protected:
	virtual void process() = 0;
	void setMotor(uint8_t id, int8_t state);
	Color* processedBuffer = nullptr;

private:
	CameraFeed cam;
	Color* frameBuffer = nullptr;
	int8_t motors[4] = {0};

	Task task;
};

#endif //WHEELSON_FIRMWARE_DRIVER_HPP
