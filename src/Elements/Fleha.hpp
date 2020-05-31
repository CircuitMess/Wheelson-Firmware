#ifndef AUTOCAR_FLEHA_H
#define AUTOCAR_FLEHA_H

#include <UI/CustomElement.h>

class Fleha : public CustomElement {
public:
	Fleha(ElementContainer* parent, uint width, uint height) : CustomElement(parent, width, height){ }

	void draw() override {
		Sprite* canvas = getSprite();

		canvas->fillRect(getX(), getY() + 2, getWidth(), getHeight() - 4, bgColor);
		canvas->drawFastHLine( getX() + 1, getY(), getWidth() - 2, borderTopColor);
		canvas->drawFastHLine(getX(), getY() + 1, getWidth(), borderTopColor);
		canvas->drawFastHLine(getX(), getY() + getHeight() - 2, getWidth(), borderBotColor);
		canvas->drawFastHLine(getX() + 1, getY() + getHeight() - 1, getWidth() - 2, borderBotColor);
	}

private:
	Color bgColor = C_HEX(0x00beff);
	Color borderTopColor = C_HEX(0x00ffff);
	Color borderBotColor = C_HEX(0x0082ff);

};

#endif //AUTOCAR_FLEHA_H
