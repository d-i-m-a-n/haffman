// haffman.cpp: главный файл проекта.

#include "stdafx.h"
#include "haffman_tree.h"
#include <iostream>
#include <string>
//функция проверки правильноти раскодирования
bool CMPfiles(std::string file1, std::string file2)
{
	std::ifstream in1(file1);
	std::ifstream in2(file2);

	char a, b;
	in1.get(a);
	in2.get(b);
	while (!in1.eof() && !in2.eof())
	{
		if (a != b)
			return false;
		in1.get(a);
		in2.get(b);
	}
	
	if (in1.eof() && in2.eof())
		return true;

	return false;
}

int main()
{
	HaffmanTree tree;
	if(tree.code("input.txt", "coded.txt"))
	{
			tree.decode("coded.txt", "decoded.txt");

			if (CMPfiles("input.txt", "decoded.txt"))
				std::cout << "right solution\n";
			else
				std::cout << "wrong solution\n";
	}
	else
		std::cout << "empty input text\n";

	system("pause");
    return 0;
}
