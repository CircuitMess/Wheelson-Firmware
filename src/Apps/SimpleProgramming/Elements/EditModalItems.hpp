#ifndef WHEELSON_FIRMWARE_EDITMODALITEMS_HPP
#define WHEELSON_FIRMWARE_EDITMODALITEMS_HPP

#include <UI/CustomElement.h>

namespace Simple {
	class EditModalItems : public CustomElement {
	public:
		EditModalItems(ElementContainer* parent, String text, char unit, float value, float step);

		virtual ~EditModalItems();

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

#endif //WHEELSON_FIRMWARE_EDITMODALITEMS_HPP
