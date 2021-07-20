#ifndef WHEELSON_FIRMWARE_DRIVER_HPP
#define WHEELSON_FIRMWARE_DRIVER_HPP

#include <Loop/LoopListener.h>
#include <Display/Color.h>
#include <Util/Task.h>
#include <Camera.h>
#include <string>
#include <Display/Sprite.h>

class Driver {
public:
	Driver();
	virtual ~Driver();

	void start();
	void stop();

	int8_t getMotorState(uint8_t id);
	const Color* getCameraImage() const;
	const Color* getCameraImage888() const;
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
	uint8_t param = 0;

private:
	Color* frameBuffer = nullptr;
	Color* frameBuffer888 = nullptr;
	int8_t motors[4] = {0};
	bool motorsStop = true;

	Task task;

	Camera cam;
};

#endif //WHEELSON_FIRMWARE_DRIVER_HPP
