#include "NPC.h"
#include <fstream>
using std::ifstream;
#include <iostream>
using std::iostream;

NPC::NPC()
{
	npcDialogueState = NPC_DNOSTATE;
	npcAnimationState = NPC_ANOSTATE;
	size = 0;
}
NPC::~NPC()
{
}
void NPC::Update(float dt)
{

}
//Text file is going to include NPC id, NPC animation state, NPC dialogue state, NPC dialogue
bool NPC::ReadFromFile(string filename)
{
	ifstream myfile;
	myfile.open(filename);
	string temp;
	NPC n;
	while (!myfile.eof())
	{
		getline(myfile, temp, ',');
		n.SetID(temp);

		getline(myfile, temp, ',');
		if (temp == "1")
			n.SetAnimationState(NPC::NPC_AIDLE);
		if (temp == "2")
			n.SetAnimationState(NPC_AIDLE);
		if (temp == "3")
			n.SetAnimationState(NPC_AWANDERING);
		if (temp == "4")
			n.SetAnimationState(NPC_ADYING);
		if (temp == "5")
			n.SetAnimationState(NPC_ACRYING);

		getline(myfile, temp, ',');
		if (temp == "1")
			n.SetDialogueState(NPC::NPC_DSTORY1);
		if (temp == "2")
			n.SetDialogueState(NPC::NPC_DSTORY2);

		getline(myfile, temp, '\n');
		n.SetDialogue(temp);
		npcs.push_back(n);
	}
	return true;
}
vector<NPC> NPC::GetNPCVec()
{
	return npcs;
}
int NPC::GetTextSize(string filename)
{
	ifstream myfile;
	myfile.open(filename);
	string temp;
	if (myfile.good())
	{
		string temp;
		while (getline(myfile, temp))
		{
			size++;
		}
	}
	return size;
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