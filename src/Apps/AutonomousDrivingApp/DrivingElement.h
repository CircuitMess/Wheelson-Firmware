#ifndef WHEELSON_FIRMWARE_DRIVINGELEMENT_H
#define WHEELSON_FIRMWARE_DRIVINGELEMENT_H

#include <UI/CustomElement.h>

enum DrivingIcon{
 MOTOR
};
class DrivingElement : public CustomElement{
public:
	DrivingElement(ElementContainer* parent,DrivingIcon icon,String iconText);

	virtual ~DrivingElement();

	void draw();

private:

	DrivingIcon icon;
	String iconText;
	Color* iconBuffer = nullptr;

	static const char* const Icons[1];
};

#endif //WHEELSON_FIRMWARE_DRIVINGELEMENT_H
