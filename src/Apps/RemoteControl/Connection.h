#ifndef WHEELSONRC_BYTEBOI_CONNECTION_H
#define WHEELSONRC_BYTEBOI_CONNECTION_H


#include <WiFiServer.h>
#include <Loop/LoopListener.h>
#include "RCDataCom.h"

class ConnectionListener {
public:
	virtual void connected() = 0;
	virtual void disconnected() = 0;
};

class Connection : public LoopListener {
public:
	Connection();
	virtual ~Connection();

	void start();
	void stop();
	WiFiClient& getFeedClient();
	WiFiClient& getControlClient();
	void setListener(ConnectionListener *listener);
	bool connected();

	enum State { IDLE, FETCHING, WIFI, CONNECTING, CONNECTED };
	State getState() const;

	void disconnected();
	void loop(uint micros) override;

	const RCInfo& getInfo();

private:
	RCDataCom rcData;
	void startWifi();

	State state = IDLE;

	IPAddress controllerIP;
	WiFiClient feedClient;
	WiFiClient controlClient;

	IPAddress localIP;
	IPAddress gateway;
	IPAddress subnet;

	uint32_t wifiTime = 0;
	bool wifiConnected = false;

	ConnectionListener* listener = nullptr;
};

#endif //WHEELSONRC_BYTEBOI_CONNECTION_H
