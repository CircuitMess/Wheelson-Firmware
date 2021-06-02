#ifndef WHEELSON_LIBRARY_SHUTDOWNPOPUP_H
#define WHEELSON_LIBRARY_SHUTDOWNPOPUP_H

#include <Support/Modal.h>
#include <Loop/LoopListener.h>

class ShutdownPopup : public Modal, public LoopListener {
public:
	ShutdownPopup(Context &context);

	void draw() override;

	void start() override;

	void stop() override;

	void loop(uint) override;
private:
	uint shutdownTimer = 0;
	static const uint8_t shutdownTime;

};


#endif //WHEELSON_LIBRARY_SHUTDOWNPOPUP_H
