#ifndef AUTOCAR_MOTORS_H
#define AUTOCAR_MOTORS_H

#include <Arduino.h>
#include <Input/I2cExpander.h>
#include <Sync/Mutex.h>
#define MOTOR_FORWARD 0
#define MOTOR_BACKWARD 1
#define MOTOR_STOP 2


class Motors
{
public:
	Motors(I2cExpander* i2c = nullptr, Mutex* _mutex = nullptr);
	void begin(uint8_t leftA, uint8_t leftB, uint8_t rightA, uint8_t rightB);
	void leftMotor(uint8_t action);
	void rightMotor(uint8_t action);
	void stopAll();
	static Motors* getInstance();
private:
	void motor(bool motor, uint8_t action);
	uint8_t pinA[2];
	uint8_t pinB[2];
	uint8_t currentAction[2];
	I2cExpander* i2c;
	Mutex* mutex;
	static Motors* instance;
};

#endif