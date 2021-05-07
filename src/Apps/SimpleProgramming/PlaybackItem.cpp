#include "PlaybackItem.h"
#include <U8g2_for_TFT_eSPI.h>

const char* const PlaybackItem::PlaybackIcons[] = {"/Simple/arrow_up.raw", "/Simple/arrow_down.raw", "/Simple/arrow_left.raw", "/Simple/arrow_right.raw", "/Simple/light_off.raw", "/Simple/light_on.raw"};

PlaybackItem::PlaybackItem(ElementContainer* parent, PlaybackItems item, String name) : CustomElement(parent, 100, 18), item(item), name(name){

	itemBuffer = static_cast<Color*>(ps_malloc(18 * 18 * 2));
	if(itemBuffer == nullptr){
		Serial.printf("PlaybackItem picture %s unpack error\n", PlaybackIcons[item]);
		return;
	}

	fs::File itemFile = SPIFFS.open(PlaybackIcons[item]);
	itemFile.read(reinterpret_cast<uint8_t*>(itemBuffer), 18 * 18 * 2);
	itemFile.close();
}

PlaybackItem::~PlaybackItem(){
	free(itemBuffer);
}

void PlaybackItem::draw(){
	getSprite()->drawIcon(itemBuffer, getTotalX() + 35, getTotalY(), 18, 18, 1, TFT_TRANSPARENT);

	FontWriter u8f = getSprite()->startU8g2Fonts();
	u8f.setFont(u8g2_font_HelvetiPixel_tr);
	u8f.setForegroundColor(TFT_WHITE);
	u8f.setFontMode(1);
	u8f.setCursor(getTotalX() + 55, getTotalY() + 13);
	u8f.print(name);
	if(selected){
		getSprite()->fillTriangle(getTotalX() + 26, getTotalY() + 15, getTotalX() + 32,
								  getTotalY() + 9,
								  getTotalX() + 26, getTotalY() + 3, TFT_GREEN);
		getSprite()->drawTriangle(getTotalX() + 26, getTotalY() + 15, getTotalX() + 32,
								  getTotalY() + 9,
								  getTotalX() + 26, getTotalY() + 3, TFT_BLACK);
	}
}

void PlaybackItem::setIsSelected(bool selected){
	PlaybackItem::selected = selected;
}
