#ifndef AUTOCAR_PATTERNEDITOR_H
#define AUTOCAR_PATTERNEDITOR_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Support/Context.h>
#include <Elements/ListMenu.h>
#include "ActionSelector.h"
#include "../../Components/AutoAction.h"

#include "../../Elements/Fleha.hpp"
#include "ActionEditor.h"

class PatternEditor : public Context {
public:
	PatternEditor(Display& display);

	void draw() override;
	void start() override;
	void stop() override;

	void addAction(AutoAction::Type type);
	void returned(void* data) override;

private:
	static PatternEditor* instance;

	ActionEditor aEditor;

	Layout layers;
	ScrollLayout scroll;
	LinearLayout timelineList;
	Fleha fleha;

	ActionSelector selector;

	Vector<AutoAction> actions;
	uint selectedAction = 0;

	void buildUI();
};


#endif //AUTOCAR_PATTERNEDITOR_H
