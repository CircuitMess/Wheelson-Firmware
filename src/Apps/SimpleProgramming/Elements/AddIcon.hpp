#ifndef WHEELSON_FIRMWARE_ADDICON_HPP
#define WHEELSON_FIRMWARE_ADDICON_HPP

#include <UI/CustomElement.h>

namespace Simple {
	class AddIcon : public CustomElement {
	public:
		AddIcon(ElementContainer* parent);

		virtual ~AddIcon();

		void draw();

		void setSelected(bool selected);

	private:

		bool selected = false;
		String name;
		Color* addBuffer = nullptr;
		Color* borderBuffer = nullptr;
	};
}

#endif //WHEELSON_FIRMWARE_ADDICON_HPP
