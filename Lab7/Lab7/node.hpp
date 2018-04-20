#pragma once

enum Colour { RED, BLACK };

template<class t>
class Node
{
public:
	t value;
	Colour colour;
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
	left = nullptr;
	right = nullptr;
	parent = nullptr;
}

template<class t>
Node<t>::Node(t value)
{
	this->value = value;
	colour = RED;
	left = nullptr;
	right = nullptr;
	parent = nullptr;
}