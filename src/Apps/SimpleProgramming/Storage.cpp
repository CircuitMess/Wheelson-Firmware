#include "Storage.h"
#include <SPIFFS.h>
const char* const Simple::Storage::filePath ="/SimpleProgs.bin";

Simple::Storage::Storage(){
	if(SPIFFS.exists(filePath)){
		readProgs();
	}else{
		writeProgs();
	}
}

Simple::Storage::~Storage(){
	for( Program* prog: programs){
		free((void*)prog->actions);
		delete prog;
	}
}

uint8_t Simple::Storage::getNumProgs(){
	return programs.size();
}

const Simple::Program *Simple::Storage::getProg(uint8_t index){
	if(index >= programs.size()) return nullptr;
	return programs[index];
}

void Simple::Storage::addProg(const Simple::Action *actions, uint8_t numActions){
	if(programs.size() == 0xFF) return;

	Action* newActions = (Action*)malloc(numActions*sizeof(Action));
	memcpy(newActions, actions, numActions*sizeof(Action));
	programs.push_back(new Program{ newActions, numActions});
	writeProgs();
}

void Simple::Storage::removeProg(uint8_t index){
	if(index >= programs.size()) return;

	free((void*)programs[index]->actions);
	delete programs[index];
	programs.erase(programs.begin() + index);
	writeProgs();
}

void Simple::Storage::updateProg(uint8_t index, const Simple::Action *actions, uint8_t numActions){
	if(index >= programs.size()) return;

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
	uint8_t numProgs;
	progsFile.read(&numProgs, 1);
	for(uint8_t i = 0; i < numProgs; i++){
		Program* prog = new Program();
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
	uint8_t numProgs = programs.size();
	progsFile.write(numProgs);
	for(uint8_t i = 0; i < numProgs; i++){
		Program *prog = programs[i];
		progsFile.write(&prog->numActions, 1);
		progsFile.write((uint8_t*)prog->actions, sizeof(Action)*prog->numActions);
	}
	progsFile.close();
}
