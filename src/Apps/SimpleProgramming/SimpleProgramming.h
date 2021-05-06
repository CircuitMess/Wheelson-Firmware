#ifndef WHEELSON_FIRMWARE_SIMPLEPROGRAMMING_H
#define WHEELSON_FIRMWARE_SIMPLEPROGRAMMING_H

#include <Support/Context.h>
#include <UI/Screen.h>
#include <UI/LinearLayout.h>
#include <UI/ScrollLayout.h>
#include <Loop/LoopListener.h>
#include "ProgramList.h"

class SimpleProgramming : public Context, public LoopListener{
public:

	SimpleProgramming(Display &display);

	virtual ~SimpleProgramming();

	void start();

	void stop();

	void draw();

	void loop(uint micros) override;

	void unpack() override;

private:

	static SimpleProgramming *instance;

	ScrollLayout* scrollLayout;
	LinearLayout* list;
	std::vector<ProgramList* > programs;

	Color* backgroundBuffer = nullptr;
	Color* addBuffer = nullptr;

	void buildUI();

};

#endif //WHEELSON_FIRMWARE_SIMPLEPROGRAMMING_H
