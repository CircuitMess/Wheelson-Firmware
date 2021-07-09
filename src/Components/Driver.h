#ifndef WHEELSON_FIRMWARE_DRIVER_HPP
#define WHEELSON_FIRMWARE_DRIVER_HPP

#include <Loop/LoopListener.h>
#include <Display/Color.h>
#include <Util/Task.h>
#include <Sync/Mutex.h>
#include <Camera.h>

class Driver {
public:
	Driver();
	virtual ~Driver();

	void start();
	void stop();
	virtual void draw() = 0;
	void prepareFrame();


	int8_t getMotorState(uint8_t id);
	const Color* getCameraImage() const;
	const uint8_t* getCameraImage888() const;
	const Color* getProcessedImage() const;

	bool isRunning() const;

	static void taskFunc(Task* task);

	virtual void toggleDisplayMode();


protected:
	virtual void process() = 0;
	void setMotor(uint8_t id, int8_t state);
	Color* processedBuffer = nullptr;
	Mutex bufferMutex;


private:
	Color* frameBuffer = nullptr;
	uint8_t* frameBuffer888 = nullptr;
	int8_t motors[4] = {0};

	Task task;

	Camera cam;
};

#endif //WHEELSON_FIRMWARE_DRIVER_HPP
