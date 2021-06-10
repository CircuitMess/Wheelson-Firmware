#include "CameraFeed.h"

#include <esp_camera.h>
#include <JPEGDecoder.h>
#include "../Wheelson.h"

CameraFeed::CameraFeed() : buffer((uint16_t*) ps_malloc(160 * 120 * sizeof(uint16_t))), bufferRGB(static_cast<uint8_t*>(ps_malloc(160 * 120 * 3))){

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
	config.pin_pwdn = PWDN_GPIO_NUM;
	config.pin_reset = RESET_GPIO_NUM;
	config.xclk_freq_hz = 20000000;
	config.pixel_format = PIXFORMAT_JPEG;

	config.frame_size = FRAMESIZE_QQVGA;
	config.jpeg_quality = 1;
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

CameraFeed::~CameraFeed(){
	free(buffer);
}

void CameraFeed::loadFrame(){
	frame = esp_camera_fb_get();
	if(!frame){
		Serial.println("Camera capture failed");
		return;
	}

}

void CameraFeed::decodeFrame(){
	JpegDec.decodeArray(frame->buf, frame->len);
	memcpy(bufferRGB, JpegDec.pImage, 160 * 120 * 3);
	jpegToArray(buffer);
}

void CameraFeed::releaseFrame(){
	esp_camera_fb_return(frame);
}

void CameraFeed::jpegToArray(uint16_t* buffer){
	uint16_t* pImg;
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

	if(win_w == 0 || win_h == 0){
		printf("decode error\n");
		return;
	}

	// save the coordinate of the right and bottom edges to assist image cropping
	// to the screen size
	// read each MCU block until there are no more
	while(JpegDec.readSwappedBytes()){ // Swap byte order so the SPI buffer can be used

		// save a pointer to the image block
		pImg = JpegDec.pImage;

		// calculate where the image block should be drawn on the screen
		int mcu_x = JpegDec.MCUx * mcu_w; // Calculate coordinates of top left corner of current MCU
		int mcu_y = JpegDec.MCUy * mcu_h;

		// check if the image block size needs to be changed for the right edge
		if(mcu_x + mcu_w <= max_x)
			win_w = mcu_w;
		else
			win_w = min_w;

		// check if the image block size needs to be changed for the bottom edge
		if(mcu_y + mcu_h <= max_y)
			win_h = mcu_h;
		else
			win_h = min_h;

		// copy pixels into a contiguous block
		if(win_w != mcu_w){
			uint16_t* cImg;
			int p = 0;
			cImg = pImg + win_w;
			for(int h = 1; h < win_h; h++){
				p += mcu_w;
				for(int w = 0; w < win_w; w++){
					*cImg = *(pImg + w + p);
					cImg++;
				}
			}
		}

		// draw image MCU block only if it will fit on the screen
		for(uint32_t x = 0; x < mcu_w; x++){
			for(uint32_t y = 0; y < mcu_h; y++){
				buffer[max_x * (mcu_y + y) + mcu_x + x] = pImg[y * mcu_w + x] << 8 | pImg[y * mcu_w + x] >> 8;
			}
		}
	}
}

uint16_t* CameraFeed::getRaw() const{
	return buffer;
}

camera_fb_t* CameraFeed::getFrame(){
	return frame;
}

uint8_t* CameraFeed::getRawRGB() const{
	return bufferRGB;
}
