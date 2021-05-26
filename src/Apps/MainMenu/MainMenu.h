#ifndef AUTOCAR_MAINMENU_H
#define AUTOCAR_MAINMENU_H

#include <Arduino.h>
#include <Support/Context.h>
#include <UI/Layout.h>
#include <Elements/GridMenu.h>
#include <Input/InputListener.h>
#include <Loop/LoopListener.h>
#include "MainMenuItem.h"


class MainMenu : public Context, private InputListener, public LoopListener{
public:
	MainMenu(Display& display);

	virtual ~MainMenu();

	void start() override;

	void stop() override;

	void draw() override;

	void loop(uint micros) override;

protected:
	void init() override;
	void deinit() override;

private:
	static MainMenu* instance;

	Layout layout;
	std::vector<MainMenuItem*> apps;

	int8_t appNum = 0;
	uint8_t lastDrawnBatteryLevel = 0;
	void selectApp(int8_t num);
	Color* backgroundBuffer = nullptr;

	void buildUI();
	void buttonPressed(uint i) override;

	static const char* const AppTitles[5];

};


#endif //AUTOCAR_MAINMENU_H
