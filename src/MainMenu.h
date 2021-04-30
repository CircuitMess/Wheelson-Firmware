#ifndef AUTOCAR_MAINMENU_H
#define AUTOCAR_MAINMENU_H

#include <Arduino.h>
#include <Support/Context.h>
#include <Elements/GridMenu.h>
#include "Apps/AutonomousDriving/autonomousApp.h"

struct LauncherItem {
	std::string title;
	Context* context;
	Element* image;
};

class MainMenu : public Context {
public:
	MainMenu(Display& display);

	virtual ~MainMenu();

	void start() override;
	void stop() override;
	void unpack() override;
	void draw() override;

private:
	static MainMenu* instance;

	GridMenu appMenu;
	std::vector<LauncherItem> menuItems;

	void fillMenu();
	void buildUI();
	fs::File iconFile[5];
	Color *buffer[5]={nullptr};

	static const char* const icons[5];

};


#endif //AUTOCAR_MAINMENU_H
