#ifndef WHEELSON_FIRMWARE_DRIVER_HPP
#define WHEELSON_FIRMWARE_DRIVER_HPP

#include <Loop/LoopListener.h>
#include <Display/Color.h>
#include <Util/Task.h>
#include <Camera.h>

class Driver {
public:
	Driver();
	virtual ~Driver();

	void start();
	void stop();

	uint getMotorState(uint id);
	const Color* getCameraImage() const;
	const Color* getCameraImage888() const;

	const Color* getProcessedImage() const;

	bool isRunning() const;

	static void taskFunc(Task* task);

	virtual void toggleDisplayMode();

protected:
	virtual void process() = 0;
	void setMotor(uint8_t id, int8_t state);
	Color* processedBuffer = nullptr;

private:
	Color* frameBuffer = nullptr;
	Color* frameBuffer888 = nullptr;
	int8_t motors[4] = {0};

	Task task;

	Camera cam;
};

#endif //WHEELSON_FIRMWARE_DRIVER_HPP
