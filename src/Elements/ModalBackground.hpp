#ifndef AUTOCAR_FLEHA_H
#define AUTOCAR_FLEHA_H

#include <UI/CustomElement.h>

class ModalBackground : public CustomElement{
public:
	ModalBackground(ElementContainer* parent, uint width, uint height) : CustomElement(parent, width, height){
		getSprite()->setChroma(TFT_TRANSPARENT);
	}

	void draw() override {
		Sprite* canvas = getSprite();

		canvas->clear(TFT_TRANSPARENT);
		canvas->fillRect(getTotalX(), getTotalY() + 2, getWidth(), getHeight() - 4, bgColor);
		canvas->drawFastHLine( getTotalX() + 1, getTotalY(), getWidth() - 2, borderTopColor);
		canvas->drawFastHLine(getTotalX(), getTotalY() + 1, getWidth(), borderTopColor);
		canvas->drawFastHLine(getTotalX(), getTotalY() + getHeight() - 2, getWidth(), borderBotColor);
		canvas->drawFastHLine(getTotalX() + 1, getTotalY() + getHeight() - 1, getWidth() - 2, borderBotColor);

		if(border){
			canvas->drawFastVLine(getTotalX() + 0, getTotalY() + 2, getHeight() - 4, borderTopColor);
			canvas->drawFastVLine(getTotalX() + 1, getTotalY() + 2, getHeight() - 4, borderTopColor);
			canvas->drawFastVLine(getTotalX() + getWidth() - 2, getTotalY() + 2, getHeight() - 4, borderTopColor);
			canvas->drawFastVLine(getTotalX() + getWidth() - 1, getTotalY() + 2, getHeight() - 4, borderTopColor);
		}
	}

	bool border = false;

	Color bgColor = C_HEX(0x00beff);
	Color borderTopColor = C_HEX(0x00ffff);
	Color borderBotColor = C_HEX(0x0082ff);

};

#endif //AUTOCAR_FLEHA_H
