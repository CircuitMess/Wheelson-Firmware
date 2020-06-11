#include "autonomousApp.h"

AutonomousApp* AutonomousApp::instance = nullptr;

AutonomousApp::AutonomousApp(Display& display) :
				Context(display),
				baseImage(&screen, screen.getWidth(), screen.getHeight()),
				contrastPopup(&screen, 124, 34),
				motors(Motors::getInstance()),
				videoFeedTask("videoTask", updateFeedTask, 10000),
				drawTask("drawTask", updateDrawTask, 10000)
{
    instance = this;
	addSprite(&baseImage);
	addSprite(&contrastPopup);
	screen.addChild(&baseImage);
	screen.addChild(&contrastPopup);
	baseImage.setPos(0,0);
	contrastPopup.setPos(18, 48);
	

	initCamera();
    pack();
}

void AutonomousApp::getCameraFrame(Task* task)
{
	uint32_t timerMillis = millis();
	
	fb = esp_camera_fb_get();
	if(!fb) {
	Serial.println("Camera capture failed");
	return;
	}

	// String path = "/picture.jpg";
	// Serial.printf("Picture file name: %s\n", path.c_str());
	// File file = fs.open(path.c_str(), FILE_WRITE);
	// if(!file){
	// Serial.println("Failed to open file in writing mode");
	// }
	// else {
	// file.write(fb->buf, fb->len); // payload (image), payload length
	// Serial.printf("Saved file to path: %s\n", path.c_str());
	// }
	// file.close();
	// file = fs.open(path.c_str(), "r");
	// Serial.println("no crash4");

	JpegDec.decodeArray(fb->buf, fb->len);
	esp_camera_fb_return(fb);
	// JpegDec.decodeSdFile(file);
	// Serial.println("no crash5");
	uint16_t *decodedArrayRGB = (uint16_t*)ps_malloc(160*120);
	// Serial.println("no crash5.1");

	free(decodedArrayRGB);
	// Serial.println("no crash5.2");

	jpegToArray(decodedArrayRGB);
	if(!task->running) return;
	// feedMutex.lock();
	imageStream = decodedArrayRGB;
	// feedMutex.unlock();
	// Serial.println("no crash5.5");
	bool *monochromeArray = (bool*)malloc(160*120);
	// Serial.println("mallocated");
	uint16_t leftCounted = 0;
	uint16_t rightCounted = 0;

	for(uint16_t i = 0; i < 160*120; i++)
	{
		uint16_t color = decodedArrayRGB[i];
		// uint8_t r = (decodedArrayRGB[i] & 0xF800) >> 8;
		// uint8_t g = (decodedArrayRGB[i] & 0x07E0) >> 3;
		// uint8_t b = (decodedArrayRGB[i] & 0x1F) << 3;
		double luminance = 0.2126*((color & 0xF800) >> 8) + 0.7152*((color & 0x07E0) >> 3) + 0.0722*((color & 0x1F) << 3);
		// mutexContrast.lock();
		monochromeArray[i] = luminance < contrastThreshold; //tolerance level
		if(luminance < contrastThreshold)
		{
			if((i%160) < 80)
				leftCounted++;
			else
				rightCounted++;
		}
		// mutexContrast.unlock();
	}
	int64_t difference = leftCounted - rightCounted;
	if(leftCounted > rightCounted && abs(difference) > 1000)
	{
		currentDirection = AutoAction::LEFT;
	}
	else if(leftCounted < rightCounted && abs(difference) > 1000)
	{
		currentDirection = AutoAction::RIGHT;
	}
	else
	{
		currentDirection = AutoAction::FORWARD;
	}
	free(monochromeArray);
	
	// Serial.println("no crash6");
	timerMillis = millis() - timerMillis;
	// Serial.println("-------------");
	// Serial.printf("Done in %dms\n", timerMillis);
	// Serial.printf("Estimated framerate %.2f\n", (float)(1000.0/(float)timerMillis));
	// Serial.println("-------------");
}

uint frameMicros = 0;
void AutonomousApp::draw()
{
	screen.getSprite()->clear(TFT_WHITE);
	bool noImage = 0;

	// feedMutex.lock();
	if(imageStream != nullptr){
		noImage = 1;
		screen.getSprite()->drawIcon(imageStream, 0, 0, 160, 120);
	}
	// feedMutex.unlock();
	
	screen.getSprite()->setTextColor(TFT_RED);
	screen.getSprite()->setTextColor(TFT_RED);
	screen.getSprite()->setTextSize(1);
	screen.getSprite()->setTextFont(2);
	screen.getSprite()->fillRoundRect(0, 106, 160, 36, 4, TFT_RED);
	screen.getSprite()->fillRoundRect(2, 108, 156, 34, 3, TFT_DARKGREY);
	// mutexContrast.lock();
	if(showContrast && millis() - contrastMillis <= contrastDuration)
	{
		uint8_t fillValue = map(contrastThreshold, 0, 255, 0, 112);
		// mutexContrast.unlock();
		contrastPopup.getSprite()->fillRect(7, 23, 112, 2, TFT_DARKGREY);
		contrastPopup.getSprite()->fillRect(7, 23, fillValue, 2, TFT_WHITE);
		contrastPopup.draw();
	}
	else
	{
		contrastMillis = millis();
		showContrast = 0;
	}
	// mutexContrast.unlock();
	screen.getSprite()->setCursor(7,109);
	screen.getSprite()->setTextColor(TFT_WHITE);
	// mutexMotors.lock();
	if(motorsStop)
	{
		// mutexMotors.unlock();
		screen.getSprite()->print("Motors stopped");
	}
	else
	{
		// mutexMotors.unlock();
		if(noImage){
			// directionMutex.lock();
			switch (currentDirection)
			{
			case AutoAction::LEFT:
					// directionMutex.unlock();
					screen.getSprite()->print("Left");
				break;
			case AutoAction::RIGHT:
					// directionMutex.unlock();
					screen.getSprite()->print("Right");
				break;
			case AutoAction::FORWARD:
					// directionMutex.unlock();
					screen.getSprite()->print("Forward");
				break;
			}
			// directionMutex.unlock();
		}
		else
		{
			screen.getSprite()->print("Camera error");
		}
	}

	float frameTime = (float) (micros() - frameMicros) / 1000000.0f;
  	frameMicros = micros();

  	screen.getSprite()->setCursor(5, 5);
  	screen.getSprite()->printf("%.0f fps", 1.0f / frameTime);
    screen.commit();
}
void AutonomousApp::start()
{
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
	
	contrastThreshold = (double)(settings()->contrastSetting);
	motorsStop = (bool)(settings()->motorsStop);
	contrastMillis = millis();
	showContrast = 0;

	
	
	Input::getInstance()->setBtnPressCallback(BTN_LEFT, contrastDown);
	Input::getInstance()->setBtnPressCallback(BTN_RIGHT, contrastUp);
	Input::getInstance()->setBtnPressCallback(BTN_A, toggleMotors);
	Input::getInstance()->setBtnPressCallback(BTN_B, [](){
		if(instance == nullptr) return;
		instance->pop();
	});
	UpdateManager::addListener(this);
	// drawTask.setPinned(1);
	videoFeedTask.start(10 | portPRIVILEGE_BIT);
    
}
void AutonomousApp::stop()
{
	videoFeedTask.stop();
	UpdateManager::removeListener(this);
    Input::getInstance()->removeBtnPressCallback(BTN_LEFT);
    Input::getInstance()->removeBtnPressCallback(BTN_RIGHT);
    Input::getInstance()->removeBtnPressCallback(BTN_A);
    Input::getInstance()->removeBtnPressCallback(BTN_B);
}
void AutonomousApp::contrastUp()
{
	// instance->mutexContrast.lock();
	instance->showContrast = 1;
	instance->contrastMillis = millis();
	if(instance->contrastThreshold < 255)
		instance->contrastThreshold+=5;
	if(instance->contrastThreshold > 255)
		instance->contrastThreshold = 255;
	settings()->contrastSetting = uint8_t(instance->contrastThreshold);
	// instance->mutexContrast.unlock();
	// Settings::store();
}
void AutonomousApp::contrastDown()
{
	// instance->mutexContrast.lock();
	instance->showContrast = 1;
	instance->contrastMillis = millis();
	if(instance->contrastThreshold > 0)
		instance->contrastThreshold-=5;
	if(instance->contrastThreshold < 0)
		instance->contrastThreshold = 0;
	settings()->contrastSetting = uint8_t(instance->contrastThreshold);
	// instance->mutexContrast.unlock();
	// Settings::store();
}
void AutonomousApp::toggleMotors()
{
	// instance->mutexContrast.lock();
	instance->showContrast = 0;
	instance->contrastMillis = millis();
	// instance->mutexContrast.unlock();
	// instance->mutexMotors.lock();
	instance->motorsStop = !instance->motorsStop;
	settings()->motorsStop = instance->motorsStop;
	// instance->mutexMotors.unlock();
	// Settings::store();
}
void AutonomousApp::jpegToArray(uint16_t *frameBuffer)
{
	// Serial.println("JPEG to array");
	// delay(5);

	// retrieve infomration about the image
	uint16_t *pImg;
	uint16_t mcu_w = JpegDec.MCUWidth;
	uint16_t mcu_h = JpegDec.MCUHeight;
	uint32_t max_x = JpegDec.width;
	uint32_t max_y = JpegDec.height;
	

	// Jpeg images are drawn as a set of image block (tiles) called Minimum Coding Units (MCUs)
	// Typically these MCUs are 16x16 pixel blocks
	// Determine the width and height of the right and bottom edge image blocks
	// Serial.println("Here");
	// delay(5);
	uint32_t min_w = min<int>(mcu_w, max_x % mcu_w);

	uint32_t min_h = min<int>(mcu_h, max_y % mcu_h);

	// save the current image block size
	uint32_t win_w = mcu_w;
	uint32_t win_h = mcu_h;

	// record the current time so we can measure how long it takes to draw an image
	uint32_t drawTime = millis();

	// save the coordinate of the right and bottom edges to assist image cropping
	// to the screen size
	// read each MCU block until there are no more
	while (JpegDec.readSwappedBytes())
	{ // Swap byte order so the SPI buffer can be used

		// save a pointer to the image block
		pImg = JpegDec.pImage;

		// calculate where the image block should be drawn on the screen
		int mcu_x = JpegDec.MCUx * mcu_w; // Calculate coordinates of top left corner of current MCU
		int mcu_y = JpegDec.MCUy * mcu_h;

		// check if the image block size needs to be changed for the right edge
		if (mcu_x + mcu_w <= max_x)
			win_w = mcu_w;
		else
			win_w = min_w;

		// check if the image block size needs to be changed for the bottom edge
		if (mcu_y + mcu_h <= max_y)
			win_h = mcu_h;
		else
			win_h = min_h;

		// copy pixels into a contiguous block
		if (win_w != mcu_w)
		{
			uint16_t *cImg;
			int p = 0;
			cImg = pImg + win_w;
			for (int h = 1; h < win_h; h++)
			{
				p += mcu_w;
				for (int w = 0; w < win_w; w++)
				{
					*cImg = *(pImg + w + p);
					cImg++;
				}
			}
		}

		// draw image MCU block only if it will fit on the screen
		for(uint32_t x = 0; x < mcu_w; x++)
		{
			for(uint32_t y = 0; y < mcu_h; y++)
			{
				frameBuffer[max_x*(mcu_y + y) + mcu_x + x] = pImg[y*mcu_w + x] << 8 | pImg[y*mcu_w + x] >> 8;
			}
		}
	}

	// calculate how long it took to draw the image
	drawTime = millis() - drawTime; // Calculate the time it took

	// print the results to the serial port
	// Serial.print("Total render time was    : ");
	// Serial.print(drawTime);
	// Serial.println(" ms");
	// Serial.println("=====================================");
	// return frameBuffer;
	// free(frameBuffer);

}
void AutonomousApp::updateFeedTask(Task* task)
{
	while(task->running){
		if(instance->packed) return;
		if(instance == nullptr)
		{
			// vTaskDelay(1);
			continue;
		}
		instance->getCameraFrame(task);
		if(!task->running) break;
		instance->updateMotorsAction();
		// vTaskDelay(1);
	}
	instance->motors->stopAll();
}
void AutonomousApp::updateDrawTask(Task* task)
{
	while(task->running){
		if(instance->packed) return;
		if(instance == nullptr)
		{
			vTaskDelay(1);
			continue;
		}
		instance->draw();
		vTaskDelay(1);
	}
}
void AutonomousApp::update(uint)
{
	draw();
}
AutonomousApp* AutonomousApp::getInstance()
{
    return instance;
}
void AutonomousApp::initCamera()
{
	camera_config_t config;
	config.ledc_channel = LEDC_CHANNEL_0;
	config.ledc_timer = LEDC_TIMER_0;
	config.pin_d0 = Y2_GPIO_NUM;
	config.pin_d1 = Y3_GPIO_NUM;
	config.pin_d2 = Y4_GPIO_NUM;
	config.pin_d3 = Y5_GPIO_NUM;
	config.pin_d4 = Y6_GPIO_NUM;
	config.pin_d5 = Y7_GPIO_NUM;
	config.pin_d6 = Y8_GPIO_NUM;
	config.pin_d7 = Y9_GPIO_NUM;
	config.pin_xclk = XCLK_GPIO_NUM;
	config.pin_pclk = PCLK_GPIO_NUM;
	config.pin_vsync = VSYNC_GPIO_NUM;
	config.pin_href = HREF_GPIO_NUM;
	config.pin_sscb_sda = SIOD_GPIO_NUM;
	config.pin_sscb_scl = SIOC_GPIO_NUM;
	config.pin_pwdn = -1;
	config.pin_reset = RESET_GPIO_NUM;
	config.xclk_freq_hz = 20000000;
	config.pixel_format = PIXFORMAT_JPEG;
	
	config.frame_size = FRAMESIZE_QQVGA;
	config.jpeg_quality = 7;
	config.fb_count = 1;

	// Init Camera
	esp_err_t err = esp_camera_init(&config);
	if (err != ESP_OK) {
	Serial.printf("Camera init failed with error 0x%x", err);
	return;
	}
	Serial.println("Camera init OK!");
	sensor_t *sensor = esp_camera_sensor_get();
	sensor->set_hmirror(sensor, 1);
	sensor->set_vflip(sensor, 1);
	sensor->set_quality(sensor, 10);
	sensor->set_framesize(sensor, FRAMESIZE_QQVGA);
	sensor->set_special_effect(sensor, 0);
}
void AutonomousApp::updateMotorsAction()
{
	if(motorsStop || imageStream == nullptr)
	{
		motors->stopAll();
	}
	else
	{
		// directionMutex.lock();
		switch (currentDirection)
		{
		case AutoAction::LEFT:
				// directionMutex.unlock();
				motors->rightMotor(MOTOR_FORWARD);
				motors->leftMotor(MOTOR_BACKWARD);
			break;
		case AutoAction::RIGHT:
				// directionMutex.unlock();
				motors->leftMotor(MOTOR_FORWARD);
				motors->rightMotor(MOTOR_BACKWARD);
			break;
		case AutoAction::FORWARD:
				// directionMutex.unlock();
				motors->leftMotor(MOTOR_FORWARD);
				motors->rightMotor(MOTOR_FORWARD);
			break;
		}
		// directionMutex.unlock();
	}
}