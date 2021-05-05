#ifndef WHEELSON_FIRMWARE_STREAMSERVER_H
#define WHEELSON_FIRMWARE_STREAMSERVER_H


#include <Loop/LoopListener.h>
#include "CameraFeed.h"
#include <esp_http_server.h>

class StreamServer {
public:
	StreamServer();

	void start();
	void stop();

	static esp_err_t stream_handler(httpd_req_t *req);
	virtual ~StreamServer();

private:
	static StreamServer* instance;

	CameraFeed* cam = nullptr;
	httpd_handle_t httpd = nullptr;

};

#endif //WHEELSON_FIRMWARE_STREAMSERVER_H
