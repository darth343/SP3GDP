#pragma once
#include "Monster.h"
#include <string>
#include <fstream>
#include <iostream>
#include <ostream>
#include <vector>

using std::iostream;
using std::ifstream;
using std::string;
using std::vector;
using std::ostream;

class MonsterManager
{
public:
	MonsterManager();
	~MonsterManager();

	vector <Monster> MonsterData; // Type, Name, HP, Atk, Def, Slot1, Slot2, Slot3, Slot4

	void readMonsterCSV(const string fileName);

private:

};


