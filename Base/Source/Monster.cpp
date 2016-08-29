#include "Monster.h"
#include <fstream>
#include <string>
#include <sstream>
using std::string;
using std::getline;
using std::ifstream;
using std::stoi;
using std::stof;
using std::stringstream;
Monster* Monster::database[MONSTER_TOTAL];

Monster::Monster()
{
}

Monster::~Monster()
{
}

string Monster::getName()
{
	return name;
}

int Monster::GetType()
{
	return type;
}

float  Monster::getDefense()
{
	return defense;
}
float  Monster::getHealth()
{
	return health;
}
float  Monster::getMaxHealth()
{
	return maxHealth;
}
float  Monster::getDamage()
{
	return damage;
}

void Monster::takeDamage(int damage)
{
	health -= damage;
}

Monster Monster::getMonster(MONSTER_TYPE type)
{
	return *database[type];
}


void Monster::InitDatabase()
{
	ifstream file("Data\\monsterData.csv");
	string line;
	if (file.is_open())
	{
		getline(file, line); // Remove first line
		while (getline(file, line))
		{
			if (line == "")
				continue;
			stringstream ss;
			ss << line;
			Monster* temp = new Monster;
			getline(ss, line, ','); //Name
			temp->name = line;
			getline(ss, line, ','); //Type
			temp->type = (MONSTER_TYPE)stoi(line);
			getline(ss, line, ','); //Health
			temp->health = stof(line);
			temp->maxHealth = temp->health;
			getline(ss, line, ','); //Defense
			temp->defense = stof(line);
			getline(ss, line, '\n'); //Damage
			temp->damage = stof(line);
			database[temp->type] = temp;
		}
	}
}