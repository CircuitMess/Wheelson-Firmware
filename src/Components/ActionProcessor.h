#ifndef AUTOCAR_ACTIONPROCESSOR_H
#define AUTOCAR_ACTIONPROCESSOR_H

#include <Update/UpdateListener.h>
#include <Sync/Queue.h>
#include <Util/Task.h>
#include "Motors.h"
#include "AutoAction.h"

/**
 * Plan akcije:
 *
 * Dok se primi neka akcija koja ima trajanje, npr. drive, ta akcija se pokrene (motori se startaju), thread se delaya,
 * nakon čega se ackija zaustavlja (motori se zaustave). Ako se obrađuje akcija koja ima trajanje, i trajanje je 0, to
 * znači da akcija traje tak dugo dok se ne dobi sljedeća akcija. U stopCurrentAction bi se trablo riješit zaustavljanje
 * trenutne akcije, i onda se ta funkcija pozivat i kod akcija koje imaju trajanje (start motori -> delay -> stopCurrentAction()).
 *
 */

class ActionProcessor {
public:
	ActionProcessor();
	void start();
	void stop();

	/**
	 * Add a new action to the processing queue. The action object and it's params will be deleted after processing.
	 * @param action
	 */
	void addAction(AutoAction* action);

	static void actionProcessorTask(Task* task);

private:
	static ActionProcessor* instance;

	Task actionTask;
	Queue actionQueue;

	AutoAction* currentAction = nullptr;

	void processAction();
	void stopCurrentAction();
};


#endif //AUTOCAR_ACTIONPROCESSOR_H
