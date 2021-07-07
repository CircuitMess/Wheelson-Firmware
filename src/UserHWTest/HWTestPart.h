#ifndef WHEELSON_FIRMWARE_HWTESTPART_H
#define WHEELSON_FIRMWARE_HWTESTPART_H

#include <Display/Display.h>
#include "UserHWTest.h"

class HWTestPart : public InputListener{
public:
	HWTestPart(UserHWTest* userHwTest);

	virtual ~HWTestPart();

	virtual void draw();

	virtual void start() = 0;
	virtual void stop() = 0;

private:


protected:
	UserHWTest* userHwTest;
};

#endif //WHEELSON_FIRMWARE_HWTESTPART_H
