#ifndef AUTOCAR_PATTERNEDITOR_H
#define AUTOCAR_PATTERNEDITOR_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Support/Context.h>
#include <Elements/ListMenu.h>
#include "ActionSelector.h"
#include "../../Components/AutoAction.h"

class PatternEditor : public Context {
public:
	PatternEditor(Display& display);

	void draw() override;
	void start() override;
	void stop() override;

	void returned(void* data) override;

private:
	static PatternEditor* instance;

	ListMenu actionList;
	ActionSelector* actionSelector = nullptr;

	Vector<AutoAction> actions;

	int editingIndex = -1;

	void fillMenu();
	void buildUI();

	void redrawSelected();
};


#endif //AUTOCAR_PATTERNEDITOR_H
