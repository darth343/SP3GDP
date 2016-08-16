#include "NPC.h"
#include <fstream>
using std::ifstream;
#include <iostream>
using std::iostream;

NPC::NPC(Vector3 scale) : GameObject(scale)
{
	npcDialogueState = NPC_DNOSTATE;
	npcAnimationState = NPC_ANOSTATE;
	size = 0;
}
NPC::~NPC()
{
}

//Text file is going to include NPC id, NPC animation state, NPC dialogue state, NPC dialogue
bool NPC::ReadFromFile(string filename, vector<GameObject*>&m_goList)
{
	ifstream myfile;
	myfile.open(filename);
	string temp;
	while (!myfile.eof())
	{
		NPC* n = new NPC(Vector3(32.f, 32.f, 1));

		n->type = GameObject::GO_NPC;
		getline(myfile, temp, ',');
		n->SetID(temp);
		getline(myfile, temp, ',');
		if (temp == "1")
			n->SetAnimationState(NPC::NPC_AIDLE);
		if (temp == "2")
			n->SetAnimationState(NPC::NPC_AWANDERING);
		if (temp == "3")
			n->SetAnimationState(NPC::NPC_ADYING);
		if (temp == "4")
			n->SetAnimationState(NPC::NPC_ACRYING);

		getline(myfile, temp, ',');
		if (temp == "1")
			n->SetDialogueState(NPC::NPC_DSTORY1);
		if (temp == "2")
			n->SetDialogueState(NPC::NPC_DSTORY2);
		if (temp == "")
			break;
		//getline(myfile, temp, ',');
		//	cout << stoi(temp);
		////	n->position.x = stoi(temp);
		////n->position.x = stoi(x);
		//getline(myfile, temp, ',');
		//cout << temp;
		////temp = stoi(temp);
		//n->position.z = 1;
		getline(myfile, temp, '\n');
		n->SetDialogue(temp);
		m_goList.push_back(n);

	}
	myfile.close();
	return true;
}

void NPC::SetID(string id)
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
string NPC::GetID()
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
void NPC::Update(double dt, Vector3 playerPos, Vector3 mapOffset, CMap* m_cMap)
{
	if (this->active && CheckCollision(playerPos, mapOffset, m_cMap))
	{
		if (npcID == "3")
		{
			//cout << "HI";
			SetAnimationState(NPC::	NPC_AWANDERING);
			cout <<"3" << GetAnimationState()<<endl;
		}
		if (npcID == "1")
		{
			SetAnimationState(NPC::NPC_ADYING);
			cout << "1" << GetAnimationState() << endl;
		}
		if (npcID == "2")
		{
			SetAnimationState(NPC::NPC_AIDLE);
			cout << "2" << GetAnimationState() << endl;
		}
	}
}