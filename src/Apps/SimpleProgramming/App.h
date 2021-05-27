#ifndef WHEELSON_FIRMWARE_APP_H
#define WHEELSON_FIRMWARE_APP_H

#include <Support/Context.h>
#include <UI/Screen.h>
#include <UI/LinearLayout.h>
#include <UI/ScrollLayout.h>
#include <Loop/LoopListener.h>
#include <Input/InputListener.h>
#include "Elements/ProgramElement.h"
#include "Elements/AddIcon.hpp"
#include "Storage.h"

namespace Simple {
	class App : public Context, public LoopListener, public InputListener {
	public:

		App(Display& display);

		virtual ~App();

		void start();

		void stop();

		void draw();

		void loop(uint micros) override;

	protected:

		void init() override;

		void deinit() override;

	private:
		Storage storage;

		ScrollLayout* scrollLayout;
		LinearLayout* list;
		std::vector<ProgramElement*> programs;
		AddIcon* addIcon;
		Color* backgroundBuffer = nullptr;
		uint8_t programNum = 0;
		int seconds = 0;
		uint16_t previousTime = 0;
		uint16_t currentTime = 0;

		void loadProgs();

		void buildUI();

		void buttonPressed(uint id) override;

		void buttonReleased(uint id) override;

		void selectAction(uint8_t num);
	};
}
#endif //WHEELSON_FIRMWARE_APP_H
