#ifndef AUTOCAR_CAMERAFEED_H
#define AUTOCAR_CAMERAFEED_H

#include <Arduino.h>

class CameraFeed {
public:
	CameraFeed(uint width, uint height);

	void loadFrame();
	uint16_t* getBuffer() const;

	void toggleProcessFeed();

private:
	uint16_t* buffer;

	bool processFeed = true;

	void jpegToArray(uint16_t *buffer);
};


#endif //AUTOCAR_CAMERAFEED_H
