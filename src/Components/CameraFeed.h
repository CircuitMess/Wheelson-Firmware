#ifndef AUTOCAR_CAMERAFEED_H
#define AUTOCAR_CAMERAFEED_H

#include <Arduino.h>
#include <esp_camera.h>

class CameraFeed {
public:
	CameraFeed(uint width, uint height);

	void loadFrame();
	void releaseFrame();
	uint16_t* getRaw() const;
	camera_fb_t* getFrame();


private:
	camera_fb_t* frame;
	uint16_t* buffer;

	void jpegToArray(uint16_t *buffer);
};


#endif //AUTOCAR_CAMERAFEED_H
