#ifndef WHEELSON_FIRMWARE_DRIVINGELEMENT_H
#define WHEELSON_FIRMWARE_DRIVINGELEMENT_H

#include <UI/CustomElement.h>

enum DrivingIcon{
 MOTOR
};
class DrivingElement : public CustomElement{
public:
	DrivingElement(ElementContainer* parent, DrivingIcon icon, String text, bool needPercentage);

	virtual ~DrivingElement();

	void draw();

	void setText(const String& text);

private:

	DrivingIcon icon;
	String text;
	Color* iconBuffer = nullptr;
	Color* percentageBuffer = nullptr;
	bool needPercentage = false;

	static const char* const Icons[1];
};

#endif //WHEELSON_FIRMWARE_DRIVINGELEMENT_H
