#ifndef WHEELSON_FIRMWARE_SIMPLEACTION_H
#define WHEELSON_FIRMWARE_SIMPLEACTION_H

#include <UI/CustomElement.h>

enum Action {
	ARROW_UP, ARROW_DOWN, ARROW_LEFT, ARROW_RIGHT, LIGHT_OFF, LIGHT_ON, ADD
};

class SimpleAction : public CustomElement {
public:
	SimpleAction(ElementContainer* parent, Action action);

	virtual ~SimpleAction();

	void draw();

	void setIsSelected(bool selected);

private:
	Action action;
	bool selected = false;
	Color* iconActionBuffer = nullptr;
	Color* borderBuffer = nullptr;

	static const char* const AcionIcons[7];
};

#endif //WHEELSON_FIRMWARE_SIMPLEACTION_H
