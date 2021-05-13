#include "Storage.h"

Simple::ProgStorage::ProgStorage(){
	readProgs();
}

Simple::ProgStorage::~ProgStorage(){
	for( ProgStruct* prog: programs){
		free((void*)prog->actions);
		delete prog;
	}
}

uint8_t Simple::ProgStorage::getNumProgs(){
	return numProgs;
}

const Simple::ProgStruct *Simple::ProgStorage::getProg(uint8_t index){
	if(index >= numProgs) return nullptr;
	return programs[index];
}

void Simple::ProgStorage::addProg(const Simple::Action *actions, uint8_t numActions){
	programs.push_back(new ProgStruct{actions, numActions});
	writeProgs();
}

void Simple::ProgStorage::removeProg(uint8_t index){
	if(index >= numProgs) return;
	free((void*)programs[index]->actions);
	delete programs[index];
	programs.erase(programs.begin() + index);
	writeProgs();
}

void Simple::ProgStorage::updateProg(uint8_t index, const Simple::Action *actions, uint8_t numActions){
	if(index >= numProgs) return;

	free((void*)programs[index]->actions);
	programs[index]->actions = actions;
	programs[index]->numActions = numActions;
	writeProgs();
}

void Simple::ProgStorage::readProgs(){
	progsFile = SPIFFS.open("/SimpleProgs.bin", "r");
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

void Simple::ProgStorage::writeProgs(){
	progsFile = SPIFFS.open("/SimpleProgs.bin", "w");
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
