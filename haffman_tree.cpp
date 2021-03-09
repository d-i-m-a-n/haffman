
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
	// удаляем дерево с помощью очереди, записывая поддеревья в конец
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
		root = nullptr;
	}
}
// функция кодирования файла input_name в файл output_name
bool HaffmanTree::code(std::string input_name, std::string output_name)
{
	std::ifstream in(input_name);
	char a;
	in.get(a);
	// поверяем, что входной файл не пуст и что в дереве нет другого кодирования
	if (root || in.eof())
	{
		in.close();
		return false;
	}
	in.close();
	// дерево пустое, значит надо его построить
	create_tree(input_name);
	// построили дерево, начинаем кодировать символы
	std::ofstream out(output_name);
	in.open(input_name);
	in.get(a);
	Node* node = nullptr;
	do
	{
		// идем по дереву до листьев, поворачивая налево ставим 0, поворачивая направо ставим 1
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

		// кодируем, пока не закончится файл
		in.get(a);
	} while (!in.eof());
	in.close();
	out.close();
	return true;
}
// функция создания дерева из заданного файла
void HaffmanTree::create_tree(std::string file_name)
{
	// таблица количества определенных символов в тексте
	int tab[256] = { 0 };
	// функция, заполняющая таблицу
	count_chars(file_name, tab);
	// функция, создающая первоначальный упорядоченный список вершин
	create_list(tab);
	// проверяем сиутацию когда в тексте встречается один и тот же символ
	if (!root->next)
	{
		// добавляем пустую вершину, которая позволит построить дерево, т.к. необходимо минимум 2 вершины
		Node* ptr = root;
		root = new Node(256);
		root->next = ptr;
	}

	// объединяем вершины, пока не останется 1 единственная
	while (root->next)
	{
		// первая и вторая наименьшие вершины
		Node* node1 = root, * node2 = root->next;

		Node* newNode = new Node(256);
		newNode->vec = node1->vec | node2->vec;
		newNode->weight = node1->weight + node2->weight;
		newNode->lt = node1;
		newNode->rt = node2;
		node1->next = nullptr;

		// ищем место для новой вершины
		Node** tmp = &(node2->next);

		while (*tmp && newNode->weight > (*tmp)->weight)
		{
			tmp = &((*tmp)->next);
		}
		newNode->next = *tmp;
		*tmp = newNode;

		root = node2->next;
		node2->next = nullptr;
	}

}
// считаем количество символо в файле, записываем в таблицу результаты
void HaffmanTree::count_chars(std::string file_name, int* tab)
{
	std::ifstream in(file_name);

	char a;
	while (!in.eof())
	{
		in.get(a);
		tab[a]++;
	}
	tab[a]--;

	in.close();
}
// создается упорядоченый по возрастанию список для начала создания дерева
void HaffmanTree::create_list(int* tab)
{
	Node* node = nullptr;

	for (int i = 0; i < 256; i++)
	{
		if (tab[i])
		{
			// для каждого символа из текста создается вершина с весом и определяется ее место
			node = new Node(256);
			node->weight = tab[i];
			node->vec[i] = 1;

			Node** tmp = &root;

			while (*tmp && node->weight > (*tmp)->weight)
			{
				tmp = &((*tmp)->next);
			}
			node->next = *tmp;
			*tmp = node;
		}
	}
}
// функция преобразования закодированного текста в раскодированный
bool HaffmanTree::decode(std::string input_name, std::string output_name)
{
	// проверяем наличие дерева
	if (!root)
		return false;
	std::ifstream in(input_name);
	std::ofstream out(output_name);

	// проверяем файл на пустоту, декодим пока файл не закончился
	char a;
	in.get(a);
	while (!in.eof())
	{
		// идем каждый раз с корня
		Node* node = root;

		do
		{
			// если 1, то идем вправо, иначе влево
			if (a == 49)
				node = node->rt;
			else
				node = node->lt;
			in.get(a);
			// т.к. у каждой вершины обязательно есть оба поддерева, то проверяем только 1 из них
		} while (node->lt);

		// выводим найденный символ
		for (int i = 0; i < 256; i++)
			if (node->vec[i])
			{
				out << (char)i;
				// чтобы лишние разы не ходить, т.к. выводим только 1 символ
				i = 256;
			}

		// если файл закончился, выходим из функции
		if (in.eof())
			return true;
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
