#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "tree.hpp"

using namespace std;

int main()
{
	Tree<int> t;
	for (int i = 0; i < 10; ++i)
	{
		t.insert(i);
	}
	t.remove(7);
	t.print();
	system("pause");
    return 0;
}

