#ifndef NPC_H
#define NPC_H
#include <string>
#include <vector>
#include "Vector3.h"
#include "GameObject.h"

using std::vector;
using std::string;

class NPC : public GameObject
{
public:
	NPC(Vector3);
	NPC()
	{}
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

	void ReadFromFile(string,vector<GameObject*> &);
	void SetDialogue(string);
	void Update(double dt, Vector3 playerPos, Vector3 mapOffset, CMap* m_cMap);
	void SetAnimationState(NPC_ANIMATION);
	void SetDialogueState(NPC_DIALOGUE);
	void SetID(float);
	vector<NPC*>GetVec();
	int GetID();
	string GetDialogue();
	int GetNum();
	void ScrollDialogue(int&);
	int collideWhichNPC();
	void SetState(int&);
	bool collisionDetected;
	
	bool enterPressed;
	int currState = 1;
	int dialogueNumber;
	NPC_ANIMATION GetAnimationState();
	NPC_DIALOGUE GetDialogueState();

private:
	float npcID;
	int collideWithNPC;
	int num;
	bool npc1;
	bool npc2;
	bool npc3;
	bool emptyString = false;
	string npcDialogue;
	vector<NPC*>npcs;
	NPC_ANIMATION npcAnimationState;
	NPC_DIALOGUE npcDialogueState;
};

#endif // !NPC_H
