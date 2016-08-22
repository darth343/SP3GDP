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
		NPC* n = new NPC(Vector3(32.f, 32.f, 1));
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
	if (SharedData::GetInstance()->gameState == SharedData::GAME_S1)
	{
		if (this->GetID() == 1)
		{
			if (this->GetAnimationState() == NPC::NPC_AWANDERING)
			{
				if (this->position.x > 300)
					this->position.x -= 30 * dt;
			}

			if (this->GetDialogueState() == 1)
				this->maxDia = 4;
			else if (this->GetDialogueState() == 2)
				this->maxDia = 3;
			this->maxState = 2;
		}
		if (this->GetID() == 2)
		{
			if (this->GetAnimationState() == NPC::NPC_AWANDERING)
			{
				if (this->position.y > 100)
					this->position.y -= 30 * dt;
			}

			if (this->GetDialogueState() == 1)
				this->maxDia = 5;
			else if (this->GetDialogueState() == 2)
				this->maxDia = 4;
			else if (this->GetDialogueState() == 3)
				this->maxDia = 4;
			this->maxState = 2;
		}
		if (this->GetID() == 3)
		{
			if (this->GetAnimationState() == NPC::NPC_AWANDERING)
			{
				if (this->position.y > 50)
					this->position.y -= 30 * dt;
			}

			if (this->GetDialogueState() == 1)
				this->maxDia = 2;
			else if (this->GetDialogueState() == 2)
				this->maxDia = 2;
			this->maxState = 2;
		}
	}
	if (this->active && CheckCollision(playerPos, mapOffset, m_cMap))
	{
		if (GetAnimationState() != NPC::NPC_ADYING)
		{
			SetAnimationState(NPC::NPC_AIDLE);
		}
		collisionDetected = true;
		if (Application::IsKeyPressed(VK_RETURN))
		{
			enterPressed = true;
			this->collideWithNPC = this->GetID();
		}
	}
	else 
	{
		enterPressed = false;
		collideWithNPC = 0;
		SetAnimationState(NPC::NPC_AWANDERING);
	}
}