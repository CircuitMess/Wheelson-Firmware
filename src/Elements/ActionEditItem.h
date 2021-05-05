#ifndef AUTOCAR_ACTIONEDITITEM_H
#define AUTOCAR_ACTIONEDITITEM_H

#include <Arduino.h>
#include <Util/Vector.h>
#include <string>
#include <UI/TextElement.h>
#include <UI/BitmapElement.h>
#include <UI/LinearLayout.h>

struct Setting {
	enum Type { NUMERIC, OPTION, BOOLEAN } type;
	std::string ps;
	enum Icon { TIME, COLOR, FREQ, VOLUME } iconType;
	void* params = nullptr;
	size_t offset;

	Setting(Type type, const std::string& ps, Icon iconType, void* params, size_t offset = 0) :
			type(type), ps(ps), iconType(iconType), params(params), offset(offset){ }
};

struct SettingNumeric {
	SettingNumeric(int min, int max, int step) : min(min), max(max), step(step){ }

	int min;
	int max;
	int step;
};

struct SettingOption {
	SettingOption(const std::initializer_list<std::string>& list) : options(list){}

	Vector<std::string> options;
};

class ActionEditItem : public LinearLayout {
public:
	ActionEditItem(ElementContainer* parent, const Setting* setting, void* valptr);

	virtual ~ActionEditItem();

	void reflow() override;
	void reposChildren() override;

	void setSelected(bool selected);
	void trig();
	void trigAlt();

	void draw() override;

private:
	//BitmapElement icon;
	TextElement text;

	const Setting* setting;
	bool selected = false;
	int* value = nullptr;

	fs::File iconFile;
	fs::File borderFile;
	Color* buffer = nullptr;
	Color* borderBuffer = nullptr;
	static const char* const SettingsSprites[4];

	void setText();
};




#endif //AUTOCAR_ACTIONEDITITEM_H
