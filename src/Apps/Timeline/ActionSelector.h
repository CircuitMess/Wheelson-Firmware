#ifndef AUTOCAR_ACTIONSELECTOR_H
#define AUTOCAR_ACTIONSELECTOR_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Elements/GridMenu.h>
#include <UI/BitmapElement.h>
#include <Support/Modal.h>
#include <Input/InputListener.h>
#include "../SimpleProgramming/SimpleAction.h"

class SimpleEdit;

enum ActionItems {
	FORWARD, BACKWARD, LEFT, RIGHT, LIGHTOFF, LIGHTON
};

class ActionSelector : public Modal , private InputListener{
public:
	ActionSelector(Context& context);

	virtual ~ActionSelector();

	void draw();

	void start();

	void stop();

	void pack() override;

	void unpack() override;

private:
	static ActionSelector* instance;

	 GridLayout* gridLayout;
	std::vector<SimpleAction*> actions;

	uint selectedAction = 0;
	SimpleEdit* parent = nullptr;

	void selectApp(int8_t num);

	void buildUI();

	void buttonPressed(uint id) override;


};


#endif //AUTOCAR_ACTIONSELECTOR_H
