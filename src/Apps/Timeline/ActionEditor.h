#ifndef AUTOCAR_ACTIONEDITOR_H
#define AUTOCAR_ACTIONEDITOR_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Support/Modal.h>
#include <Elements/GridMenu.h>
#include "../../Elements/Fleha.hpp"

class PatternEditor;

class ActionEditor : public Modal {
public:
	explicit ActionEditor(PatternEditor* editor);

	void draw() override;
	void start() override;
	void stop() override;

private:
	static ActionEditor* instance;

	PatternEditor* editor = nullptr;

	Fleha fleha;

	void fillMenu();
	void buildUI();
};


#endif //AUTOCAR_ACTIONEDITOR_H
