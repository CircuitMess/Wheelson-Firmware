#ifndef WHEELSON_FIRMWARE_DRIVER_HPP
#define WHEELSON_FIRMWARE_DRIVER_HPP

#include <Loop/LoopListener.h>
#include <Display/Color.h>
#include "CameraFeed.h"

class Driver : public LoopListener {
public:
	Driver();

	virtual uint16_t* getCameraImage();
	virtual uint getMotorState(uint id);
	virtual void start() = 0;
	virtual void stop() = 0;

	void loop(uint micros) override;

private:
protected:
	Color* frameBuffer = nullptr;

};

#endif //WHEELSON_FIRMWARE_DRIVER_HPP
