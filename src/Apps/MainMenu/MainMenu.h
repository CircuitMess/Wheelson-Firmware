#ifndef AUTOCAR_MAINMENU_H
#define AUTOCAR_MAINMENU_H

#include <Arduino.h>
#include <Support/Context.h>
#include <UI/Layout.h>
#include <Elements/GridMenu.h>
#include "../AutonomousDriving/autonomousApp.h"
#include "MainMenuItem.h"


class MainMenu : public Context {
public:
	MainMenu(Display& display);

	virtual ~MainMenu();

	void start() override;

	void stop() override;

	void unpack() override;

	void pack() override;

	void draw() override;

private:
	static MainMenu* instance;

	Layout layout;
	std::vector<MainMenuItem*> apps;

	int8_t appNum = 1;
	Color* backgroundBuffer = nullptr;

	void buildUI();

	static const char* const AppTitles[5];

};


#endif //AUTOCAR_MAINMENU_H
