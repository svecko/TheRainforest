#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "Vector2D.h"

struct Data
{
	std::string name;
	int score;
};

class File
{
public:
	File();
	~File();

	void WriteData(std::string name, int score, const char* fileName);
	void PrintData(const char* fileName);


private:
	std::vector<Data> m_Data;
};
