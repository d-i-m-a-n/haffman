#pragma once
#include "BBV.h"
#include <fstream>
#include <string>

class HaffmanTree
{
public:
	HaffmanTree();

	~HaffmanTree();

	bool code(std::string input_name, std::string output_name);

	bool decode(std::string input_name, std::string output_name);

	void clear_tree();

private:

	void create_tree(std::string file_name);

	void count_chars(std::string file_name, int* tab);

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