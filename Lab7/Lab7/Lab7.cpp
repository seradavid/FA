#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "tree.hpp"

using namespace std;

int main()
{
	Tree<int> t;
	t.insert(13);
	t.insert(8);
	t.insert(17);
	t.insert(1);
	t.insert(11);
	t.insert(15);
	t.insert(25);
	t.insert(6);
	t.insert(22);
	t.insert(27);
	t.print();
	system("pause");
    return 0;
}

