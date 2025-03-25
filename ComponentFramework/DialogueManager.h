#ifndef DIALOGUE_MANAGER_H
#define DIALOGUE_MANAGER_H

#include "DialogueSequence.h"


class DialogueManager
{
	DialogueSequence currentSSequence;
public:
	void Render();
	void Next();
};

#endif // !DIALOGUE_MANAGER_H