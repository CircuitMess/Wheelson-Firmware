#ifndef AUTOCAR_TIMELINE_H
#define AUTOCAR_TIMELINE_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Support/Context.h>
#include <Elements/ListMenu.h>
#include <Loop/LoopListener.h>
//#include <JPEGDecoder.h>
#include "ActionSelector.h"
#include "../../Components/AutoAction.h"

#include "../../Elements/Fleha.hpp"
#include "ActionEditor.h"

class Timeline : public Context, public LoopListener {
public:
	Timeline(Display& display);

	void draw() override;
	void start() override;
	void stop() override;

	void addAction(AutoAction::Type type);
	void returned(void* data) override;

	enum Modus { EDIT, PLAY } modus = EDIT;

	void initPattern(Vector<AutoAction>* actions, Modus modus = EDIT);

	void loop(uint micros) override;

private:
	static Timeline* instance;

	ActionEditor aEditor;

	Layout layers;
	ScrollLayout scroll;
	LinearLayout timelineList;
	Fleha fleha;

	ActionSelector selector;

	Vector<AutoAction>* actions = nullptr;
	uint selectedAction = 0;

	void fillMenu();
	void buildUI();
};


#endif //AUTOCAR_TIMELINE_H
