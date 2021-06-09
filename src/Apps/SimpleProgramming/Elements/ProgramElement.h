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

	private:

		bool selected = false;
		String name;
	};
}


#endif //WHEELSON_FIRMWARE_PROGRAMELEMENT_H
