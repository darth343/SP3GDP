#include "EquipmentManager.h"
#include "Equipment.h"

EquipmentManager::EquipmentManager()
{
}

EquipmentManager::~EquipmentManager()
{
}

void EquipmentManager::readEquipmentCSV(const string fileName)
{
	string temp;
	ifstream file(fileName);
	if (file)
	{
		while (file.good())
		{
			Equipment EQ;

			getline(file, temp, ',');

			if (temp.empty())
				continue;
			if (temp == "Sword")
			{
				EQ.setEQType(SWORD);
			}
			else if (temp == "Shield")
			{
				EQ.setEQType(SHIELD);
			}
			else if (temp == "Helm")
			{
				EQ.setEQType(HELM);
			}
			else if (temp == "Armour")
			{
				EQ.setEQType(ARMOUR);
			}
			else if (temp == "Medallion")
			{
				EQ.setEQType(MEDALLION);
			}
			temp.clear();

			getline(file, temp, ',');
			EQ.setEQName(temp);
			temp.clear();

			getline(file, temp, ',');
			EQ.setEQAtk(stoi(temp));
			temp.clear();

			getline(file, temp, ',');
			EQ.setEQDef(stoi(temp));
			temp.clear();

			getline(file, temp, '\n');
			EQ.setEQHP(stoi(temp));
			temp.clear();

			Equips.push_back(EQ);

		}
		file.close();
	}
}

ostream &operator<<(ostream &output, Equipment EQData)
{
	if (EQData.getEQType() == SWORD)
	{
		output << "Type: Sword ";
	}
	else if (EQData.getEQType() == SHIELD)
	{
		output << "Type: Shield ";
	}
	else if (EQData.getEQType() == HELM)
	{
		output << "Type: Helm ";
	}
	else if (EQData.getEQType() == ARMOUR)
	{
		output << "Type: Armour ";
	}
	else if (EQData.getEQType() == MEDALLION)
	{
		output << "Type: Medallion ";
	}

	output << "Name: " << EQData.getEQName() << " " << "Attack: " << EQData.getEQAtk() << " " << "Defense: " << EQData.getEQDef() << " " << "HP: " << EQData.getEQHP() << std::endl;

	return output;
}

void EquipmentManager::testPrint()
{
	readEquipmentCSV("Image//EquipmentInfo.csv");

	for (int i = 0; i < Equips.size(); i++)
	{
		std::cout << Equips[i] << std::endl;
	}
}