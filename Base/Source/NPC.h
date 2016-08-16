#ifndef NPC_H
#define NPC_H
#include <string>

using std::string;

class NPC
{
public:
	NPC();
	~NPC();
	enum NPC_ANIMATION
	{
		NPC_ANOSTATE,
		NPC_AIDLE,
		NPC_AWANDERING,
		NPC_ADYING,
		NPC_ACRYING,
		NPC_ATOTAL,
	};
	enum NPC_DIALOGUE
	{
		NPC_DNOSTATE,
		NPC_DSTORY1,
		NPC_DSTORY2,
		NPC_DTOTAL,
	};

	void ReadFromFile(string);
	void SetDialogue(string);
	void Update(float);
	void SetAnimationState(NPC_ANIMATION);
	void SetDialogueState(NPC_DIALOGUE);
	void SetID(int);
	int GetID();
	string GetDialogue();
	NPC_ANIMATION GetAnimationState();
	NPC_DIALOGUE GetDialogueState();

private:
	int npcID;
	string npcDialogue;
	NPC_ANIMATION npcAnimationState;
	NPC_DIALOGUE npcDialogueState;
};

#endif // !NPC_H
