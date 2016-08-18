#include "MonsterManager.h"

MonsterManager::MonsterManager()
{
}

MonsterManager::~MonsterManager()
{
}

void MonsterManager::readMonsterCSV(const string fileName)
{
	string temp;
	ifstream file(fileName);
	if (file)
	{
		while (file.good())
		{
			Monster MON;

			getline(file, temp, ',');

			if (temp.empty())
				continue;

			if (temp == "Banshee")
			{
				MON.setMonsterType(BANSHEE);
			}
			else if (temp == "Cerebus")
			{
				MON.setMonsterType(CEREBUS);
			}
			else if (temp == "Dragon")
			{
				MON.setMonsterType(DRAGON);
			}
			else if (temp == "Golem")
			{
				MON.setMonsterType(GOLEM);
			}
			else if (temp == "Hydra")
			{
				MON.setMonsterType(HYDRA);
			}
			else if (temp == "Manticore")
			{
				MON.setMonsterType(MANTICORE);
			}
			else if (temp == "Ogre")
			{
				MON.setMonsterType(OGRE);
			}
			else if (temp == "Pegasus")
			{
				MON.setMonsterType(PEGASUS);
			}
			else if (temp == "Wraith")
			{
				MON.setMonsterType(WRAITH);
			}
			else if (temp == "Sphinx")
			{
				MON.setMonsterType(SPHINX);
			}
			else if (temp == "Scylla")
			{
				MON.setMonsterType(SCYLLA);
			}
			else if (temp == "Minotaur")
			{
				MON.setMonsterType(MINOTAUR);
			}
			temp.clear();

			getline(file, temp, ',');
			MON.setMonsterName(temp);
			temp.clear();

			getline(file, temp, ',');
			MON.setMonsterHP(stoi(temp));
			temp.clear();

			getline(file, temp, ',');
			MON.setMonsterAtk(stoi(temp));
			temp.clear();

			getline(file, temp, ',');
			MON.setMonsterDef(stoi(temp));
			temp.clear();

			getline(file, temp, ',');
			MON.setSkillSlotOneName(temp);
			temp.clear();

			getline(file, temp, ',');
			MON.setSkillSlotTwoName(temp);
			temp.clear();

			getline(file, temp, ',');
			MON.setSkillSlotThreeName(temp);
			temp.clear();

			getline(file, temp, ',');
			MON.setSkillSlotFourName(temp);
			temp.clear();

			MonsterData.push_back(MON);
		}
		file.close();
	}
}