// haffman.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "haffman_tree.h"
#include <iostream>

int main()
{
	HaffmanTree tree;
	tree.code("input.txt", "coded.txt");
	tree.decode("coded.txt", "decoded.txt");

    return 0;
}

