#include <Support/ContextTransition.h>
#include <Update/UpdateManager.h>
#include <sstream>
#include "TimelineApp.h"
#include "../../defs.hpp"
#include "../../Components/ActionProcessor.h"
#include <Input/Input.h>

TimelineApp* TimelineApp::instance = nullptr;

TimelineApp::TimelineApp(Display& display) : Context(display), timeline(new Timeline(display)),
											 menu(&screen, "Patterns"){

	instance = this;

	buildUI();
	pack();
}

void TimelineApp::play(uint index){
	const Vector<AutoAction>& pattern = patterns[index];

	for(const AutoAction& action : pattern){

		AutoAction* nAction = static_cast<AutoAction*>(malloc(sizeof(AutoAction)));
		memcpy(nAction, &action, sizeof(AutoAction));

		const size_t sizes[8] = {
			sizeof(MoveParams),
			sizeof(MoveParams),
			sizeof(MoveParams),
			sizeof(MoveParams),
			sizeof(LightParams),
			0,
			sizeof(ToneParams),
			sizeof(TuneParams)
		};

		size_t size = sizes[action.type];
		if(size != 0){
			nAction->params = malloc(size);
			memcpy(nAction->params, action.params, size);
		}

		ActionProcessor::getInstance()->addAction(nAction);
	}

	Serial.println("Starting ActionProcessor");
	ActionProcessor::getInstance()->start();
}

bool playing = false;
bool editing = false;

void TimelineApp::update(uint micros){
	if(filling != -1){
		filling = min(filling + (float) micros * 255 / 1000000, 255.0f);
		// menu.setSelectedFill(filling);
		menu.draw();
		screen.commit();
	}

	if(filling >= 255){
		instance->filling = -1;
		UpdateManager::removeListener(instance);
		// instance->menu.setSelectedFill(0);

		if(editing) return;

		instance->menu.draw();
		instance->screen.commit();

		if(instance->menu.getSelected() == instance->patterns.size()) return;

		playing = true;
		instance->timeline->initPattern(&instance->patterns[instance->menu.getSelected()], Timeline::Modus::PLAY);
		instance->timeline->push(instance);
		instance->play(instance->menu.getSelected());
	}
}

void TimelineApp::draw(){
	screen.draw();
	screen.commit();
}

void TimelineApp::start(){
	playing = editing = false;

	Input::getInstance()->setBtnPressCallback(BTN_B, [](){
		if(instance == nullptr) return;
		instance->pop();
	});

	Input::getInstance()->setBtnPressCallback(BTN_A, [](){

	});

	Input::getInstance()->setBtnReleaseCallback(BTN_A, [](){
		if(instance == nullptr) return;

		instance->filling = -1;
		UpdateManager::removeListener(instance);
		// instance->menu.setSelectedFill(0);

		if(!playing){
			instance->screen.commit();
		}

		editing = true;

		if(playing){
			playing = false;
			return;
		}

		if(instance->menu.getSelected() == instance->patterns.size()){
			instance->patterns.emplace_back();
			instance->timeline->initPattern(&instance->patterns.back());
		}else{
			instance->timeline->initPattern(&instance->patterns[instance->menu.getSelected()]);
		}

		instance->timeline->push(instance);
	});

	Input::getInstance()->setButtonHeldCallback(BTN_A, 100, [](){
		if(instance == nullptr) return;
		if(instance->packed) return; // TODO: remove when btmHeldCallback is added to CircuitOS

		instance->filling = 0;
		// instance->menu.setSelectedFill(0);
		UpdateManager::addListener(instance);
	});

	Input::getInstance()->setBtnPressCallback(BTN_UP, [](){
		if(instance == nullptr) return;
		instance->menu.selectPrev();
		instance->screen.commit();
	});

	Input::getInstance()->setBtnPressCallback(BTN_DOWN, [](){
		if(instance == nullptr) return;
		instance->menu.selectNext();
		instance->screen.commit();
	});

	draw();
}

void TimelineApp::unpack(){
	Context::unpack();
	fillMenu();
}

void TimelineApp::stop(){
	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnReleaseCallback(BTN_A);
	// Input::getInstance()->removeBtnHeldCallback(BTN_A);
	Input::getInstance()->removeBtnPressCallback(BTN_B);
	Input::getInstance()->removeBtnPressCallback(BTN_UP);
	Input::getInstance()->removeBtnPressCallback(BTN_DOWN);
	UpdateManager::removeListener(this);
}

void TimelineApp::fillMenu(){
	int selected = menu.getSelected();
	menu.clearItems();

	int i = 0;
	std::ostringstream buffer;
	for(const auto& pattern : patterns){
		buffer.str(std::string());
		buffer << "Pattern " << ++i;
		std::string str = buffer.str();

		char* title = static_cast<char*>(malloc(str.size() + 1));
		memset(title, 0, str.size() + 1);
		memcpy(title, str.c_str(), str.size());
		menu.addItem(title);
	}

	menu.addItem("New");
	menu.setSelected(selected);
	menu.reflow();
	menu.repos();
}

void TimelineApp::buildUI(){
	menu.setTitleColor(TFT_RED, TFT_BLACK);
	menu.setWHType(PARENT, PARENT);
	fillMenu();

	screen.addChild(&menu);
	screen.repos();
}

