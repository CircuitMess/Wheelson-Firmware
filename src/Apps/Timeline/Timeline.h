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

private:
	static Timeline* instance;

	ListMenu patternMenu;
	PatternEditor* editor = nullptr;

	void fillMenu();
	void buildUI();
};


#endif //AUTOCAR_TIMELINE_H
