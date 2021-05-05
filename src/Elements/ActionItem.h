#ifndef AUTOCAR_ACTIONITEM_H
#define AUTOCAR_ACTIONITEM_H


#include <UI/Layout.h>
#include <UI/BitmapElement.h>
#include <UI/TextElement.h>
#include <UI/LinearLayout.h>

class ActionItem : public LinearLayout {
public:
	ActionItem(ElementContainer* parent, const uint16_t* icon, const std::string& text);

	virtual ~ActionItem();

	void reflow() override;
	void reposChildren() override;

	void setSelected(bool selected);

	void draw() override;

private:
	BitmapElement icon;
	TextElement text;

	fs::File borderFile;
	Color* borderBuffer = nullptr;
	bool selected = false;

};


#endif //AUTOCAR_ACTIONITEM_H
