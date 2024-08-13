#ifndef WHEELSON_FIRMWARE_INTROSCREEN_H
#define WHEELSON_FIRMWARE_INTROSCREEN_H

#include <Support/Context.h>
#include <UI/Screen.h>
#include <Loop/LoopListener.h>
#include <Display/AnimatedSprite.h>
#include <Nuvoton/WheelsonLED.h>


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
	uint16_t previousTime = 0;
	WLEDColor lastColor = WLEDColor::WHITE;

};

#endif //WHEELSON_FIRMWARE_INTROSCREEN_H
