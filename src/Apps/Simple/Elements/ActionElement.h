#ifndef WHEELSON_FIRMWARE_ACTIONELEMENT_H
#define WHEELSON_FIRMWARE_ACTIONELEMENT_H

#include <UI/CustomElement.h>
#include "../Action.hpp"

namespace Simple {
	class ActionElement : public CustomElement {
	public:
		ActionElement(ElementContainer* parent, Action::Type action, String text = "");

		virtual ~ActionElement();

		void draw();

		void setIsSelected(bool selected);

		Action::Type getAction() const;

	private:
		const Action::Type action;
		bool selected = false;
		Color* iconActionBuffer = nullptr;
		Color* borderBuffer = nullptr;
		String text;
		static const char* const ActionIcons[8];
	};
}
#endif //WHEELSON_FIRMWARE_ACTIONELEMENT_H
