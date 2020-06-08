#ifndef AUTOCAR_ACTIONEDITOR_H
#define AUTOCAR_ACTIONEDITOR_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Support/Modal.h>
#include <Elements/GridMenu.h>
#include "../../Elements/Fleha.hpp"
#include "../../Components/AutoAction.h"

class Timeline;

class ActionEditor : public Modal {
public:
	explicit ActionEditor(Timeline* timeline);

	void draw() override;
	void start() override;
	void stop() override;

	void initAction(AutoAction::Type type);

	void unpack() override;

private:
	static ActionEditor* instance;

	Timeline* timeline = nullptr;

	Layout layers;
	Fleha fleha;
	ScrollLayout scroll;
	LinearLayout list;

	uint selectedSetting = 0;
	void selectSetting(uint i);

	void fillMenu();
	void buildUI();
};


#endif //AUTOCAR_ACTIONEDITOR_H
