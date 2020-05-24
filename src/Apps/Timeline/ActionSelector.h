#ifndef AUTOCAR_ACTIONSELECTOR_H
#define AUTOCAR_ACTIONSELECTOR_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Support/Context.h>
#include <Elements/GridMenu.h>

class ActionSelector : public Context {
public:
	ActionSelector(Display& display);

	void draw() override;
	void start() override;
	void stop() override;

	void unpack() override;

private:
	static ActionSelector* instance;

	GridMenu menu;

	Image driveImage;
	Image turnImage;
	Image lightImage;

	void fillMenu();
	void buildUI();

};


#endif //AUTOCAR_ACTIONSELECTOR_H
