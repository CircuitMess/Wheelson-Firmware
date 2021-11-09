#ifndef WHEELSON_FIRMWARE_MAINMENUITEM_H
#define WHEELSON_FIRMWARE_MAINMENUITEM_H

#include <UI/CustomElement.h>

enum MenuApp {
	SIMPLE, AUTONOMOUS, BALL, OBJECT, RC, SETTINGS
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

	static const char* const AppIcons[6];
};


#endif //WHEELSON_FIRMWARE_MAINMENUITEM_H
