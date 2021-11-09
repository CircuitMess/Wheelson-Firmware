#ifndef WHEELSON_FIRMWARE_RCDATACOM_H
#define WHEELSON_FIRMWARE_RCDATACOM_H

#include <Loop/LoopListener.h>
#include <BLEDevice.h>

class BLEClient;
class BLERemoteService;
class BLERemoteCharacteristic;
class BLEAdvertisedDevice;

struct RCInfo {
	bool direct = true;
	String ssid;
	String pass;
	uint8_t ip[4];
	uint8_t quality = 0;
};

class RCDataCom : public LoopListener, public BLEAdvertisedDeviceCallbacks {
public:
	RCDataCom();

	void start();
	void stop();

	bool isConnected();

	void loop(uint micros) override;

	void onResult(BLEAdvertisedDevice advertisedDevice) override;

	const RCInfo& fetchInfo();
	const RCInfo& getInfo();

private:
	void cleanup();
	bool connect();

	RCInfo info;

	uint32_t scanTime = 0;
	uint32_t holdTime = 0;

	BLEClient* client = nullptr;
	BLERemoteService* service = nullptr;
	BLERemoteCharacteristic* charDirect = nullptr;
	BLERemoteCharacteristic* charWifi = nullptr;
	BLERemoteCharacteristic* charIp = nullptr;
	BLERemoteCharacteristic* charQuality = nullptr;
	BLEAdvertisedDevice* device = nullptr;

};


#endif //WHEELSON_FIRMWARE_RCDATACOM_H
