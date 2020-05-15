#pragma once
#include "C:\Users\Asus\source\repos\ConsoleApplication1\ConsoleApplication1\BBV.h"
#include <fstream>

class HaffmanTree
{
public:
	HaffmanTree();

	~HaffmanTree();

	bool code(char* input_name, char* output_name);

	bool decode(char* input_name, char* output_name);

	void clear_tree();

private:

	void create_tree(char* file_name);

	void count_chars(char* file_name, int* tab);

	void create_list(int* tab);

	class Node
	{
	public:
		Node(int length);

		BBV vec;
		int weight;
		Node* lt;
		Node* rt;
		Node* next;
	};

	Node* root;
};