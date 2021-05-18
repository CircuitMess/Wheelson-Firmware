#include "DescreteSlider.h"


SettingsScreen::DescreteSlider::DescreteSlider(ElementContainer* parent, String name, std::vector<uint8_t> shutDownTime) : SettingsElement(parent, name), shutDownTime(shutDownTime){

}

void SettingsScreen::DescreteSlider::toggle(){
	sliderIsSelected = !sliderIsSelected;
}

void SettingsScreen::DescreteSlider::selectNext(){
	Serial.println("SelectNext");
	if(shutDownTime.empty()) return;
	index += 1;
	index = min(index, (int) shutDownTime.size() - 1);
}

void SettingsScreen::DescreteSlider::selectPrev(){
	Serial.println("SelectPrev");
	if(shutDownTime.empty()) return;
	index -= 1;
	index = max(index, 0);

}

void SettingsScreen::DescreteSlider::drawControl(){
	long movingCursor;

	movingCursor = map(index, 0, shutDownTime.size()-1, 0, 51);

	getSprite()->setTextColor(TFT_WHITE);
	getSprite()->setCursor(getTotalX() + 125, getTotalY() + 8);
	getSprite()->println(shutDownTime[index]);
	getSprite()->setCursor(getTotalX() + 137, getTotalY() + 8);
	getSprite()->println("m");

	getSprite()->drawRect(getTotalX() + 100, getTotalY() + 16, 2, 5, TFT_WHITE);
	getSprite()->drawRect(getTotalX() + 153, getTotalY() + 16, 2, 5, TFT_WHITE);
	getSprite()->drawRect(getTotalX() + 100, getTotalY() + 18, 55, 1, TFT_WHITE);
	if(sliderIsSelected){
		getSprite()->fillRoundRect(getTotalX() + 100 + movingCursor, getTotalY() + 14, 4, 9, 1, TFT_RED);
	}else{
		getSprite()->fillRoundRect(getTotalX() + 100 + movingCursor, getTotalY() + 15, 4, 7, 1, TFT_WHITE);
	}

}

bool SettingsScreen::DescreteSlider::isSliderSelected() const{
	return sliderIsSelected;
}


