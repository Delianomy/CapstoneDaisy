#ifndef DIALOGUE_SEQUENCE_H
#define DIALOGUE_SEQUENCE_H
#include <vector>
#include "Dialogue.h"
class DialogueSequence
{
public:
	std::vector<Dialogue> dialogues;
	Dialogue currentDialogue;

	
	void NextDialogue();
	void OnEndSequence();
};
#endif // !DIALOGUE_SEQUENCE_H

