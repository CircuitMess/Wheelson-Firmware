#ifndef AUTOCAR_AUTONOMOUSAPP_H
#define AUTOCAR_AUTONOMOUSAPP_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Support/Context.h>
#include "../../defs.hpp"
#include <Sync/Mutex.h>
#include "esp_camera.h"
#include <JPEGDecoder.h>
#include "autonomousSettings.h"
#include <stdlib.h>
#include <UI/Image.h>
#include "../../Components/Motors.h"
#include "../../Components/AutoAction.h"
#include <Util/Task.h>
#include <Update/UpdateListener.h>
#include <Update/UpdateManager.h>

class AutonomousApp : public Context, public UpdateListener {
public:
	AutonomousApp(Display& display);

	void draw() override;
	void start() override;
	void stop() override;
    static AutonomousApp* getInstance();
    void update(uint _time) override;

private:
	static AutonomousApp* instance;
    Mutex mutexContrast;
    Mutex mutexMotors;
    bool showContrast = 1;
    bool motorsStop = 0;
    uint32_t contrastMillis = millis();
    double contrastThreshold = 122.0; //0 - 255
    uint16_t contrastDuration = 2000;
    static void contrastUp();
    static void contrastDown();
    static void toggleMotors();
    void jpegToArray(uint16_t *frameBuffer);
	Image baseImage;
    Motors *motors;
    void getCameraFrame(Task*);
    camera_fb_t * fb = NULL;
    uint16_t *imageStream = nullptr;
    AutoAction::Type currentDirection = AutoAction::FORWARD;
    static void exit();
    Task videoFeedTask;
    Task drawTask;
    static void updateFeedTask(Task*);
    void initCamera();
    void updateMotorsAction();
    Mutex feedMutex;
    Image contrastPopup;
    Mutex directionMutex;
    static void updateDrawTask(Task* task);
};

#endif