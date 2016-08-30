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
	state = TAB1;
	inputeState = INVENTORY;
	for (int i = 0; i < EI_TOTAL; ++i)
	{
		EquippedItems[i] = NULL;
	}
}

Inventory::~Inventory()
{
	for (int i = 0; i < EI_TOTAL; ++i)
	{
		if (EquippedItems[i])
			delete EquippedItems[i];
	}
	for (int i = 0; i < EQinventory.size(); ++i)
	{
		if (EQinventory[i])
		{
			delete EQinventory[i];
		}
	}
}

void Inventory::usePotion()
{
	if (ItemInventory[Items::POTION] > 0)
	{
		SharedData::GetInstance()->player->AddHealth(25);
		ItemInventory[Items::POTION]--;
	}
}

void Inventory::addToInventory(Items::ITEM_TYPE type)
{
	ItemInventory[type]++;
}

void Inventory::removeFromInventory(Items::ITEM_TYPE type)
{
	ItemInventory[type]--;
}

void swap(vector<Equipment *> list, int first, int second)
{
	Equipment temp;
	temp = *list[first];
	*list[first] = *list[second];
	*list[second] = temp;
}

void Inventory::SortByType(vector<Equipment*> theInventory)
{
	for (int iter = 1; iter < theInventory.size(); ++iter)
	{
		for (int index = 0; index < theInventory.size() - iter; ++index)
		{
			if (theInventory[index]->getType() < theInventory[index + 1]->getType())
			{
				swap(theInventory, index, index + 1);
			}
			else if (theInventory[index]->getType() == theInventory[index + 1]->getType())
			{
				SortByStats(theInventory, index);
			}
		}
	}
}

void Inventory::SortByStats(vector<Equipment*> theInventory, int index)
{
	if ((theInventory[index]->getDamage() + theInventory[index]->getDefense()) * 0.5 < (theInventory[index+1]->getDamage() + theInventory[index+1]->getDefense()) * 0.5)
	{
		swap(theInventory, index, index + 1);
	}
}

void Inventory::SortInventory()
{
	SortByType(EQinventory);
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

int Inventory::GetPotionCount()
{
	return ItemInventory[Items::POTION];
}

int Inventory::GetMemoryCount()
{
	return ItemInventory[Items::ENCRYPTED_MEMORY];
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
	seeker.SetZero();
	secondSeeker = 0;
}

void Inventory::UpdateInput()
{
	switch (state)
	{
	case TAB1:
		switch (inputeState)
		{
		case INVENTORY:
			if (Application::IsKeyPressed(VK_RIGHT) && !SharedData::GetInstance()->RIGHTkeyPressed)
			{
				seeker.x++;
				if (seeker.x > 7)
					seeker.x = 7;
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
				{
					ResetInventory();
					state = TAB2;
				}
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
						if (ItemInventory[Items::ENCRYPTED_MEMORY] >= 100)
						options.push_back("POWER UP");
						options.push_back("DISCARD");
						options.push_back("BACK");
						break;
					case Equipment::HELMET:
					case Equipment::ARMOUR:
					case Equipment::LEG:
						options.push_back("EQUIP");
						if (ItemInventory[Items::ENCRYPTED_MEMORY] >= 100)
						options.push_back("POWER UP");
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
					secondSeeker = options.size() - 1;
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
					SortInventory();
				}
				if (options[secondSeeker] == "POWER UP")
				{
					if (ItemInventory[Items::ENCRYPTED_MEMORY] >= 100)
					{
						PowerUp(getEquipmentLookAt());
						ItemInventory[Items::ENCRYPTED_MEMORY] -= 100;
					}
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
		break;
		case TAB2:
			switch (inputeState)
			{
			case INVENTORY:
				if (Application::IsKeyPressed(VK_RIGHT) && !SharedData::GetInstance()->RIGHTkeyPressed)
				{
					seeker.x++;
					if (seeker.x > 0)
					{
						ResetInventory();
					}
					SharedData::GetInstance()->RIGHTkeyPressed = true;
				}
				else if (!Application::IsKeyPressed(VK_RIGHT) && SharedData::GetInstance()->RIGHTkeyPressed)
				{
					SharedData::GetInstance()->RIGHTkeyPressed = false;
				}
				if (Application::IsKeyPressed(VK_DOWN) && !SharedData::GetInstance()->DNkeyPressed)
				{
					seeker.y++;
					if (seeker.y > Items::ITEM_TOTAL - 1)
						seeker.y = 0;
					SharedData::GetInstance()->DNkeyPressed = true;
				}
				else if (!Application::IsKeyPressed(VK_DOWN) && SharedData::GetInstance()->DNkeyPressed)
				{
					SharedData::GetInstance()->DNkeyPressed = false;
				}
				if (Application::IsKeyPressed(VK_UP) && !SharedData::GetInstance()->UPkeyPressed)
				{
					seeker.y--;
					if (seeker.y < 0)
						seeker.y = Items::ITEM_TOTAL - 1;
					SharedData::GetInstance()->UPkeyPressed = true;
				}
				else if (!Application::IsKeyPressed(VK_UP) && SharedData::GetInstance()->UPkeyPressed)
				{
					SharedData::GetInstance()->UPkeyPressed = false;
				}
				if (Application::IsKeyPressed(VK_RETURN) && !SharedData::GetInstance()->ENTERkeyPressed)
				{
					SharedData::GetInstance()->ENTERkeyPressed = true;
					inputeState = EQUIP_OPTIONS;
					options.clear();
					options.push_back("USE");
					options.push_back("BACK");
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
						secondSeeker = options.size() - 1;
					SharedData::GetInstance()->UPkeyPressed = true;
				}
				else if (!Application::IsKeyPressed(VK_UP) && SharedData::GetInstance()->UPkeyPressed)
				{
					SharedData::GetInstance()->UPkeyPressed = false;
				}
				if (Application::IsKeyPressed(VK_RETURN) && !SharedData::GetInstance()->ENTERkeyPressed)
				{
					SharedData::GetInstance()->ENTERkeyPressed = true;
					if (options[secondSeeker] == "USE")
					{
						if (seeker.y == Items::POTION)
						usePotion();
					}
					if (options[secondSeeker] == "RIGHT HAND" || options[secondSeeker] == "LEFT HAND" || options[secondSeeker] == "EQUIP")
					{
						EquipItem(options[secondSeeker]);
					}
					secondSeeker = 0;
					inputeState = INVENTORY;
				}

			}
			break;
	}
}

Equipment* Inventory::getHead()
{
	return EquippedItems[HEAD];
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
	return EquippedItems[CHEST];
}

Equipment* Inventory::getRightArm()
{
	return EquippedItems[RHAND];
}

Equipment* Inventory::getLeftArm()
{
	return EquippedItems[LHAND];
}

Equipment* Inventory::getLeg()
{
	return EquippedItems[LEGS];
}

Equipment** Inventory::getEquippedItems()
{
	return EquippedItems;
}

int Inventory::GetTotalATK()
{
	int attack = 0;
	if (EquippedItems[HEAD])
	{
		attack += EquippedItems[HEAD]->getDamage();
	}
	if (EquippedItems[LHAND])
	{
		attack += EquippedItems[LHAND]->getDamage();
	}
	if (EquippedItems[RHAND])
	{
		attack += EquippedItems[RHAND]->getDamage();
	}
	if (EquippedItems[CHEST])
	{
		attack += EquippedItems[CHEST]->getDamage();
	}
	if (EquippedItems[LEGS])
	{
		attack += EquippedItems[LEGS]->getDamage();
	}
	return attack;
}

int Inventory::GetTotalDEF()
{
	int defense = 0;
	if (EquippedItems[HEAD])
	{
		defense += EquippedItems[HEAD]->getDefense();
	}
	if (EquippedItems[LHAND])
	{
		defense += EquippedItems[LHAND]->getDefense();
	}
	if (EquippedItems[RHAND])
	{
		defense += EquippedItems[RHAND]->getDefense();
	}
	if (EquippedItems[CHEST])
	{
		defense += EquippedItems[CHEST]->getDefense();
	}
	if (EquippedItems[LEGS])
	{
		defense += EquippedItems[LEGS]->getDefense();
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
			SharedData::GetInstance()->inventory.ItemInventory[Items::ENCRYPTED_MEMORY] += 50;
		}
	}
}

void Inventory::PowerUp(Equipment* equipment)
{
	equipment->setDamage(equipment->getDamage() + Math::RandIntMinMax(0, 10));
	equipment->setDefense(equipment->getDefense() + Math::RandIntMinMax(0, 10));
}

void Inventory::EquipItem(string itemType)
{
	Equipment* equipment = getEquipmentLookAt();
	if (itemType == "EQUIP")
	{
		if (equipment->getType() == Equipment::HELMET)
		{
			Equipment* temp;
			if (EquippedItems[HEAD])
			{
				temp = EquippedItems[HEAD];
				EquippedItems[HEAD] = equipment;
				setEquipmentLookAt(temp);
				SharedData::GetInstance()->tamagucci.SetIndex(HEAD);
				EquippedItems[HEAD]->position.x = Math::RandFloatMinMax(0.f, 700);
			}
			else if (!EquippedItems[HEAD])
			{
				EquippedItems[HEAD] = new Equipment();
				*EquippedItems[HEAD] = *equipment;
				removeFromInventory(equipment);
				SharedData::GetInstance()->tamagucci.SetIndex(HEAD);
				EquippedItems[HEAD]->position.x = Math::RandFloatMinMax(0.f, 700);
			}
		}

		if (equipment->getType() == Equipment::ARMOUR)
		{
			Equipment* temp;
			if (EquippedItems[CHEST])
			{
				temp = EquippedItems[CHEST];
				EquippedItems[CHEST] = equipment;
				setEquipmentLookAt(temp);
				SharedData::GetInstance()->tamagucci.SetIndex(CHEST);
				EquippedItems[CHEST]->position.x = Math::RandFloatMinMax(0.f, 700);
			}
			else if (!EquippedItems[CHEST])
			{
				EquippedItems[CHEST] = new Equipment();
				*EquippedItems[CHEST] = *equipment;
				removeFromInventory(equipment);
				SharedData::GetInstance()->tamagucci.SetIndex(CHEST);
				EquippedItems[CHEST]->position.x = Math::RandFloatMinMax(0.f, 700);
			}
		}

		if (equipment->getType() == Equipment::LEG)
		{
			Equipment* temp;
			if (EquippedItems[LEGS])
			{
				temp = EquippedItems[LEGS];
				EquippedItems[LEGS] = equipment;
				setEquipmentLookAt(temp);
				SharedData::GetInstance()->tamagucci.SetIndex(LEGS);
				EquippedItems[LEGS]->position.x = Math::RandFloatMinMax(0.f, 700);

			}
			else if (!EquippedItems[LEGS])
			{
				EquippedItems[LEGS] = new Equipment();
				*EquippedItems[LEGS] = *equipment;
				removeFromInventory(equipment);
				SharedData::GetInstance()->tamagucci.SetIndex(LEGS);
				EquippedItems[LEGS]->position.x = Math::RandFloatMinMax(0.f, 700);


			}
		}

	}
	if (itemType == "RIGHT HAND")
	{
		Equipment* temp;
			if (EquippedItems[RHAND])
			{
				temp = EquippedItems[RHAND];
				EquippedItems[RHAND] = equipment;
				setEquipmentLookAt(temp);
				SharedData::GetInstance()->tamagucci.SetIndex(RHAND);
				EquippedItems[RHAND]->position.x = Math::RandFloatMinMax(0.f, 700);


			}
			else if (!EquippedItems[RHAND])
			{
				EquippedItems[RHAND] = new Equipment();
				*EquippedItems[RHAND] = *equipment;
				removeFromInventory(equipment);
				SharedData::GetInstance()->tamagucci.SetIndex(RHAND);
				EquippedItems[RHAND]->position.x = Math::RandFloatMinMax(0.f, 700);
				
			}
	}
	if (itemType == "LEFT HAND")
	{
		Equipment* temp;
		if (EquippedItems[LHAND])
		{
			temp = EquippedItems[LHAND];
			EquippedItems[LHAND] = equipment;
			setEquipmentLookAt(temp);
			SharedData::GetInstance()->tamagucci.SetIndex(LHAND);
			EquippedItems[LHAND]->position.x = Math::RandFloatMinMax(0.f, 700);
			
		}
		else if (!EquippedItems[LHAND])
		{
			EquippedItems[LHAND] = new Equipment();
			*EquippedItems[LHAND] = *equipment;
			removeFromInventory(equipment);
			SharedData::GetInstance()->tamagucci.SetIndex(LHAND);
			EquippedItems[LHAND]->position.x = Math::RandFloatMinMax(0.f, 700);
						
		}
	}
}