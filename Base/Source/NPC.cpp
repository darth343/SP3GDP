#include "NPC.h"
#include <fstream>
using std::ifstream;
#include "Application.h"
#include "SharedData.h"
#include <iostream>
using std::iostream;

NPC::NPC(Vector3 scale) : GameObject(scale)
{
	npcDialogueState = NPC_DNOSTATE;
	npcAnimationState = NPC_ANOSTATE;
	npcID = 0;
}
NPC::~NPC()
{
	while (npcs.size() > 0)
	{
		NPC *go = npcs.back();
		delete go;
		npcs.pop_back();
	}
}
void NPC::SetState(int &state)
{
	this->currState = state;
}
//Text file is going to include NPC id, NPC animation state, NPC dialogue state, NPC dialogue
void NPC::ReadFromFile(string filename, vector<GameObject*>&m_goList)
{
	ifstream myfile;
	myfile.open(filename);
	string temp;
	while (myfile.good())
	{
		NPC* n = new NPC(Vector3(27.f, 32.f, 1));
		n->type = GameObject::GO_NPC;
		getline(myfile, temp, '`');
		if (!temp.empty())
		n->SetID(stoi(temp));
		getline(myfile, temp, '`');
		if (temp == "1")
			n->SetAnimationState(NPC::NPC_AIDLE);
		if (temp == "2")
			n->SetAnimationState(NPC::NPC_AWANDERING);
		if (temp == "3")
			n->SetAnimationState(NPC::NPC_ADYING);
		if (temp == "4")
			n->SetAnimationState(NPC::NPC_ACRYING);
		getline(myfile, temp, '`');
		if (temp == "1")
			n->SetDialogueState(NPC::NPC_DSTORY1);
		if (temp == "2")
			n->SetDialogueState(NPC::NPC_DSTORY2);
		if (temp == "3")
			n->SetDialogueState(NPC::NPC_DSTORY3);
		
		getline(myfile, temp,'`');
		n->SetDialogue(temp);
		getline(myfile, temp);
		n->num = stoi(temp);
		npcs.push_back(n);
		if (myfile.eof())
		break;
	}
	myfile.close();
}
vector<NPC*> NPC::GetVec()
{
	return npcs;
}
int NPC::GetNum()
{
	return num;
}
int NPC::collideWhichNPC()
{
	return collideWithNPC;
}
void NPC::SetID(float id)
{
	npcID = id;
	if (id == 1)
	{
		this->moveLeft = true;
		this->moveRight = false;
	}
	if (id == 2)
	{
		this->moveLeft = false;
		this->moveRight = true;
	}
	if (id == 3)
	{
		this->moveLeft = false;
		this->moveRight = true;
	}

}
void NPC::SetDialogue(string dialogue)
{
	npcDialogue = dialogue;
}
void NPC::SetDialogueState(NPC::NPC_DIALOGUE dialogueState)
{
	npcDialogueState = dialogueState;
}
void NPC::SetAnimationState(NPC::NPC_ANIMATION animationState)
{
	npcAnimationState = animationState;
}
int NPC::GetID()
{
	return npcID;
}
string NPC::GetDialogue()
{
	return npcDialogue;
}
NPC::NPC_DIALOGUE NPC::GetDialogueState()
{
	return npcDialogueState;
}
NPC::NPC_ANIMATION NPC::GetAnimationState()
{
	return npcAnimationState;
}
void NPC::ScrollDialogue(int & dialogue)
{
	dialogue++;
}
void NPC::Update(double dt, Vector3 playerPos, Vector3 mapOffset, CMap* m_cMap)
{
	//MOVEMENT
		if (GetID() == 1)
		{
			if (this->GetAnimationState() == NPC::NPC_AWANDERING)
			{
				if (this->moveLeft)
				{
					this->position.x -= 30 * dt;
					if (this->position.x < 300)
					{
						moveRight = true;
						moveLeft = false;
					}
				}
				if (this->moveRight)
				{
					this->position.x += 30 * dt;
					if (this->position.x > 500)
					{
						moveLeft = true;
						moveRight = false;
					}
				}
			}

			if (this->GetDialogueState() == 1)
				this->maxDia = 3;
			else if (this->GetDialogueState() == 2)
				this->maxDia = 3;
		}
		if (GetID() == 2)
		{
			if (this->GetAnimationState() == NPC::NPC_AWANDERING)
			{
				if (moveLeft)
				{
					this->position.x -= 40 * dt;
					if (this->position.x < 80)
					{
						moveLeft = false;
						moveRight = true;
					}
				}
				if (this->moveRight)
				{
					this->position.x += 40 * dt;
					if (this->position.x > 200)
					{
						moveLeft = true;
						moveRight = false;
					}
				}
			}

			if (this->GetDialogueState() == 1)
				this->maxDia = 5;
			else if (this->GetDialogueState() == 2)
				this->maxDia = 4;
			else if (this->GetDialogueState() == 3)
				this->maxDia = 4;
		}
		if (GetID() == 3)
		{
			if (this->GetAnimationState() == NPC::NPC_AWANDERING)
			{
				if (moveLeft)
				{
					this->position.x -= 40 * dt;
					if (this->position.x < 80)
					{
						moveLeft = false;
						moveRight = true;
					}
				}
				if (this->moveRight)
				{
					this->position.x += 40 * dt;
					if (this->position.x > 200)
					{
						moveLeft = true;
						moveRight = false;
					}
				}
			}
			if (this->GetDialogueState() == 1)
				this->maxDia = 2;
			else if (this->GetDialogueState() == 2)
				this->maxDia = 2;
		}
	if (CheckCollision(playerPos, mapOffset, m_cMap))
	{
		if (GetAnimationState() != NPC::NPC_ADYING)
		{
			SetAnimationState(NPC::NPC_AIDLE);
		}
		collisionDetected = true;
		this->collideWithNPC = this->GetID();
	}
	else 
	{
		enterPressed = false;
		collideWithNPC = 0;
		SetAnimationState(NPC::NPC_AWANDERING);
	}
}
bool NPC::GetMoveLeft()
{
	return this->moveLeft;
}
bool NPC::GetMoveRight()
{
	return this->moveRight;
}