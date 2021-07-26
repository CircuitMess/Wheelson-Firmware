#ifndef WHEELSON_FIRMWARE_DRIVER_HPP
#define WHEELSON_FIRMWARE_DRIVER_HPP

#include <Loop/LoopListener.h>
#include <Display/Color.h>
#include <Util/Task.h>
#include <Sync/Mutex.h>
#include <Camera.h>
#include <Display/Sprite.h>

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
	void toggleMotors();

	uint8_t getParam() const;
	void setParam(uint8_t param);
	virtual const char* getParamName();
	virtual void drawParamControl(Sprite &sprite, int x, int y, uint w, uint h);

	bool camWorks() const;

protected:
	virtual void process() = 0;
	void setMotor(uint8_t id, int8_t state);

	Color* processedBuffer = nullptr;

	Mutex frameMutex;
	Mutex resultMutex;

	uint8_t param = 0;

private:
	Color* frameBuffer565 = nullptr;
	uint8_t* frameBuffer888 = nullptr;
	int8_t motors[4] = {0};
	bool motorsStop = true;

	Task task;

	Camera cam;
};

#endif //WHEELSON_FIRMWARE_DRIVER_HPP
