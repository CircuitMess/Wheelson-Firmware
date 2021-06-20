#ifndef WHEELSON_FIRMWARE_STORAGE_H
#define WHEELSON_FIRMWARE_STORAGE_H

#include <Arduino.h>
#include <vector>
#include "Action.hpp"

namespace Simple {

struct Program{
	const Action* actions;
	uint8_t numActions;
	uint8_t id;
};
class Storage {
public:
	Storage();
	~Storage();
	uint8_t getNumProgs();
	const Program* getProg(uint8_t index);
	void addProg(const Action* actions, uint8_t numActions);
	void removeProg(uint8_t index);
	void updateProg(uint8_t index, const Action* actions, uint8_t numActions);

private:
	void readProgs();
	void writeProgs();

	std::vector<Program*> programs;
	static const char* const filePath;
};

}


#endif //WHEELSON_FIRMWARE_STORAGE_H
