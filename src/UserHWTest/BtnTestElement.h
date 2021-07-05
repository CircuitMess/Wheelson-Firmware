#ifndef WHEELSON_FIRMWARE_BTNTESTELEMENT_H
#define WHEELSON_FIRMWARE_BTNTESTELEMENT_H

#include <UI/CustomElement.h>

class BtnTestElement : public CustomElement {
public:
	BtnTestElement(ElementContainer *parent);

	void btnPress();

	void draw() override;

	bool isBtnPressed() const;

private:
	bool btnPressed=false;

};

#endif //WHEELSON_FIRMWARE_BTNTESTELEMENT_H
