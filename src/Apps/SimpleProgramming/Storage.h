#ifndef WHEELSON_FIRMWARE_STORAGE_H
#define WHEELSON_FIRMWARE_STORAGE_H

#include <Arduino.h>
#include "Action.hpp"
#include <SPIFFS.h>

namespace Simple {

struct ProgStruct{
	const Action* actions;
	uint8_t numActions;
};
class ProgStorage {
public:
	ProgStorage();
	~ProgStorage();
	uint8_t getNumProgs();
	const ProgStruct* getProg(uint8_t index);
	void addProg(const Action* actions, uint8_t numActions);
	void removeProg(uint8_t index);
	void updateProg(uint8_t index, const Action* actions, uint8_t numActions);

private:
	File progsFile;
	void readProgs();
	void writeProgs();

	std::vector<ProgStruct*> programs;
	uint8_t numProgs = 0;
};

}


#endif //WHEELSON_FIRMWARE_STORAGE_H
