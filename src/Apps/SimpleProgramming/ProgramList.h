#ifndef WHEELSON_FIRMWARE_PROGRAMLIST_H
#define WHEELSON_FIRMWARE_PROGRAMLIST_H

#include <UI/CustomElement.h>


	class ProgramList : public CustomElement {
	public:
		ProgramList(ElementContainer* parent, String name);

		virtual ~ProgramList();

		void draw();

		void setIsSelected(bool isSelected);

	private:

		bool selected = false;
		String name;
	};



#endif //WHEELSON_FIRMWARE_PROGRAMLIST_H
