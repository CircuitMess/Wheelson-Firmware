#ifndef AUTOCAR_TIMELINE_H
#define AUTOCAR_TIMELINE_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Support/Context.h>
#include <Elements/ListMenu.h>
#include "PatternEditor.h"

class Timeline : public Context {
public:
	Timeline(Display& display);

	void draw() override;
	void start() override;
	void stop() override;

	void unpack() override;

private:
	static Timeline* instance;

	ListMenu menu;
	PatternEditor* editor = nullptr;

	Vector<Vector<AutoAction>> patterns;

	void fillMenu();
	void buildUI();
};


#endif //AUTOCAR_TIMELINE_H
