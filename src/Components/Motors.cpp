#include "Motors.h"
Motors* Motors::instance = nullptr;

//Motors start
//------------------------------------------------------
Motors::Motors( I2cExpander* _i2c, Mutex* _mutex) : i2c(_i2c), mutex(_mutex)
{
	instance = this;
}
void Motors::begin(uint8_t leftA, uint8_t leftB, uint8_t rightA, uint8_t rightB)
{
	pinA[0] = leftA;
	pinB[0] = leftB;
	pinA[1] = rightA;
	pinB[1] = rightB;
	if(i2c == nullptr)
	{
		pinMode(pinA[0], OUTPUT);
		pinMode(pinA[1], OUTPUT);
		pinMode(pinB[0], OUTPUT);
		pinMode(pinB[1], OUTPUT);
	}
	else
	{
		if(mutex != nullptr)
			mutex->lock();
		i2c->pinMode(pinA[0], OUTPUT);
		i2c->pinMode(pinA[1], OUTPUT);
		i2c->pinMode(pinB[0], OUTPUT);
		i2c->pinMode(pinB[1], OUTPUT);
		if(mutex != nullptr)
			mutex->unlock();
	}
	stopAll();
}
void Motors::leftMotor(uint8_t action)
{
	motor(0, action);
}
void Motors::rightMotor(uint8_t action)
{
	motor(1, action);
}
void Motors::motor(bool motor, uint8_t action)
{
	if(currentAction[motor] == action)
		return;
	currentAction[motor] = action;
	if(i2c != nullptr && mutex != nullptr)
		mutex->lock();
	switch (action)
	{
	case MOTOR_FORWARD:
		if(i2c != nullptr)
		{
			i2c->pinWrite(pinA[motor], 1);
			i2c->pinWrite(pinB[motor], 0);
		}
		else
		{
			digitalWrite(pinA[motor], 1);
			digitalWrite(pinB[motor], 0);
		}
		break;

	case MOTOR_BACKWARD:
		if(i2c != nullptr)
		{
			i2c->pinWrite(pinA[motor], 0);
			i2c->pinWrite(pinB[motor], 1);
		}
		else
		{
			digitalWrite(pinA[motor], 0);
			digitalWrite(pinB[motor], 1);
		}
		break;

	case MOTOR_STOP:
		if(i2c != nullptr)
		{
			i2c->pinWrite(pinA[motor], 0);
			i2c->pinWrite(pinB[motor], 0);
		}
		else
		{
			digitalWrite(pinA[motor], 0);
			digitalWrite(pinB[motor], 0);
		}
		break;
	
	default:
		break;
	}
	if(i2c != nullptr && mutex != nullptr)
		mutex->unlock();
}
void Motors::stopAll()
{
	motor(0, MOTOR_STOP);
	motor(1, MOTOR_STOP);
}
Motors* Motors::getInstance()
{
	return instance;
}

//Motors end
//------------------------------------------------------
//Leds start
