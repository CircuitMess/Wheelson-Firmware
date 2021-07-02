#include "StreamServer.h"
#include <WiFi.h>
#include <esp_wifi_types.h>
#include <esp_wifi.h>

#define PART_BOUNDARY "123456789000000000000987654321"

static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

StreamServer* StreamServer::instance = nullptr;

StreamServer::StreamServer(){
	instance = this;
}

StreamServer::~StreamServer(){
	instance = nullptr;
}

void StreamServer::start(){
	IPAddress spencerIP(10, 0, 0, 1);
	esp_wifi_set_ps(WIFI_PS_NONE);
	WiFi.softAP("Wheelson Stream", "");
	WiFi.softAPConfig(spencerIP, spencerIP, IPAddress(255, 255, 255, 0));

	cam = new Camera();

	httpd_config_t config = HTTPD_DEFAULT_CONFIG();
	config.server_port = 80;

	httpd_uri_t index_uri = {
			.uri       = "/",
			.method    = HTTP_GET,
			.handler   = stream_handler,
			.user_ctx  = NULL
	};

	if (httpd_start(&httpd, &config) == ESP_OK) {
		httpd_register_uri_handler(httpd, &index_uri);
	}

	Serial.printf("StreamServer started, IP address %s\n", spencerIP.toString().c_str());
}

void StreamServer::stop(){
	httpd_stop(&httpd);

	delete cam;
	cam = nullptr;


	WiFi.mode(WIFI_OFF);
}

esp_err_t StreamServer::stream_handler(httpd_req_t* req){
	esp_err_t res = ESP_OK;
	char * part_buf[64];

	res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
	if(res != ESP_OK){
		return res;
	}

	for(;;){
		instance->cam->loadFrame();
		const camera_fb_t* frame = instance->cam->getFrame();
		size_t hlen = snprintf((char*) part_buf, 64, _STREAM_PART, frame->len);

		if(httpd_resp_send_chunk(req, (const char *)part_buf, hlen) != ESP_OK){
			instance->cam->releaseFrame();
			break;
		}

		if(httpd_resp_send_chunk(req, (const char*) frame->buf, frame->len) != ESP_OK){
			instance->cam->releaseFrame();
			break;
		}

		if(httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY)) != ESP_OK){
			instance->cam->releaseFrame();
			break;
		}

		instance->cam->releaseFrame();
	}
}
