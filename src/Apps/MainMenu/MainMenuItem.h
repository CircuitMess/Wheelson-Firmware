#ifndef WHEELSON_FIRMWARE_MAINMENUITEM_H
#define WHEELSON_FIRMWARE_MAINMENUITEM_H

#include <UI/CustomElement.h>

enum MenuApp {
	AUTONOMUS, ACTION, KUGLICA, QR, SETTINGS
};

class MainMenuItem : public CustomElement {
public:

	MainMenuItem(ElementContainer* parent, MenuApp app);

	virtual ~MainMenuItem();

	void draw();

	void isSelected(bool selected);

private:

	MenuApp app;
	bool selected = false;
	Color* bgBuffer = nullptr;

	static const char* const appIcons[5];
};


#endif //WHEELSON_FIRMWARE_MAINMENUITEM_H
