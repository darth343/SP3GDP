#include "Inventory.h"
#include "Application.h"
#include "SharedData.h"
Inventory::Inventory(Vector3 scale)
{
	seeker.SetZero();
	EQinventory.resize(24);
	for (int i = 0; i < EQinventory.size(); ++i)
	{
		EQinventory[i] = new Equipment;
	}
	inputeState = INVENTORY;
	head = NULL;
	leftArm = NULL;
	rightArm = NULL;
	armour = NULL;
}

Inventory::~Inventory()
{
}

void Inventory::addToInventory(Items::ITEM_TYPE type)
{
	ItemInventory[type]++;
}

void Inventory::removeFromInventory(Items::ITEM_TYPE type)
{
	ItemInventory[type]--;
}

void Inventory::addToInventory(Enemy* enemy)
{
	Equipment* temp = new Equipment();
	Equipment::EQUIPMENT_TYPE randType = (Equipment::EQUIPMENT_TYPE)Math::RandIntMinMax(Equipment::SWORD, Equipment::ARMOUR);
	//Equipment::EQUIPMENT_TYPE randType = Equipment::ARMOUR;
	stringstream ss;
	ss << enemy->GetName() << " " << randType;
	temp->SetName(ss.str());
	temp->SetMonster(enemy->GetMonster());
	temp->setType(randType);
	temp->setDamage(Equipment::getDatabase()[randType][0]->getDamage() + Math::RandIntMinMax(-10, 10));
	temp->setDefense(Equipment::getDatabase()[randType][0]->getDefense() + Math::RandIntMinMax(-10, 10));
	if (temp->getDamage() < 0)
		temp->setDamage(0);
	if (temp->getDefense() < 0)
		temp->setDefense(0);
	for (int i = 0; i < EQinventory.size(); ++i)
	{
		if (EQinventory[i]->getName() == "UNDEFINED")
		{
			EQinventory[i] = temp;
			break;
		}
	}
}

Vector3 Inventory::getSeeker()
{
	return seeker;
}

Inventory::INVENTORY_STATE Inventory::getState()
{	
	return state;
}

vector<string> Inventory::getOptions()
{
	return options;
}

void Inventory::printInventory()
{
	for (int i = 0; i < Items::ITEM_TOTAL; i++)
	{
		cout << ItemInventory[i] << endl;
	}
	return;
}

void Inventory::ResetInventory()
{
	state = TAB1;
	inputeState = INVENTORY;
	secondSeeker = 0;
}

void Inventory::UpdateInput()
{
	switch (inputeState)
	{
	case INVENTORY:
		if (Application::IsKeyPressed(VK_RIGHT) && !SharedData::GetInstance()->RIGHTkeyPressed)
		{
			seeker.x++;
			if (seeker.x > 7)
				seeker.x = 0;
			SharedData::GetInstance()->RIGHTkeyPressed = true;
		}
		else if (!Application::IsKeyPressed(VK_RIGHT) && SharedData::GetInstance()->RIGHTkeyPressed)
		{
			SharedData::GetInstance()->RIGHTkeyPressed = false;
		}

		if (Application::IsKeyPressed(VK_LEFT) && !SharedData::GetInstance()->LEFTkeyPressed)
		{
			seeker.x--;
			if (seeker.x < 0)
				seeker.x = 7;
			SharedData::GetInstance()->LEFTkeyPressed = true;
		}
		else if (!Application::IsKeyPressed(VK_LEFT) && SharedData::GetInstance()->LEFTkeyPressed)
		{
			SharedData::GetInstance()->LEFTkeyPressed = false;
		}

		if (Application::IsKeyPressed(VK_UP) && !SharedData::GetInstance()->UPkeyPressed)
		{
			seeker.y++;
			if (seeker.y > 0)
				seeker.y = -2;
			SharedData::GetInstance()->UPkeyPressed = true;
		}
		else if (!Application::IsKeyPressed(VK_UP) && SharedData::GetInstance()->UPkeyPressed)
		{
			SharedData::GetInstance()->UPkeyPressed = false;
		}

		if (Application::IsKeyPressed(VK_DOWN) && !SharedData::GetInstance()->DNkeyPressed)
		{
			seeker.y--;
			if (seeker.y < -2)
				seeker.y = 0;
			SharedData::GetInstance()->DNkeyPressed = true;
		}
		else if (!Application::IsKeyPressed(VK_DOWN) && SharedData::GetInstance()->DNkeyPressed)
		{
			SharedData::GetInstance()->DNkeyPressed = false;
		}

		if (Application::IsKeyPressed(VK_RETURN) && !SharedData::GetInstance()->ENTERkeyPressed)
		{
			SharedData::GetInstance()->ENTERkeyPressed = true;
			if (getEquipmentLookAt()->getName() != "UNDEFINED")
			{
				inputeState = EQUIP_OPTIONS;
				options.clear();
				switch (getEquipmentLookAt()->getType())
				{
				case Equipment::SWORD:
				case Equipment::SHIELD:
					options.push_back("RIGHT HAND");
					options.push_back("LEFT HAND");
					options.push_back("DISCARD");
					options.push_back("BACK");
					break;
				case Equipment::HELMET:
				case Equipment::ARMOUR:
					options.push_back("EQUIP");
					options.push_back("DISCARD");
					options.push_back("BACK");
					break;
				}
			}
		}
		else if (!Application::IsKeyPressed(VK_RETURN) && SharedData::GetInstance()->ENTERkeyPressed)
		{

			SharedData::GetInstance()->ENTERkeyPressed = false;
		}

		break;
	case EQUIP_OPTIONS:
		if (Application::IsKeyPressed(VK_DOWN) && !SharedData::GetInstance()->DNkeyPressed)
		{
			secondSeeker++;
			if (secondSeeker > options.size() - 1)
				secondSeeker = 0;
			SharedData::GetInstance()->DNkeyPressed = true;
		}
		else if (!Application::IsKeyPressed(VK_DOWN) && SharedData::GetInstance()->DNkeyPressed)
		{
			SharedData::GetInstance()->DNkeyPressed = false;
		}

		if (Application::IsKeyPressed(VK_UP) && !SharedData::GetInstance()->UPkeyPressed)
		{
			secondSeeker--;
			if (secondSeeker < 0)
				secondSeeker = options.size() -1;
			SharedData::GetInstance()->UPkeyPressed = true;
		}

		else if (!Application::IsKeyPressed(VK_UP) && SharedData::GetInstance()->UPkeyPressed)
		{
			SharedData::GetInstance()->UPkeyPressed = false;
		}

		if (Application::IsKeyPressed(VK_RETURN) && !SharedData::GetInstance()->ENTERkeyPressed)
		{
			SharedData::GetInstance()->ENTERkeyPressed = true;
			if (options[secondSeeker] == "DISCARD")
			{
				removeFromInventory(getEquipmentLookAt());
			}
			if (options[secondSeeker] == "RIGHT HAND" || options[secondSeeker] == "LEFT HAND" || options[secondSeeker] == "EQUIP")
			{
				EquipItem(options[secondSeeker]);
			}
			secondSeeker = 0;
			inputeState = INVENTORY;
		}
		else if (!Application::IsKeyPressed(VK_RETURN) && SharedData::GetInstance()->ENTERkeyPressed)
		{
			SharedData::GetInstance()->ENTERkeyPressed = false;
		}
		break;
	}
}

Equipment* Inventory::getHead()
{
	return head;
}

void Inventory::Update(double dt)
{
	UpdateInput();
}

Equipment* Inventory::getEquipmentLookAt()
{
	return EQinventory[seeker.x + (abs(seeker.y) * 8)];
}

void Inventory::setEquipmentLookAt(Equipment* equipment)
{
	EQinventory[seeker.x + (abs(seeker.y) * 8)] = equipment;
}


Inventory::INPUT_STATE Inventory::getInputState()
{
	return inputeState;
}

int Inventory::getSecondSeeker()
{
	return secondSeeker;
}

Equipment* Inventory::getArmour()
{
	return armour;
}

Equipment* Inventory::getRightArm()
{
	return rightArm;
}

Equipment* Inventory::getLeftArm()
{
	return leftArm;
}

int Inventory::GetTotalATK()
{
	int attack = 0;
	if (head)
	{
		attack += head->getDamage();
	}
	if (leftArm)
	{
		attack += leftArm->getDamage();
	}
	if (rightArm)
	{
		attack += rightArm->getDamage();
	}
	if (armour)
	{
		attack += armour->getDamage();
	}
	if (leggings)
	{
		attack += leggings->getDamage();
	}
	return attack;
}

int Inventory::GetTotalDEF()
{
	int defense = 0;
	if (head)
	{
		defense += head->getDefense();
	}
	if (leftArm)
	{
		defense += leftArm->getDefense();
	}
	if (rightArm)
	{
		defense += rightArm->getDefense();
	}
	if (armour)
	{
		defense += armour->getDefense();
	}
	if (leggings)
	{
		defense += leggings->getDefense();
	}
	return defense;
}

void Inventory::removeFromInventory(Equipment* equipment)
{
	for (int i = 0; i < EQinventory.size(); ++i)
	{
		if (EQinventory[i] == equipment)
		{
			delete EQinventory[i];
			EQinventory[i] = NULL;
			EQinventory[i] = new Equipment;
		}
	}
}

void Inventory::EquipItem(string itemType)
{
	Equipment* equipment = getEquipmentLookAt();
	if (itemType == "EQUIP")
	{
		if (equipment->getType() == Equipment::HELMET)
		{
			Equipment* temp;
			if (head)
			{
				temp = head;
				head = equipment;
				setEquipmentLookAt(temp);
			}
			else if (!head)
			{
				head = new Equipment();
				*head = *equipment;
				removeFromInventory(equipment);
			}
		}

		if (equipment->getType() == Equipment::ARMOUR)
		{
			Equipment* temp;
			if (armour)
			{
				temp = armour;
				armour = equipment;
				setEquipmentLookAt(temp);
			}
			else if (!armour)
			{
				armour = new Equipment();
				*armour = *equipment;
				removeFromInventory(equipment);

			}
		}

	}
	if (itemType == "RIGHT HAND")
	{
		Equipment* temp;
			if (rightArm)
			{
				temp = rightArm;
				rightArm = equipment;
				setEquipmentLookAt(temp);
			}
			else if (!rightArm)
			{
				rightArm = new Equipment();
				*rightArm = *equipment;
				removeFromInventory(equipment);
			}
	}
	if (itemType == "LEFT HAND")
	{
		Equipment* temp;
		if (leftArm)
		{
			temp = leftArm;
			leftArm = equipment;
			setEquipmentLookAt(temp);
		}
		else if (!leftArm)
		{
			leftArm = new Equipment();
			*leftArm = *equipment;
			removeFromInventory(equipment);
		}
	}
}