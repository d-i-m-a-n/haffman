
#include "stdafx.h"
#include "haffman_tree.h"
#include <fstream>
#include <queue>

HaffmanTree::Node::Node(int length) :vec(length)
{
	weight = 0;
	lt = rt = next = nullptr;
}

HaffmanTree::HaffmanTree()
{
	root = nullptr;
}

HaffmanTree::~HaffmanTree()
{
	// ������� ������ � ������� �������, ��������� ���������� � �����
	if (root)
	{
		std::queue<Node*> nodes;
		nodes.push(root);
		Node* ptr = nullptr;
		do
		{
			ptr = nodes.front();
			if (ptr->lt)
				nodes.push(ptr->lt);
			if (ptr->rt)
				nodes.push(ptr->rt);

			ptr->lt = ptr->rt = ptr->next = nullptr;
			delete ptr;
		} while (!nodes.empty());

	}
	root = nullptr;
}

bool HaffmanTree::code(char* input_name, char* output_name)
{
	std::ifstream in(input_name);
	char a;
	in >> a;
	// ��������, ��� ���� �� ���� � ��� � ������ ��� ������� �����������
	if (root || in.eof())
	{
		in.close();
		return false;
	}
	in.close();

	create_tree(input_name);
	
	std::ofstream out(output_name);
	in.open(input_name);
	Node* node = nullptr;
	do
	{
		node = root;
		while (node->lt)
		{
			if (node->lt->vec[a])
			{
				out << 0;
				node = node->lt;
			}
			else
			{
				out << 1;
				node = node->rt;
			}
		}


		in >> a;
	} while (!in.eof());
	in.close();
	out.close();
	return true;
}

void HaffmanTree::create_tree(char* file_name)
{
	int tab[256] = { 0 };

	count_chars(file_name, tab);
	create_list(tab);

	// ���������� �������, ���� �� ��������� 1 ������������
	while (root->next)
	{
		// ������ � ������ ���������� �������
		Node* node1 = root, *node2 = root->next;

		Node* newNode = new Node(256);
		newNode->vec = node1->vec | node2->vec;
		newNode->weight = node1->weight + node2->weight;
		newNode->lt = node1;
		newNode->rt = node2;
		node1->next = nullptr;

		// ���� ����� ��� ����� �������
		Node** tmp = &(node2->next);

		while (*tmp && newNode->weight >(*tmp)->weight)
		{
			tmp = &((*tmp)->next);
		}
		newNode->next = *tmp;
		*tmp = newNode;

		root = node2->next;
		node2->next = nullptr;
	}
}

void HaffmanTree::count_chars(char* file_name, int* tab)
{
	std::ifstream in(file_name);

	char a;
	while (!in.eof())
	{
		in >> a;
		tab[a]++;
	}
	tab[a]--;

	in.close();
}

void HaffmanTree::create_list(int* tab)
{
	Node* node = nullptr;

	for (int i = 0; i < 256; i++)
	{
		if (tab[i])
		{
			node = new Node(256);
			node->weight = tab[i];
			node->vec[i] = 1;

			Node** tmp = &root;

			while (*tmp && node->weight >(*tmp)->weight)
			{
				tmp = &((*tmp)->next);
			}
			node->next = *tmp;
			*tmp = node;
		}
	}
}

bool HaffmanTree::decode(char* input_name, char* output_name)
{
	std::ifstream in(input_name);
	std::ofstream out(output_name);

	// ��������� ���� �� �������, ������� ���� ���� �� ����������
	char a;
	in >> a;
	while (!in.eof())
	{
		// ���� ������ ��� � �����
		Node* node = root;

		do
		{
			// ���� 1, �� ���� ������, ����� �����
			if (a == 49)
				node = node->rt;
			else
				node = node->lt;
			in >> a;
			// �.�. � ������ ������� ����������� ���� ��� ���������, �� ��������� ������ 1 �� ���
		} while (node->lt);

		// ���� ���� ����������, ������� �� �������
		if (in.eof())
			return true;

		// ������� ��������� ������
		for (int i = 0; i < 256; i++)
			if (node->vec[i])
			{
				out << (char)i;
				// ����� ������ ���� �� ������, �.�. ������� ������ 1 ������
				i = 256;
			}
		out << "  " << node->weight << '\n';
	}
	return false;
}

void HaffmanTree::clear_tree()
{
	if (root)
	{
		std::queue<Node*> nodes;
		nodes.push(root);
		Node* ptr = nullptr;
		do
		{
			ptr = nodes.front();
			if (ptr->lt)
				nodes.push(ptr->lt);
			if (ptr->rt)
				nodes.push(ptr->rt);

			ptr->lt = ptr->rt = ptr->next = nullptr;
			delete ptr;
		} while (!nodes.empty());

	}
	root = nullptr;
}
