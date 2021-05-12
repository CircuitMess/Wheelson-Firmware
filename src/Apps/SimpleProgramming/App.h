#ifndef WHEELSON_FIRMWARE_APP_H
#define WHEELSON_FIRMWARE_APP_H

#include <Support/Context.h>
#include <UI/Screen.h>
#include <UI/LinearLayout.h>
#include <UI/ScrollLayout.h>
#include <Loop/LoopListener.h>
#include "Elements/ProgramList.h"

namespace Simple {
	class App : public Context, public LoopListener {
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

		static App* instance;

		ScrollLayout* scrollLayout;
		LinearLayout* list;
		std::vector<ProgramList*> programs;

		Color* backgroundBuffer = nullptr;
		Color* addBuffer = nullptr;

		void buildUI();

	};
}
#endif //WHEELSON_FIRMWARE_APP_H
