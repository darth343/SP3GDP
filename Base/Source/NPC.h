#ifndef NPC_H
#define NPC_H
#include <string>
#include <vector>

using std::vector;
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

	bool ReadFromFile(string);
	void SetDialogue(string);
	int GetTextSize(string);
	void Update(float);
	void SetAnimationState(NPC_ANIMATION);
	void SetDialogueState(NPC_DIALOGUE);
	void SetID(string);
	string GetID();
	vector<NPC> GetNPCVec();
	string GetDialogue();
	NPC_ANIMATION GetAnimationState();
	NPC_DIALOGUE GetDialogueState();

private:
	string npcID;
	int size;
	string npcDialogue;
	vector<NPC>npcs;
	NPC_ANIMATION npcAnimationState;
	NPC_DIALOGUE npcDialogueState;
};

#endif // !NPC_H
