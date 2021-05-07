#ifndef WHEELSON_FIRMWARE_PLAYBACKITEM_H
#define WHEELSON_FIRMWARE_PLAYBACKITEM_H

#include <UI/CustomElement.h>

enum PlaybackItems {
	FORWARD, BACKWARD, LEFT, RIGHT, LIGHTOFF, LIGHTON
};

class PlaybackItem : public CustomElement {
public:
	PlaybackItem(ElementContainer* parent,PlaybackItems item, String name);

	virtual ~PlaybackItem();

	void draw();

	void setIsSelected(bool isSelected);

private:
	bool selected = false;
	String name;
	PlaybackItems item;
	Color* itemBuffer = nullptr;

	static const char* const PlaybackIcons[6];
};


#endif //WHEELSON_FIRMWARE_PLAYBACKITEM_H
