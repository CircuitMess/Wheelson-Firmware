#ifndef WHEELSON_FIRMWARE_PLAYBACK_H
#define WHEELSON_FIRMWARE_PLAYBACK_H

#include <Display/Display.h>
#include <Support/Context.h>
#include <Loop/LoopListener.h>
#include <UI/LinearLayout.h>
#include <UI/ScrollLayout.h>
#include <Input/InputListener.h>
#include "Elements/ActionElement.h"
#include "ProgPlayer.h"
#include <Input/Input.h>

namespace Simple {
	class Playback : public Context, public LoopListener, private InputListener {
	public:
		Playback(Display& display,Action* action,uint8_t numActions);

		virtual ~Playback();

		void start();

		void draw();

		void stop();

		void loop(uint micros) override;

	protected:

		void init() override;

		void deinit() override;

	private:
		Action* action;
		ProgPlayer newPlayer;
		uint8_t numActions = 0;

		static Playback* instance;

		ScrollLayout* scrollLayout;
		LinearLayout* layout;
		std::vector<ActionElement*> item;

		uint8_t itemNum = 0;
		Color* backgroundBuffer = nullptr;

		void buildUI();

		void buttonPressed(uint id) override;

		void selectAction(uint8_t num);

	};
}
#endif //WHEELSON_FIRMWARE_PLAYBACK_H
