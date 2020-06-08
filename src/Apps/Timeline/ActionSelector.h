#ifndef AUTOCAR_ACTIONSELECTOR_H
#define AUTOCAR_ACTIONSELECTOR_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Elements/GridMenu.h>
#include <UI/BitmapElement.h>
#include <Support/Modal.h>
#include "../../Elements/Fleha.hpp"

class Timeline;

class ActionSelector : public Modal {
public:
	ActionSelector(Timeline* timeline);

	void draw();
	void start();
	void stop();

	void unpack() override;

private:
	static ActionSelector* instance;

	Timeline* timeline = nullptr;

	Layout layers;
	Fleha fleha;
	GridLayout actionGrid;
	BitmapElement selectedBorder;

	uint selectedAction = 0;
	void selectAction();

	void fillMenu();
	void buildUI();

};


#endif //AUTOCAR_ACTIONSELECTOR_H
