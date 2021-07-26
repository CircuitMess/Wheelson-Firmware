#ifndef WHEELSON_FIRMWARE_PROGRAMELEMENT_H
#define WHEELSON_FIRMWARE_PROGRAMELEMENT_H

#include <UI/CustomElement.h>

namespace Simple {
	class ProgramElement : public CustomElement {
	public:
		ProgramElement(ElementContainer* parent, String name);

		virtual ~ProgramElement();

		void draw();

		void setIsSelected(bool isSelected);

		void touchStart(Color color);
		void touchEnd();


	private:

		bool selected = false;
		String name;

		Color touchColor;
		uint32_t touchStartTime = 0;
	};
}


#endif //WHEELSON_FIRMWARE_PROGRAMELEMENT_H
