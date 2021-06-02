#include "BtnTestElement.h"

BtnTestElement::BtnTestElement(ElementContainer* parent) : CustomElement(parent, 20, 20){

}

void BtnTestElement::btnPress(){
	if(!btnPressed){
		btnPressed = true;

	}
}

void BtnTestElement::draw(){
	getSprite()->drawRect(getTotalX(), getTotalY(), 21, 21, !btnPressed ? TFT_DARKGREY : TFT_GREEN);
	getSprite()->fillCircle(getTotalX() + 10, getTotalY() + 10, 5, !btnPressed ? TFT_DARKGREY : TFT_GREEN);

}

bool BtnTestElement::isBtnPressed() const{
	return btnPressed;
}
