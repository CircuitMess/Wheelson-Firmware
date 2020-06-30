#ifndef AUTOCAR_TIMELINEAPP_H
#define AUTOCAR_TIMELINEAPP_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Support/Context.h>
#include <Elements/ListMenu.h>
#include "Timeline.h"

class TimelineApp : public Context, public UpdateListener {
public:
	TimelineApp(Display& display);

	void draw() override;
	void start() override;
	void stop() override;

	void unpack() override;

	void update(uint micros) override;

	float filling = -1;
	ListMenu menu;

private:
	static TimelineApp* instance;

	// ListMenu menu;
	Timeline* timeline = nullptr;

	Vector<Vector<AutoAction>> patterns;

	void play(uint index);

	// float filling = -1;

	void fillMenu();
	void buildUI();
};


#endif //AUTOCAR_TIMELINEAPP_H
