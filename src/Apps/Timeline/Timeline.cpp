#include "Timeline.h"
#include "../../Wheelson.h"
#include "../../Elements/ActionItem.h"
#include "../../Components/ActionProcessor.h"
#include <Input/Input.h>
#include <Support/ContextTransition.h>
#include <Util/Debug.h>
#include <Loop/LoopManager.h>
#include <sstream>
#include <iomanip>
#include <Input/Input.h>


Mutex* screenMutex = nullptr;

 const char* const Timeline::ActionsSprites[] = {
		"/arrow_up.raw", "/arrow_down.raw", "/arrow_left.raw", "/arrow_right.raw", "/light_on.raw", "/light_off.raw", "/tone.raw", "/tune.raw"
};

Timeline* Timeline::instance = nullptr;

Timeline::Timeline(Display& display) : Context(display),
									   layers(&screen), fleha(&layers, display.getWidth(), display.getHeight()),
									   scroll(&layers), timelineList(&scroll, VERTICAL),
									   selector(this), aEditor(this){

	for(int i = 0; i < 8; i++){
		buffer[i] = static_cast<Color*>(malloc(18 * 18 * 2));
		if(buffer[i]== nullptr){
			Serial.printf("Timeline picture %s unpack error\n", ActionsSprites[i]);
			return;
		}
		iconFile[i] = SPIFFS.open(ActionsSprites[i]);
		iconFile[i].seek(0);
		iconFile[i].read(reinterpret_cast<uint8_t*>(buffer[i]), 18 * 18 * 2);
		iconFile[i].close();
	}

	bufferAdd = static_cast<Color*>(malloc(18 * 18 * 2));
	if(bufferAdd == nullptr){
		Serial.println("Timeline picture /add.raw unpack error");
		return;
	}
	addFile = SPIFFS.open("/add.raw");
	addFile.seek(0);
	addFile.read(reinterpret_cast<uint8_t*>(bufferAdd), 18 * 18 * 2);
	addFile.close();

	instance = this;

	screenMutex = Motors::getInstance()->mutex;

	buildUI();
	pack();
}

void Timeline::initPattern(Vector<AutoAction>* actions, Modus modus){
	this->actions = actions;
	this->modus = modus;
	fillMenu();
	instance->scroll.setScroll(0, 0);
}

void Timeline::returned(void* data){
	int* type = static_cast<int*>(data);

	if(*type != -1){
		addAction(static_cast<AutoAction::Type>(*type));
		instance->timelineList.reflow();
		instance->timelineList.getChildren().relocate(instance->actions->size(), instance->actions->size() - 1);
		instance->timelineList.repos();
		instance->selectedAction++;
		instance->scroll.scrollIntoView(instance->timelineList.getChildren().size() - 1, 5);
	}

	delete type;
}

void Timeline::addAction(AutoAction::Type type){
	AutoAction action = { type, nullptr };

	switch(type){
		case AutoAction::Type::FORWARD:
		case AutoAction::Type::BACKWARD:
		case AutoAction::Type::LEFT:
		case AutoAction::Type::RIGHT:
			action.params = new MoveParams();
			break;

		case AutoAction::Type::LIGHT_ON:
			action.params = new LightParams();
			break;

		case AutoAction::Type::LIGHT_OFF:
			break;

		case AutoAction::Type::TONE:
			action.params = new ToneParams();
			break;

		case AutoAction::Type::TUNE:
			action.params = new TuneParams();
			break;

		default:
			logln("Invalid AutoAction type passed to Timeline::addAction");
			return;
	}

	actions->push_back(action);
	timelineList.addChild(new ActionItem(&timelineList, buffer[action.type], ActionText[action.type]));
}

void Timeline::draw(){
	screen.draw();
	screenMutex->lock();

	screenMutex->unlock();
}

int timeleft = 0;

void Timeline::start(){
	draw();
	screen.commit();

	if(modus == PLAY){
		ActionProcessor::getInstance()->setActionListener([](){
			if(instance == nullptr) return;

			uint oldSelected = instance->selectedAction;
			reinterpret_cast<ActionItem*>(instance->timelineList.getChildren()[instance->selectedAction])->setSelected(false);
			instance->selectedAction = max((int)(instance->actions->size() - ActionProcessor::getInstance()->count() - 1), 0);
			reinterpret_cast<ActionItem*>(instance->timelineList.getChildren()[instance->selectedAction])->setSelected(true);

			const AutoAction& action = instance->actions->at(instance->selectedAction);
			Vector<AutoAction::Type> durationTypes = { AutoAction::Type::FORWARD, AutoAction::Type::BACKWARD,
													   AutoAction::Type::LEFT, AutoAction::Type::RIGHT,
													   AutoAction::Type::TONE };

			if(durationTypes.indexOf(action.type) != (uint) -1){
				timeleft = static_cast<DurationActionParams*>(action.params)->millis * 1000;
				Serial.printf("setting timeleft to %d", timeleft);
			}else{
				timeleft = 0;
			}

			if(durationTypes.indexOf(instance->actions->at(oldSelected).type) != (uint) -1){
				TextElement* text = reinterpret_cast<TextElement*>(reinterpret_cast<ActionItem*>(instance->timelineList.getChild(oldSelected))->getChild(1));
				text->setText(ActionText[instance->actions->at(oldSelected).type]);
				delay(10);
				text->draw();
			}

			instance->scroll.scrollIntoView(instance->selectedAction, 5);
		});

		ActionProcessor::getInstance()->setDoneListener([](){
			if(instance == nullptr) return;
			instance->pop();
		});

		timeleft = 0;
		LoopManager::addListener(this);
		return;
	}

	Input::getInstance()->setBtnPressCallback(BTN_B, [](){
		if(instance == nullptr) return;
		instance->pop();
	});

	Input::getInstance()->setBtnPressCallback(BTN_A, [](){
		if(instance == nullptr) return;

		if(instance->selectedAction == instance->actions->size()){
			instance->selector.push(instance);
		}else{
			if(instance->actions->at(instance->selectedAction).type == AutoAction::Type::LIGHT_OFF) return;

			instance->aEditor.initAction(instance->actions->at(instance->selectedAction).type,
										 &instance->actions->at(instance->selectedAction));
			instance->aEditor.push(instance);
		}
	});

	Input::getInstance()->setBtnPressCallback(BTN_UP, [](){
		if(instance == nullptr) return;

		reinterpret_cast<ActionItem*>(instance->timelineList.getChildren()[instance->selectedAction])->setSelected(
				false);
		if(instance->selectedAction == 0){
			instance->selectedAction = instance->timelineList.getChildren().size() - 1;
		}else{
			instance->selectedAction--;
		}
		reinterpret_cast<ActionItem*>(instance->timelineList.getChildren()[instance->selectedAction])->setSelected(
				true);

		instance->scroll.scrollIntoView(instance->selectedAction, 5);
		instance->draw();
	});

	Input::getInstance()->setBtnPressCallback(BTN_DOWN, [](){
		if(instance == nullptr) return;

		reinterpret_cast<ActionItem*>(instance->timelineList.getChildren()[instance->selectedAction])->setSelected(
				false);
		instance->selectedAction = (instance->selectedAction + 1) % (instance->timelineList.getChildren().size());
		reinterpret_cast<ActionItem*>(instance->timelineList.getChildren()[instance->selectedAction])->setSelected(
				true);

		instance->scroll.scrollIntoView(instance->selectedAction, 5);
		instance->draw();
	});
}

void Timeline::stop(){
	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnPressCallback(BTN_B);
	Input::getInstance()->removeBtnPressCallback(BTN_UP);
	Input::getInstance()->removeBtnPressCallback(BTN_DOWN);
	LoopManager::removeListener(this);
	for(int i=0;i<8;i++){
		free(buffer[i]);
		buffer[i]= nullptr;
		iconFile[i].close();
	}
	free(bufferAdd);
	bufferAdd= nullptr;
	addFile.close();
}

void Timeline::fillMenu(){
	for(auto child : timelineList.getChildren()){
		delete child;
	}
	timelineList.getChildren().clear();

	if(actions == nullptr) return;

	for(const auto& action : *actions){
		timelineList.addChild(new ActionItem(&timelineList, buffer[action.type], ActionText[action.type]));
	}

	if(modus == EDIT){
		ActionItem* item = new ActionItem(&timelineList, bufferAdd, "New action");
		timelineList.addChild(item);
	}

	reinterpret_cast<ActionItem*>(timelineList.getChildren().front())->setSelected(true);
	selectedAction = 0;

	timelineList.reflow();
	timelineList.repos();
}

void Timeline::buildUI(){
	layers.setWHType(PARENT, PARENT);
	layers.addChild(&fleha);
	layers.addChild(&scroll);
	layers.reflow();

	scroll.setWHType(PARENT, PARENT);
	scroll.addChild(&timelineList);
	scroll.reflow();

	timelineList.setWHType(PARENT, CHILDREN);
	timelineList.setPadding(5);
	timelineList.setGutter(5);

	fleha.bgColor = TFT_BLACK;
	fleha.borderTopColor = fleha.borderBotColor = TFT_RED;

	screen.addChild(&layers);
}

void Timeline::loop(uint micros){
	if(timeleft <= 0) return;

	timeleft -= micros;
	TextElement* text = reinterpret_cast<TextElement*>(reinterpret_cast<ActionItem*>(timelineList.getChild(selectedAction))->getChild(1));
	const AutoAction& action = actions->at(selectedAction);
	auto time = max(0.0f, (float) timeleft / 1000000);

	std::ostringstream buffer;
	buffer.precision(2);
	buffer << ActionText[action.type] << " ";
	buffer << std::fixed << std::setprecision(2) << std::setw(2) << time << "s";
	text->setText(buffer.str());
	text->getSprite()->fillRect(text->getTotalX(), text->getTotalY(), text->getWidth(), text->getHeight(), TFT_BLACK);
	text->draw();

	screenMutex->lock();
	screen.commit();
	screenMutex->unlock();
}

