#include "RCDataCom.h"
#include <Wheelson.h>
#include <Loop/LoopManager.h>

#define SERVICE_UUID      "d8771990-683b-4bc7-9651-8601be142e94"
#define CHAR_DIRECT_UUID  "8084b55f-d8b3-46c5-acaa-5c99809ed448"
#define CHAR_WIFI_UUID    "bc137c7c-408b-402d-ae4d-009d6e008bcf"
#define CHAR_IP_UUID      "a8a92cf9-3c1a-48c9-8079-29a0a334b879"
#define CHAR_QUALITY_UUID "7d9917e0-aeee-44c4-958e-684b08dd76e2"

RCDataCom::RCDataCom(){

}

void RCDataCom::onResult(BLEAdvertisedDevice advertisedDevice){
	if(advertisedDevice.getName() != "Wheelson RC" && !advertisedDevice.isAdvertisingService(BLEUUID(SERVICE_UUID))) return;

	BLEDevice::getScan()->stop();
	cleanup();

	device = new BLEAdvertisedDevice(advertisedDevice);
}

void RCDataCom::loop(uint micros){
	if(holdTime){
		if(millis() - holdTime >= 3000){
			holdTime = 0;
			start();
		}

		return;
	}

	if(device && !client){
		if(connect()){
			LoopManager::removeListener(this);
		}else{
			holdTime = millis();
		}

		return;
	}

	if(!device && millis() - scanTime >= 5500){
		start();
	}
}

bool RCDataCom::connect(){
	if(!device) return false;

	client = BLEDevice::createClient();

	if(!client->connect(device)){
		printf("Failed connecting to device\n");
		cleanup();
		return false;
	}

	service = client->getService(SERVICE_UUID);
	if(service == nullptr){
		printf("Wrong service\n");
		cleanup();
		return false;
	}

	charDirect = service->getCharacteristic(CHAR_DIRECT_UUID);
	charWifi = service->getCharacteristic(CHAR_WIFI_UUID);
	charIp = service->getCharacteristic(CHAR_IP_UUID);
	charQuality = service->getCharacteristic(CHAR_QUALITY_UUID);
	if(!charDirect || !charWifi || !charIp){
		printf("Wrong characteristic\n");
		cleanup();
		return false;
	}

	return true;
}

const RCInfo& RCDataCom::fetchInfo(){
	if(!charDirect || !charWifi || !charIp) return info;

	info.direct = charDirect->readUInt8();
	info.quality = charQuality->readUInt8();
	info.quality = constrain(info.quality, 0, 2);

	uint32_t ip = charIp->readUInt32();
	memcpy(info.ip, &ip, sizeof(info.ip));

	String ssidPass = charWifi->readValue().c_str();

	// poor man's encryption
	char key[] = "abc";
	for(int i = 0, j = 0; i < ssidPass.length(); i++, j = (j+1) % sizeof(key)){
		ssidPass[i] = ssidPass[i] ^ key[j];
	}

	int index = ssidPass.indexOf(";:");
	info.ssid = ssidPass.substring(0, index);
	info.pass = ssidPass.substring(index+2);

	printf("Direct: %s, Quality: %d\n", info.direct ? "Y" : "N", info.quality);
	printf("SSID: %s, pass: %s\n", info.ssid.c_str(), info.pass.c_str());
	printf("IP: %d.%d.%d.%d\n", info.ip[0], info.ip[1], info.ip[2], info.ip[3]);

	return info;
}

const RCInfo& RCDataCom::getInfo(){
	return info;
}

void RCDataCom::start(){
	stop();

	if(!BLEDevice::getInitialized()){
		BLEDevice::init("Wheelson");
	}

	cleanup();

	BLEScan* scan = BLEDevice::getScan();
	scan->stop();

	scan->setAdvertisedDeviceCallbacks(this);
	scan->setActiveScan(true);
	scan->start(5);

	LoopManager::addListener(this);

	scanTime = millis();
}

void RCDataCom::stop(){
	LoopManager::removeListener(this);

	if(BLEDevice::getInitialized()){
		BLEScan* scan = BLEDevice::getScan();
		scan->stop();
	}

	cleanup();
}

void RCDataCom::cleanup(){
	if(!BLEDevice::getInitialized()) return;

	if(client){
		client->disconnect();
	}

	delete device;
	delete client; // service gets deleted with client, characteristics get deleted with service

	charDirect = charWifi = charIp = nullptr;
	service = nullptr;
	device = nullptr;
	client = nullptr;
}

bool RCDataCom::isConnected(){
	return charDirect && charWifi && charIp;
}