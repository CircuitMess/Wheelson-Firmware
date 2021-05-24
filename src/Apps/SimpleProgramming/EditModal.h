#ifndef WHEELSON_FIRMWARE_EDITMODAL_H
#define WHEELSON_FIRMWARE_EDITMODAL_H

#include <Support/Modal.h>
#include <Input/InputListener.h>
#include <UI/LinearLayout.h>
#include "Action.hpp"
#include "Elements/EditModalItem.h"
#include "../../Elements/ModalBackground.hpp"

class SimpleEdit;

namespace Simple {
	class EditModal : public Modal, private InputListener {
	public:
		EditModal(Context& context, Action* action);

		virtual ~EditModal();

		void draw();

		void start();

		void stop();

	protected:

		void init() override;

		void deinit() override;

	private:
		static EditModal* instance;

		Action* action;

		ModalBackground modalBg;
		LinearLayout* layout;
		EditModalItem* speed;
		EditModalItem* time;

		void buildUI();

		void buttonPressed(uint id) override;


	};
}

#endif //WHEELSON_FIRMWARE_EDITMODAL_H
