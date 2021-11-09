#ifndef WHEELSON_FIRMWARE_RCDRIVER_H
#define WHEELSON_FIRMWARE_RCDRIVER_H

#include "../Autonomous/Driver.h"
#include "Connection.h"
#include <Loop/LoopManager.h>
#include <Display/Display.h>
#include <WiFiClient.h>
#include "RCDataCom.h"

#define FRAME_LENGTH 160*120

enum {
	FORWARD, BACKWARD, LEFT, RIGHT
};

class RCDriver : public LoopListener {
public:
	RCDriver(Connection* connection);
	void loop(uint) override;

	static void feedTaskFunc(Task* task);
	virtual ~RCDriver();

private:
	Connection* con = nullptr;
	WiFiClient& feedClient;
	WiFiClient& controlClient;

	Camera cam;
	Task feedTask;

	uint8_t command = 0;

	bool feedFunc();

	bool send(WiFiClient& client, const uint8_t* buffer, size_t size);

};


#endif //WHEELSON_FIRMWARE_RCDRIVER_H
