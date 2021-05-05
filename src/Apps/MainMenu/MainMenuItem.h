#ifndef WHEELSON_FIRMWARE_MAINMENUITEM_H
#define WHEELSON_FIRMWARE_MAINMENUITEM_H

#include <UI/CustomElement.h>

enum MenuApp {
	AUTONOMOUS, SIMPLE, BALL, OBJECT, SETTINGS
};

class MainMenuItem : public CustomElement {
public:

	MainMenuItem(ElementContainer* parent, MenuApp app);

	virtual ~MainMenuItem();

	void draw();

	void setSelected(bool selected);

	bool isSelected() const;

private:

	MenuApp app;
	bool selected = false;
	Color* appIconBuffer = nullptr;
	Color* borderBuffer = nullptr;

	static const char* const AppIcons[5];
};


#endif //WHEELSON_FIRMWARE_MAINMENUITEM_H
