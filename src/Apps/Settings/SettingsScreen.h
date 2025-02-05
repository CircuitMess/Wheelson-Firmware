#ifndef WHEELSON_FIRMWARE_SETTINGSSCREEN_H
#define WHEELSON_FIRMWARE_SETTINGSSCREEN_H

#include <Support/Context.h>
#include <UI/LinearLayout.h>
#include "SliderElement.h"
#include "DiscreteSlider.h"
#include "BooleanElement.h"
#include <Input/InputListener.h>
#include "TextElement.h"
#include <FS.h>

namespace SettingsScreen {
	class SettingsScreen : public Context, private InputListener {
	public:

		SettingsScreen(Display &display);

		virtual ~SettingsScreen();

		void start();

		void stop();

		void draw();

	protected:
		void deinit() override;

		void init() override;


	private:
		static SettingsScreen *instance;

		LinearLayout* screenLayout;
		DiscreteSlider* shutDownSlider;
		BooleanElement* camRotate;
		TextElement* inputTest;
		TextElement* save;

		int selectedSetting = 0;
		bool disableMainSelector = false;

		void buildUI();
		void buttonPressed(uint id) override;
		void selectApp(int8_t num);

		Color* backgroundBuffer= nullptr;
		fs::File introSong;
	};
}

#endif //WHEELSON_FIRMWARE_SETTINGSSCREEN_H
