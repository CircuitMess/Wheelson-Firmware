#include "Storage.h"
#include <SPIFFS.h>
const char* const Simple::Storage::filePath ="/SimpleProgs.bin";

Simple::Storage::Storage(){
	readProgs();
}

Simple::Storage::~Storage(){
	for( ProgStruct* prog: programs){
		free((void*)prog->actions);
		delete prog;
	}
}

uint8_t Simple::Storage::getNumProgs(){
	return numProgs;
}

const Simple::ProgStruct *Simple::Storage::getProg(uint8_t index){
	if(index >= numProgs) return nullptr;
	return programs[index];
}

void Simple::Storage::addProg(const Simple::Action *actions, uint8_t numActions){
	if(numProgs == 0xFF) return;

	Action* newActions = (Action*)malloc(numActions*sizeof(Action));
	memcpy(newActions, actions, numActions*sizeof(Action));
	programs.push_back(new ProgStruct{newActions, numActions});
	writeProgs();
}

void Simple::Storage::removeProg(uint8_t index){
	if(index >= numProgs) return;

	free((void*)programs[index]->actions);
	delete programs[index];
	programs.erase(programs.begin() + index);
	writeProgs();
}

void Simple::Storage::updateProg(uint8_t index, const Simple::Action *actions, uint8_t numActions){
	if(index >= numProgs) return;

	free((void*)programs[index]->actions);
	Action* newActions = (Action*)malloc(numActions*sizeof(Action));
	memcpy(newActions, actions, numActions*sizeof(Action));
	programs[index]->actions = newActions;
	programs[index]->numActions = numActions;
	writeProgs();
}

void Simple::Storage::readProgs(){
	File progsFile = SPIFFS.open(filePath, "r");
	if(!progsFile){
		Serial.printf("Error opening SimpleProgs.bin\n");
		return;
	}
	progsFile.read(&numProgs, 1);
	for(uint8_t i = 0; i < numProgs; i++){
		ProgStruct* prog = new ProgStruct();
		progsFile.read(&prog->numActions, 1);
		prog->actions = (Action*)malloc(prog->numActions*sizeof(Action));
		progsFile.read((uint8_t*)prog->actions, prog->numActions*sizeof(Action));
		programs.push_back(prog);
	}
	progsFile.close();
}

void Simple::Storage::writeProgs(){
	File progsFile = SPIFFS.open(filePath, "w");
	if(!progsFile){
		Serial.printf("Error opening SimpleProgs.bin\n");
		return;
	}
	progsFile.write(numProgs);
	for(uint8_t i = 0; i < numProgs; i++){
		ProgStruct *prog = programs[i];
		progsFile.write(prog->numActions);
		progsFile.write((uint8_t*)prog->actions, sizeof(Action)*prog->numActions);
	}
	progsFile.close();
}
