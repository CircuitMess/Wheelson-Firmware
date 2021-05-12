#ifndef WHEELSON_FIRMWARE_PLAYBACK_H
#define WHEELSON_FIRMWARE_PLAYBACK_H

#include <Display/Display.h>
#include <Support/Context.h>
#include <Loop/LoopListener.h>
#include <UI/LinearLayout.h>
#include <UI/ScrollLayout.h>
#include "Elements/ActionElement.h"

namespace Simple {
	class Playback : public Context, public LoopListener {
	public:
		Playback(Display& display);

		virtual ~Playback();

		void start();

		void draw();

		void stop();

		void loop(uint micros) override;

	protected:
		void init() override;

		void deinit() override;

	private:

		static Playback* instance;

		ScrollLayout* scrollLayout;
		LinearLayout* layout;
		std::vector<ActionElement*> item;

		int8_t itemNum = 0;

		Color* backgroundBuffer = nullptr;

		void buildUI();

	};
}
#endif //WHEELSON_FIRMWARE_PLAYBACK_H
