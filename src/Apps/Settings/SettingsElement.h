#ifndef WHEELSON_FIRMWARE_SETTINGSELEMENT_HPP
#define WHEELSON_FIRMWARE_SETTINGSELEMENT_HPP

#include <UI/CustomElement.h>

namespace SettingsScreen {
	class SettingsElement : public CustomElement {
	public:

		SettingsElement(ElementContainer* parent, String name);

		void setIsSelected(bool isSelected);

		void draw();

		virtual void toggle() = 0;

		bool isSelected() const;

	private:
		bool selected = false;
		String name;

	protected:
		virtual void drawControl() = 0;
	};
}

#endif //WHEELSON_FIRMWARE_SETTINGSELEMENT_HPP
