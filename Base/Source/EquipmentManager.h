#pragma once
#include "Equipment.h"
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

class EquipmentManager
{
public:
	EquipmentManager();
	~EquipmentManager();

	vector <Equipment> Equips; // Vector to push equipment info into 

	void readEquipmentCSV(const string fileName); // Read from CSV for easy changing of weapon data values
	friend ostream &operator<<(ostream &output, Equipment EQData); // Overload for testPrint function
	void testPrint(); // For Debugging purposes


private:

};

