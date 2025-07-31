#ifndef AUTOCAR_ACTIONSELECTOR_H
#define AUTOCAR_ACTIONSELECTOR_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Elements/GridMenu.h>
#include <UI/BitmapElement.h>
#include <Support/Modal.h>
#include <Input/InputListener.h>
#include "Elements/ActionElement.h"
#include <Elements/ModalBackground.hpp>

class SimpleEdit;

namespace Simple {
	class ActionSelector : public Modal, private InputListener {
	public:
		ActionSelector(Context& context);

		~ActionSelector() override = default;

		void draw();

		void start();

		void stop();

	protected:

		void init() override;

		void deinit() override;

	private:
		static ActionSelector* instance;

		GridLayout* gridLayout;
		std::vector<ActionElement*> actionElements;

		ModalBackground modalBg;

		uint8_t selection = 0;
		SimpleEdit* parent = nullptr;

		void selectApp(int8_t num);

		void buildUI();

		void buttonPressed(uint id) override;

		uint8_t actionsNum;

		/**
		 * Number of elements in last (possibly uneven row)
		 */
		uint8_t lastRowElements;

		static constexpr uint8_t Cols = 3;

		static const char* const ActionTitles[7];
	};
}

#endif //AUTOCAR_ACTIONSELECTOR_H
