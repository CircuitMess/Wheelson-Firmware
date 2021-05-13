#ifndef WHEELSON_FIRMWARE_EDITMODAL_HPP
#define WHEELSON_FIRMWARE_EDITMODAL_HPP

#include <Support/Modal.h>
#include <Input/InputListener.h>
#include <UI/LinearLayout.h>
#include "Action.hpp"
#include "Elements/EditModalItems.hpp"

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

		LinearLayout* layout;
		EditModalItems* speed;
		EditModalItems* time;

		void buildUI();

		void buttonPressed(uint id) override;


	};
}

#endif //WHEELSON_FIRMWARE_EDITMODAL_HPP
