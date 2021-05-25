#ifndef WHEELSON_FIRMWARE_EDIT_H
#define WHEELSON_FIRMWARE_EDIT_H

#include <Support/Context.h>
#include <UI/Screen.h>
#include <UI/LinearLayout.h>
#include <Loop/LoopListener.h>
#include <UI/ScrollLayout.h>
#include <UI/GridLayout.h>
#include <Input/InputListener.h>
#include "Elements/ActionElement.h"
#include "Storage.h"

namespace Simple {
	class Edit : public Context, public LoopListener, private InputListener {
	public:

		Edit(Display& display, Storage *storage,int16_t programIndex);

		virtual ~Edit();

		void start();

		void stop();

		void draw();

		void loop(uint micros) override;

		void returned(void* data) override;

	protected:

		void init() override;
		void deinit() override;

	private:

		static Edit* instance;

		ScrollLayout* scrollLayout;
		GridLayout* list;
		std::vector<Action> actions;
		Storage* storage;
		int16_t programIndex;

		Color* backgroundBuffer = nullptr;
		uint8_t actionNum = 0;

		void buttonPressed(uint id) override;

		void selectAction(uint8_t num);

		void buildUI();

	};
}
#endif //WHEELSON_FIRMWARE_EDIT_H
