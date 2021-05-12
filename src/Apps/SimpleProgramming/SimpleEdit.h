#ifndef WHEELSON_FIRMWARE_SIMPLEEDIT_H
#define WHEELSON_FIRMWARE_SIMPLEEDIT_H

#include <Support/Context.h>
#include <UI/Screen.h>
#include <UI/LinearLayout.h>
#include <Loop/LoopListener.h>
#include <UI/ScrollLayout.h>
#include <UI/GridLayout.h>
#include <Input/InputListener.h>
#include "SimpleAction.h"

class SimpleEdit : public Context, public LoopListener, private InputListener {
public:

	SimpleEdit(Display& display);

	virtual ~SimpleEdit();

	void start();

	void stop();

	void draw();

	void pack() override;

	void unpack() override;

	void loop(uint micros) override;

	void returned(void* data) override;

private:

	static SimpleEdit* instance;

	ScrollLayout* scrollLayout;
	GridLayout* list;
	Vector<SimpleAction*> actions;

	Color* backgroundBuffer = nullptr;
	Color* addBuffer = nullptr;
	uint8_t actionNum = 0;

	void buttonPressed(uint id) override;

	void selectAction(uint8_t num);

	void buildUI();

};

#endif //WHEELSON_FIRMWARE_SIMPLEEDIT_H
