#ifndef WHEELSON_FIRMWARE_MAINMENUAPP_H
#define WHEELSON_FIRMWARE_MAINMENUAPP_H

#include <UI/CustomElement.h>

enum MenuApp {
	AUTONOMUS, ACTION, KUGLICA, QR, SETTINGS
};

class MainMenuApp : public CustomElement {
public:

	MainMenuApp(ElementContainer* parent, MenuApp app);

	virtual ~MainMenuApp();

	void draw();

	void isSelected(bool selected);

private:

	MenuApp app;
	bool selected = false;
	Color* bgBuffer = nullptr;

	static const char* const appIcons[5];
};


#endif //WHEELSON_FIRMWARE_MAINMENUAPP_H
