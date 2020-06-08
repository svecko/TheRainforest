#include "File.h"

File::File()
{
}

File::~File()
{
}

void File::WriteData(std::string name, int score, const char* fileName)
{
	std::ofstream data(fileName, std::ios::app);

	if (data.is_open())
	{
		data << name << std::endl;
		data << score << std::endl;
		data.close();
	}
}

void File::PrintData(const char* fileName)
{
	std::string line;
	std::ifstream file(fileName);
	system("cls");
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			std::cout << line << std::endl;
		}
		file.close();
	}
}
