#ifndef WHEELSON_FIRMWARE_DRIVINGELEMENT_H
#define WHEELSON_FIRMWARE_DRIVINGELEMENT_H

#include <UI/CustomElement.h>

enum DrivingIcon{
 MOTOR
};
class DrivingElement : public CustomElement{
public:
	DrivingElement(ElementContainer* parent,DrivingIcon icon,String iconText, bool needPercentage);

	virtual ~DrivingElement();

	void draw();

	void setIconText(const String& iconText);

private:

	DrivingIcon icon;
	String iconText;
	Color* iconBuffer = nullptr;
	Color* percentageBuffer = nullptr;
	bool needPercentage = false;

	static const char* const Icons[1];
};

#endif //WHEELSON_FIRMWARE_DRIVINGELEMENT_H
