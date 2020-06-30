#include <Update/UpdateManager.h>
#include "autonomousSettings.h"
#include "autonomousApp.h"
#include "../../defs.hpp"
#include <Input/Input.h>

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include <NeoPixelBrightnessBus.h>

AutonomousApp* AutonomousApp::instance = nullptr;

const char* AutonomousApp::DirectionStrings[] = { "FORWARD", "BACKWARD", "LEFT", "RIGHT" };

AutonomousApp::AutonomousApp(Display& display) :
		Context(display),
		contrastPopup(&screen, 124, 34),
		motors(Motors::getInstance()),
		processTask("videoTask", updateFeedTask, 10000),
		feed(160, 128),
		frameBuffers { (uint16_t*) malloc(128 * 168 * sizeof(uint16_t)), (uint16_t*) malloc(128 * 168 * sizeof(uint16_t)) }{

    instance = this;

	addSprite(&contrastPopup);
	contrastPopup.setPos(18, 48);
	screen.addChild(&contrastPopup);

	imageBuffer = (uint16_t*) malloc(128 * 160 * sizeof(uint16_t));

    pack();
}

int lastTop = 80;
int lastBottom = 80;
void AutonomousApp::processFrame()
{
	volatile const uint16_t* frame = imageBuffer;

	uint16_t leftCounted = 0;
	uint16_t rightCounted = 0;

	/*for(uint16_t i = 0; i < 160 * 120; i++){
		uint16_t color = frame[i];
		// uint8_t r = (frame[i] & 0xF800) >> 8;
		// uint8_t g = (frame[i] & 0x07E0) >> 3;
		// uint8_t b = (frame[i] & 0x1F) << 3;
		double luminance = 0.2126 * ((color & 0xF800) >> 8) + 0.7152 * ((color & 0x07E0) >> 3) + 0.0722 * ((color & 0x1F) << 3);

		int x = (i % 160);
		int y = i - x * 160;
		if(luminance < settings()->contrastSetting){
			if(x < 60){
				leftCounted++;
			}else if(x > 100){
				rightCounted++;
			}
		}
	}

	int64_t difference = leftCounted - rightCounted;
	if(leftCounted > rightCounted && abs(difference) > 1000){
		currentDirection = AutoAction::LEFT;
	}else if(leftCounted < rightCounted && abs(difference) > 1000){
		currentDirection = AutoAction::RIGHT;
	}else{
		currentDirection = AutoAction::FORWARD;
	}
	 */

	int totalCounted = 0;
	float top = 0;
	for(int i = 0; i < 160 * 10; i++){
		uint16_t color = frame[i];
		double luminance = 0.2126 * ((color & 0xF800) >> 8) + 0.7152 * ((color & 0x07E0) >> 3) + 0.0722 * ((color & 0x1F) << 3);

		int x = (i % 160);
		if(luminance < settings()->contrastSetting){
			totalCounted++;
			top += x;
		}
	}
	if(totalCounted < 50){
		top = lastTop;
	}else{
		top /= totalCounted;
		lastTop = top;
	}

	totalCounted = 0;
	float bottom = 0;
	for(int i = 160 * 117; i < 160 * 128; i++){
		uint16_t color = frame[i];
		double luminance = 0.2126 * ((color & 0xF800) >> 8) + 0.7152 * ((color & 0x07E0) >> 3) + 0.0722 * ((color & 0x1F) << 3);

		int x = (i % 160);
		if(luminance < settings()->contrastSetting){
			totalCounted++;
			bottom += x;
		}
	}
	bottom /= totalCounted;

	if(!driving) return;

	uint a = 40, b = 120;
	if(top < b && top > a  && bottom < b && bottom > a){
		if(fabs(top - bottom) < 30){
			currentDirection = AutoAction::FORWARD;
		}else if(top > bottom){
			currentDirection = AutoAction::RIGHT;
		}else if(top < bottom){
			currentDirection = AutoAction::LEFT;
		}
	}else if(fabs(top - bottom) > 30){
		currentDirection = AutoAction::FORWARD;
	}else if(top < a || bottom < a){
		currentDirection = AutoAction::LEFT;
	}else if(top > b || bottom > b){
		currentDirection = AutoAction::RIGHT;
	}

	switch(currentDirection){
		case AutoAction::LEFT:
			motors->rightMotor(MOTOR_FORWARD);
			motors->leftMotor(MOTOR_BACKWARD);
			break;
		case AutoAction::RIGHT:
			motors->leftMotor(MOTOR_FORWARD);
			motors->rightMotor(MOTOR_BACKWARD);
			break;
		case AutoAction::FORWARD:
			motors->leftMotor(MOTOR_FORWARD);
			motors->rightMotor(MOTOR_FORWARD);
			break;
	}
}

void AutonomousApp::draw()
{
	Sprite* canvas = screen.getSprite();

	canvas->clear(TFT_DARKGREY);
	canvas->setTextColor(TFT_WHITE);
	canvas->setTextSize(1);
	canvas->setTextFont(2);

	if(imageBuffer != nullptr){
		volatile const uint16_t* cameraFrame = imageBuffer;
		canvas->drawIcon(const_cast<const unsigned short*>(cameraFrame), 0, 0, 160, 120);
	}


	if(contrastShown != -1){
		uint8_t fillValue = map(settings()->contrastSetting, 0, 255, 0, 112);
		// mutexContrast.unlock();
		contrastPopup.getSprite()->fillRect(7, 23, 112, 2, TFT_DARKGREY);
		contrastPopup.getSprite()->fillRect(7, 23, fillValue, 2, TFT_WHITE);
		contrastPopup.draw();
	}

	if(!debug){
		canvas->fillRoundRect(0, 106, 160, 36, 4, TFT_RED);
		canvas->fillRoundRect(2, 108, 156, 34, 3, TFT_DARKGREY);
	}else{
		canvas->setTextColor(TFT_RED);
	}
	canvas->setCursor(7,109);


	if(imageBuffer == nullptr){
		canvas->print("Camera error");
	}else if(!driving){
		canvas->print("Motors stopped");
	}else{
		canvas->print(DirectionStrings[currentDirection]);
	}

	if(debug){
		canvas->setTextColor(TFT_RED);
		float frameTime = (float) (micros() - frameMicros) / 1000000.0f;
		float camTime = (float) (micros() - camMicros) / 1000000.0f;
		frameMicros = micros();
		canvas->setCursor(7, 2);
		canvas->printf("S: %.0f fps  /  C: %.0f fps\n", 1.0f / frameTime, 1.0f / camTime);
	}

    screen.commit();
}

bool processPress = false;

void AutonomousApp::start()
{
	Settings::retrieve();
	Serial.printf("Settings retrieved, contrast: %.2f\n", settings()->contrastSetting);

	contrastPopup.getSprite()->clear(TFT_GREEN);
	contrastPopup.getSprite()->setChroma(TFT_GREEN);
	contrastPopup.getSprite()->fillRoundRect(0, 0, contrastPopup.getWidth(), contrastPopup.getHeight(), 3, TFT_RED);
	contrastPopup.getSprite()->fillRoundRect(1, 1, contrastPopup.getWidth() - 2, contrastPopup.getHeight() - 2, 3, TFT_RED);
	contrastPopup.getSprite()->fillRoundRect(2, 2, contrastPopup.getWidth() - 4, contrastPopup.getHeight() - 4, 3, TFT_DARKGREY);
	contrastPopup.getSprite()->setCursor(0, 2);
	contrastPopup.getSprite()->setTextColor(TFT_WHITE);
	contrastPopup.getSprite()->setTextFont(2);
	contrastPopup.getSprite()->setTextSize(1);
	contrastPopup.getSprite()->printCenter("Contrast");
	contrastPopup.getSprite()->drawRect(4, 20, 116, 8, TFT_WHITE);
	contrastPopup.getSprite()->drawRect(5, 21, 114, 6, TFT_WHITE);

	Input::getInstance()->setBtnPressCallback(BTN_LEFT, [](){
		instance->contrastShown = millis();
		settings()->contrastSetting = max(settings()->contrastSetting - 5, 0.0);
	});

	Input::getInstance()->setBtnPressCallback(BTN_RIGHT, [](){
		instance->contrastShown = millis();
		settings()->contrastSetting = min(settings()->contrastSetting + 5, 255.0);
	});

	Input::getInstance()->setButtonHeldCallback(BTN_A, 500, [](){
		processPress = true;
		if(instance == nullptr) return;
		instance->feed.toggleProcessFeed();

	});

	Input::getInstance()->setBtnReleaseCallback(BTN_A, [](){
		if(processPress){
			processPress = false;
			return;
		}

		instance->contrastShown = -1;
		instance->driving = !instance->driving;

		if(!instance->driving){
			instance->motors->stopAll();
		}
	});

	Input::getInstance()->setBtnPressCallback(BTN_B, [](){
		if(instance == nullptr) return;
		instance->pop();
	});

	driving = false;
	UpdateManager::addListener(this);
	processTask.start(10);
}

void AutonomousApp::stop(){
	Input::getInstance()->removeBtnPressCallback(BTN_LEFT);
	Input::getInstance()->removeBtnPressCallback(BTN_RIGHT);
	Input::getInstance()->removeBtnReleaseCallback(BTN_A);
	Input::getInstance()->removeBtnPressCallback(BTN_B);

	UpdateManager::removeListener(this);
	processTask.stop();

	Settings::store();
	Serial.printf("Settings stored, contrast: %.2f\n", settings()->contrastSetting);
}

void AutonomousApp::updateFeedTask(Task* task){
	while(task->running){
		if(instance->packed) return;

		if(instance == nullptr){
			delay(1);
			continue;
		}

		instance->camMicros = micros();

		instance->feed.loadFrame();
		uint16_t* targetBuffer = (instance->imageBuffer == instance->frameBuffers[0] ? instance->frameBuffers[1]
																					 : instance->frameBuffers[0]);

		memcpy(targetBuffer, instance->feed.getBuffer(), 128 * 160 * sizeof(uint16_t));
		instance->imageBuffer = targetBuffer;

		instance->processFrame();
	}

	instance->motors->stopAll();
}

void AutonomousApp::update(uint){
	if(contrastShown != -1){
		if(millis() - contrastShown > contrastDuration){
			contrastShown = -1;
		}
	}

	draw();
}