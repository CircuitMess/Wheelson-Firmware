#ifndef AUTOCAR_PATTERNEDITOR_H
#define AUTOCAR_PATTERNEDITOR_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Support/Context.h>
#include <Elements/ListMenu.h>
#include "ActionSelector.h"
#include "../../Components/AutoAction.h"

#include "../../Elements/Fleha.hpp"

class PatternEditor : public Context {
public:
	PatternEditor(Display& display);

	void draw() override;
	void start() override;
	void stop() override;

	void addAction(AutoAction::Type type);

private:
	static PatternEditor* instance;

	Fleha fleha;

	Vector<AutoAction> actions;
	uint selectedAction = 0;

	void drawTimeline();
	void buildUI();
};


#endif //AUTOCAR_PATTERNEDITOR_H
