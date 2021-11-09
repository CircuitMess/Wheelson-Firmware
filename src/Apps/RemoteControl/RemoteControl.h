#ifndef WHEELSON_FIRMWARE_REMOTECONTROL_H
#define WHEELSON_FIRMWARE_REMOTECONTROL_H


#include <Support/Context.h>
#include <Input/InputListener.h>
#include "Connection.h"
#include "RCDriver.h"

class RemoteControl : public Context, public ConnectionListener, public InputListener {
public:
	RemoteControl(Display &display);
	virtual ~RemoteControl();

	void draw() override;
	void start() override;
	void stop() override;

	void connected() override;
	void disconnected() override;

protected:
	void init() override;
	void deinit() override;

private:
	Color* icon = nullptr;
	Color* crossedIcon = nullptr;
	Color* background = nullptr;

	Connection con;
	RCDriver* driver = nullptr;

	void buttonPressed(uint i) override;
};


#endif //WHEELSON_FIRMWARE_REMOTECONTROL_H
