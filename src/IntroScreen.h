#ifndef WHEELSON_FIRMWARE_INTROSCREEN_H
#define WHEELSON_FIRMWARE_INTROSCREEN_H

#include <Support/Context.h>
#include <UI/Screen.h>
#include <Loop/LoopListener.h>
#include <Display/AnimatedSprite.h>



namespace IntroScreen {

	class IntroScreen: public Context, public LoopListener {
	public:

		IntroScreen(Display &display);

		virtual ~IntroScreen();

		void start();

		void stop();

		void draw();

		void loop(uint micros) override;

	private:
		static IntroScreen *instance;

		AnimatedSprite* gif = nullptr;


	};
}

#endif //WHEELSON_FIRMWARE_INTROSCREEN_H
