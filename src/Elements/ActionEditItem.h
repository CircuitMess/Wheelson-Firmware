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
	const uint16_t* icon = nullptr;
	void* params = nullptr;

	Setting(Type type, const std::string& ps, const uint16_t* icon, void* params) : type(type), ps(ps), icon(icon),
																					params(params){ }
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
	ActionEditItem(ElementContainer* parent, const Setting* setting);

	void reflow() override;
	void reposChildren() override;

	void setSelected(bool selected);
	void trig();
	void trigAlt();

	void draw() override;

private:
	BitmapElement icon;
	TextElement text;

	const Setting* setting;
	bool selected = false;
	int value = 0;

	void setText();
};


#endif //AUTOCAR_ACTIONEDITITEM_H
