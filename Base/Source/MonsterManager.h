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

	vector <Monster> Monsters;

	void readMonsterCSV(const string fileName);

private:

};


