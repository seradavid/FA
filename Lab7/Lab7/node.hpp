#pragma once

enum Colour { RED, BLACK };

template<class t>
class Node
{
public:
	t value;
	Colour colour;
	int size;
	Node *left;
	Node *right;
	Node *parent;

	Node();

	Node(t value);
};

template<class t>
Node<t>::Node()
{
	value = 0;
	colour = RED;
	size = 0;
	left = nullptr;
	right = nullptr;
	parent = nullptr;
}

template<class t>
Node<t>::Node(t value)
{
	this->value = value;
	colour = RED;
	size = 1;
	left = nullptr;
	right = nullptr;
	parent = nullptr;
}