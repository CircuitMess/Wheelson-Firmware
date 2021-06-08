#ifndef AUTOCAR_CAMERAFEED_H
#define AUTOCAR_CAMERAFEED_H

#include <Arduino.h>
#include <esp_camera.h>

class CameraFeed {
public:
	CameraFeed();

	/**
	 * Load a frame from the camera.
	 */
	void loadFrame();

	/**
	 * Release last frame.
	 */
	void releaseFrame();

	/**
	 * Get a raw RGB565 frame buffer size 160 x 120
	 */
	uint16_t* getRaw() const;

	uint8_t* getRawRGB() const;

	/**
	 * Get the raw camera frame.
	 */
	camera_fb_t* getFrame();
	virtual ~CameraFeed();


private:
	camera_fb_t* frame;
	uint16_t* buffer;
	uint8_t* bufferRGB;

	void jpegToArray(uint16_t *buffer);
};


#endif //AUTOCAR_CAMERAFEED_H
