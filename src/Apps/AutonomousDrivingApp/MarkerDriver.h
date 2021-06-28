#ifndef WHEELSON_FIRMWARE_MARKERDRIVER_H
#define WHEELSON_FIRMWARE_MARKERDRIVER_H


#include "../../Components/Driver.h"

class MarkerDriver : public Driver {
protected:
	void process() override;

private:
	enum {
		IDLE,
		FORWARD,
		BACKWARD,
		BURNOUT,
		DO360,
		LEDON,
		LEDOFF
	} state;
};


#endif //WHEELSON_FIRMWARE_MARKERDRIVER_H
