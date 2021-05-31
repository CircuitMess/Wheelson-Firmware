#ifndef WHEELSON_FIRMWARE_EDITMODALITEM_H
#define WHEELSON_FIRMWARE_EDITMODALITEM_H

#include <UI/CustomElement.h>

namespace Simple {
	class EditModalItem : public CustomElement {
	public:
		EditModalItem(ElementContainer* parent, String text, char unit, float value, float step);

		virtual ~EditModalItem();

		void draw();

		void setSelected(bool selected);

		bool isSelected() const;

		float getValue() const;

		void up();

		void down();

	private:
		String text;
		char unit;
		float value = 0;
		float step = 0;
		bool selected = false;
	};
}

#endif //WHEELSON_FIRMWARE_EDITMODALITEM_H
