#include <WiFi.h>
#include <Loop/LoopManager.h>
#include "Connection.h"

Connection::Connection() : localIP(10, 0, 0, 12),
						   gateway(10, 0, 0, 1),
						   subnet(10, 0, 0, 0){


}

Connection::~Connection(){
	stop();
}

void Connection::start(){
	state = FETCHING;
	rcData.start();

	LoopManager::addListener(this);
}

void Connection::stop(){
	rcData.stop();

	feedClient.stop();
	controlClient.stop();

	state = IDLE;

	wifiTime = 0;
	WiFi.disconnect();
	wifiConnected = false;

	LoopManager::removeListener(this);
}

void Connection::loop(uint micros){
	if(connected()){
		LoopManager::removeListener(this);
		return;
	}

	if(state == FETCHING){
		if(!rcData.isConnected()) return;

		startWifi();
		return;
	}else if(state == WIFI){
		if(WiFi.status() != WL_CONNECTED){
			if(millis() - wifiTime >= 5000){
				printf("reset wifi\n");
				WiFi.disconnect();
				wifiConnected = false;
				delay(500);
				startWifi();
			}

			return;
		}else if(!wifiConnected){
			printf("connected to wifi\n");
			wifiConnected = true;
			state = CONNECTING;
			delay(500);
			wifiTime = millis();
			return;
		}
	}else if(state == CONNECTING){
		if(!connected()){
			if(millis() - wifiTime > 5000){
				printf("reset wifi 2\n");
				feedClient.stop();
				controlClient.stop();
				WiFi.disconnect();
				wifiConnected = false;
				delay(500);
				startWifi();
			}
		}

		if(!feedClient.connected() && !feedClient.connect(controllerIP, 5000)) return;
		if(!controlClient.connected() && !controlClient.connect(controllerIP, 5001)) return;
	}

	if(connected()){
		printf("connected\n");
		wifiTime = 0;
		rcData.stop();
		state = CONNECTED;
		LoopManager::removeListener(this);

		if(listener){
			listener->connected();
		}
	}
}

void Connection::startWifi(){
	const RCInfo info = rcData.fetchInfo();
	controllerIP = IPAddress(info.ip);

	if(info.direct){
		localIP = IPAddress(10, 0, 0, rand() % 256); // poor man's dhcp
	}

	localIP.toString();

	WiFi.begin(info.ssid.c_str(), info.pass.c_str());
	wifiTime = millis();
	wifiConnected = false;

	state = WIFI;
}

void Connection::disconnected(){
	stop();

	if(listener){
		listener->disconnected();
	}
}

void Connection::setListener(ConnectionListener *listener){
	Connection::listener = listener;
}

bool Connection::connected(){
	return feedClient.connected() && controlClient.connected();
}

WiFiClient& Connection::getFeedClient(){
	return feedClient;
}

WiFiClient& Connection::getControlClient(){
	return controlClient;
}

Connection::State Connection::getState() const{
	return state;
}

const RCInfo& Connection::getInfo(){
	return rcData.getInfo();
}
