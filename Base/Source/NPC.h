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
		NPC_DSTORY3,
		NPC_DTOTAL,
	};

	static void ReadFromFile(string, vector<NPC*>&npcs);
	void SetDialogue(string);
	void Update(double dt, Vector3 playerPos, Vector3 mapOffset, CMap* m_cMap);
	void SetAnimationState(NPC_ANIMATION);
	void SetDialogueState(NPC_DIALOGUE);
	void SetID(float);
	bool GetMoveLeft();
	bool GetMoveRight();
	int GetID();
	string GetDialogue();
	int GetNum();
	void ScrollDialogue(int&);
	int collideWhichNPC();
	void SetState(int&);
	bool collisionDetected = false;
	
	bool enterPressed;
	int currState = 1;
	int dialogueNumber;
	NPC_ANIMATION GetAnimationState();
	NPC_DIALOGUE GetDialogueState();
private:
	float npcID;
	int maxState = 1;

	int collideWithNPC;
	int num;
	bool moveLeft;
	bool moveRight;
	string npcDialogue;
	NPC_ANIMATION npcAnimationState;
	NPC_DIALOGUE npcDialogueState;
};

#endif // !NPC_H
