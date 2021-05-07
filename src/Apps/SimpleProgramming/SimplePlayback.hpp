#ifndef WHEELSON_FIRMWARE_SIMPLEPLAYBACK_HPP
#define WHEELSON_FIRMWARE_SIMPLEPLAYBACK_HPP

#include <Display/Display.h>
#include <Support/Context.h>
#include <Loop/LoopListener.h>
#include <UI/LinearLayout.h>
#include <UI/ScrollLayout.h>
#include "PlaybackItem.h"

class SimplePlayback : public Context, public LoopListener {
public:
	SimplePlayback(Display& display);

	virtual ~SimplePlayback();

	void start();

	void draw();

	void stop();

	void loop(uint micros) override;

	void pack() override;

	void unpack() override;

private:

	static SimplePlayback *instance;

	ScrollLayout *scrollLayout;
	LinearLayout *layout;
	std::vector<PlaybackItem*> item;

	int8_t itemNum = 0;

	Color* backgroundBuffer = nullptr;

	void buildUI();

};

#endif //WHEELSON_FIRMWARE_SIMPLEPLAYBACK_HPP
