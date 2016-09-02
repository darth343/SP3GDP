#ifndef CHARACTERDATA_H
#define CHARACTERDATA_H
#include <string>
#include <fstream>
using std::ifstream;
using std::string;
using std::getline;
struct CharacterData
{
	void Init(string filedirectory)
	{
		ifstream file;
		string line;
		file.open(filedirectory);
		while(file.good())
		{
			getline(file, line, ',');
			int index = stoi(line);
			getline(file, line, '\n');
			float offset = stoi(line) * 0.01f;
			charOffsets[index] = offset;
		}
		int test;
	}
	float charOffsets[255];
};
#endif // !CHARACTERDATA_H
