#ifndef WHEELSON_LIBRARY_WARNINGPOPUP_H
#define WHEELSON_LIBRARY_WARNINGPOPUP_H

#include <Support/Modal.h>
#include <Loop/LoopListener.h>

class WarningPopup : public Modal, public LoopListener {
public:
	WarningPopup(Context &context);

	void draw() override;

	void start() override;

	void stop() override;

	void loop(uint) override;
private:
	uint warningTimer = 0;
	static const uint8_t warningTime;
};


#endif //WHEELSON_LIBRARY_WARNINGPOPUP_H
