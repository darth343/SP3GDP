#include "Equipment.h"
#include <fstream>
#include <sstream>
using std::stringstream;
using std::ifstream;
vector<Equipment*> Equipment::equipmentDatabase[Equipment::TOTAL_ETYPE];

Equipment::Equipment() 
: name("UNDEFINED")
, damage(-1)
, defense(-1)
, type(NO_TYPE)
, direction(true)
{
	position.Set(350, 100, 1);
	Levels = 1;
	TamHappy = 5;
	TamHunger = 5;
	TamEnergy = 5;
	hungerLosstimer = 0;
	energyLosstimer = 0;
	happinessLosstimer = 0;
}

Equipment::~Equipment()
{
}
int Equipment::GetTamEnergy()
{
	return TamEnergy;
}
int Equipment::GetTamHappy()
{
	return TamHappy;
}
int Equipment::GetTamHunger()
{
	return TamHunger;
}
void Equipment::SetTamHunger(int hunger)
{
	TamHunger = hunger;
}
void Equipment::IncrementTamEnergy()
{
	TamEnergy++;
	if (TamEnergy > 5)
		TamEnergy = 5;
}

void Equipment::IncrementTamHappy()
{
		TamHappy = 5;
}

void Equipment::IncrementTamHunger(int amount)
{
	TamHunger += amount;
	if (TamHunger > 5)
		TamHunger = 5;
}

void Equipment::DecrementTamEnergy()
{
	TamEnergy--;
	if (TamEnergy < 0)
		TamEnergy = 0;
}

void Equipment::DecrementTamHappy()
{
	TamHappy--;
	if (TamHappy < 0)
		TamHappy = 0;
}

void Equipment::DecrementTamHunger()
{
	TamHunger--;
	if (TamHunger < 0)
		TamHunger = 0;
}


void Equipment::reset()
{
	SetName("UNDEFINED");
	setDamage(-1);
	setDefense(-1);
	setType(SWORD);
}

vector<Equipment*>* Equipment::getDatabase()
{
	return equipmentDatabase;
}

void Equipment::SetName(string name)
{
	this->name = name;
}

void Equipment::SetMonster(Monster monster)
{
	this->monster = monster;
}

void Equipment::setType(EQUIPMENT_TYPE type)
{
	this->type = type;
}

string Equipment::getName()
{
	return name;
}

Monster Equipment::getMonster()
{
	return monster;
}

Equipment::EQUIPMENT_TYPE Equipment::getType()
{
	return type;
}

int Equipment::getLevels()
{
	return Levels;
}

void Equipment::setLevels(int level)
{
	Levels = level;
}

int Equipment::getDamage()
{
	if (pooPositions.size() > 0)
		return 0;
	int totalLevel = TamHappy + TamHunger + TamEnergy;
	return (damage / 15) * totalLevel;
}

int Equipment::getDefense()
{
	if (pooPositions.size() > 0)
		return 0;
	int totalLevel = TamHappy + TamHunger + TamEnergy;
	return (defense / 15) * totalLevel;
}

void Equipment::setDamage(float damage)
{
	this->damage = damage;
}

void Equipment::setDefense(float defense)
{
	this->defense = defense;
}


std::ostream& operator<<(std::ostream& cout, Equipment::EQUIPMENT_TYPE type)
{
	switch (type)
	{
	case Equipment::SWORD:
		cout << "SWORD";
		break;
	case Equipment::SHIELD:
		cout << "SHIELD";
		break;
	case Equipment::HELMET:
		cout << "HELMET";
		break;
	case Equipment::ARMOUR:
		cout << "ARMOUR";
		break;
	case Equipment::LEG:
		cout << "LEG";
		break;
	}
	return cout;
}

void Equipment::InitDatabase()
{
	ifstream file("Data\\swordData.csv");
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
			Equipment* temp = new Equipment;
			getline(ss, line, ','); //Name
			temp->name = line;
			getline(ss, line, ','); //Type
			temp->type = (EQUIPMENT_TYPE)stoi(line);
			getline(ss, line, ','); //Damage
			temp->damage = stoi(line);
			getline(ss, line, '\n'); //Defense
			temp->defense = stof(line);
			equipmentDatabase[temp->type].push_back(temp);
		}
	}
	file.close();
	file.open("Data\\shieldData.csv");
	if (file.is_open())
	{
		getline(file, line); // Remove first line
		while (getline(file, line))
		{
			if (line == "")
				continue;
			stringstream ss;
			ss << line;
			Equipment* temp = new Equipment;
			getline(ss, line, ','); //Name
			temp->name = line;
			getline(ss, line, ','); //Type
			temp->type = (EQUIPMENT_TYPE)stoi(line);
			getline(ss, line, ','); //Damage
			temp->damage = stoi(line);
			getline(ss, line, '\n'); //Defense
			temp->defense = stof(line);
			equipmentDatabase[temp->type].push_back(temp);
		}
	}
	file.close();
	file.open("Data\\helmetData.csv");
	if (file.is_open())
	{
		getline(file, line); // Remove first line
		while (getline(file, line))
		{
			if (line == "")
				continue;
			stringstream ss;
			ss << line;
			Equipment* temp = new Equipment;
			getline(ss, line, ','); //Name
			temp->name = line;
			getline(ss, line, ','); //Type
			temp->type = (EQUIPMENT_TYPE)stoi(line);
			getline(ss, line, ','); //Damage
			temp->damage = stoi(line);
			getline(ss, line, '\n'); //Defense
			temp->defense = stof(line);
			equipmentDatabase[temp->type].push_back(temp);
		}
	}
	file.close();
	file.open("Data\\armourData.csv");
	if (file.is_open())
	{
		getline(file, line); // Remove first line
		while (getline(file, line))
		{
			if (line == "")
				continue;
			stringstream ss;
			ss << line;
			Equipment* temp = new Equipment;
			getline(ss, line, ','); //Name
			temp->name = line;
			getline(ss, line, ','); //Type
			temp->type = (EQUIPMENT_TYPE)stoi(line);
			getline(ss, line, ','); //Damage
			temp->damage = stoi(line);
			getline(ss, line, '\n'); //Defense
			temp->defense = stof(line);
			equipmentDatabase[temp->type].push_back(temp);
		}
	}
	file.close();
	file.open("Data\\legData.csv");
	if (file.is_open())
	{
		getline(file, line); // Remove first line
		while (getline(file, line))
		{
			if (line == "")
				continue;
			stringstream ss;
			ss << line;
			Equipment* temp = new Equipment;
			getline(ss, line, ','); //Name
			temp->name = line;
			getline(ss, line, ','); //Type
			temp->type = (EQUIPMENT_TYPE)stoi(line);
			getline(ss, line, ','); //Damage
			temp->damage = stoi(line);
			getline(ss, line, '\n'); //Defense
			temp->defense = stof(line);
			equipmentDatabase[temp->type].push_back(temp);
		}
	}
}