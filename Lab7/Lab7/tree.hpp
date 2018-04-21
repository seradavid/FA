#pragma once
#include "node.hpp"

template<class t>
class Tree
{
private:
	Node<t> *root;

public:
	Tree();

	~Tree();

	void insert(const t&);

	void remove(const t&);

	void print();

protected:
	void rotateLeft(Node<t> *);

	void rotateRight(Node<t> *);

	void fixInsert(Node<t> *);

	void fixRemove(Node<t> *);

	void transplant(Node<t> *, Node<t> *);

	Node<t>* minimum(Node<t> *);

	void deleteNode(Node<t> *);

	void print(Node<t> *, int);
};

template<class t>
Tree<t>::Tree()
	:root(nullptr)
{
}

template<class t>
Tree<t>::~Tree()
{
	deleteNode(root);
}

template<class t>
void Tree<t>::rotateLeft(Node<t> *x)
{
	Node<t> *y;

	y = x->right;
	x->right = y->left;
	if (y->left)
	{
		y->left->parent = x;
	}

	y->parent = x->parent;
	y->left = x;

	if (!x->parent)
	{
		root = y;
	}
	else if (x == x->parent->left)
	{
		x->parent->left = y;
	}
	else
	{
		x->parent->right = y;
	}
	x->parent = y;
}

template<class t>
void Tree<t>::rotateRight(Node<t> *y)
{
	Node<t> *x;

	x = y->left;
	y->left = x->right;
	if (x->right)
	{
		x->right->parent = y;
	}

	x->parent = y->parent;
	x->right = y;

	if (!y->parent)
	{
		root = x;
	}
	else if (y == y->parent->left)
	{
		y->parent->left = x;
	}
	else
	{
		y->parent->right = x;
	}

	y->parent = x;
}

template<class t>
void Tree<t>::insert(const t& value)
{
	Node<t> *node, *parent, *z;

	parent = nullptr;
	node = root;
	while (node)
	{
		parent = node;
		if (value < node->value)
		{
			node = node->left;
		}
		else
		{
			node = node->right;
		}
	}

	if (!parent)
	{
		z = root = new Node<t>;
		z->value = value;
		z->colour = BLACK;
		z->parent = z->left = z->right = nullptr;
	}
	else
	{
		z = new Node<t>;
		z->value = value;
		z->colour = RED;
		z->parent = parent;
		z->left = z->right = nullptr;

		if (z->value < parent->value)
		{
			parent->left = z;
		}
		else
		{
			parent->right = z;
		}
	}

	fixInsert(z);
}

template<class t>
void Tree<t>::fixInsert(Node<t> *z)
{
	Node<t> *uncle;
	bool side;
	while (z->parent && z->parent->colour == RED)
	{
		if ((side = (z->parent == z->parent->parent->left)))
		{
			uncle = z->parent->parent->right;
		}
		else
		{
			uncle = z->parent->parent->left;
		}

		if (uncle && uncle->colour == RED)
		{
			z->parent->colour = BLACK;
			uncle->colour = BLACK;
			z->parent->parent->colour = RED;
			z = z->parent->parent;
		}
		else
		{
			if (z == (side ? z->parent->right : z->parent->left))
			{
				z = z->parent;
				side ? rotateLeft(z) : rotateRight(z);
			}

			z->parent->colour = BLACK;
			z->parent->parent->colour = RED;
			side ? rotateRight(z->parent->parent) : rotateLeft(z->parent->parent);
		}
	}

	root->colour = BLACK;
}

template<class t>
void Tree<t>::remove(const t& value)
{
	Node<t> *node = root;
	while (node)
	{
		if (value < node->value)
		{
			node = node->left;
		}
		else if (value > node->value)
		{
			node = node->right;
		}
		else
		{
			break;
		}
	}

	if (!node || node->value != value)
	{
		return;
	}

	Colour original = node->colour;
	Node<t> *x, *y;
	y = node;
	if (!node->left)
	{
		transplant(node, x = node->right);
	}
	else if (!node->right)
	{
		transplant(node, x = node->left);
	}
	else
	{
		y = minimum(node->right);
		original = y->colour;
		x = y->right;

		if (y->parent == node)
		{
			x->parent = y;
		}
		else
		{
			transplant(y, y->right);
			y->right = node->right;
			y->right->parent = y;
		}

		transplant(node, y);
		y->left = node->left;
		y->left->parent = y;
		y->colour = node->colour;
	}

	delete node;
	if (original == BLACK)
	{
		fixRemove(x);
	}
}

template<class t>
void Tree<t>::fixRemove(Node<t> *x)
{
	bool side;
	Node<t> *sibling;
	while (x && x != root && x->colour == BLACK)
	{
		if ((side = (x == x->parent->left)))
		{
			sibling = x->parent->right;
		}
		else
		{
			sibling = x->parent->left;
		}

		if (sibling->colour == RED)
		{
			sibling->colour = BLACK;
			x->parent->colour = RED;
			side ? rotateLeft(x->parent) : rotateRight(x->parent);
			sibling = side ? x->parent->right : x->parent->left;
		}

		if (sibling->left->colour == BLACK && sibling->right->colour == BLACK)
		{
			sibling->colour = RED;
			x = x->parent;
		}
		else
		{
			if (BLACK == side ? sibling->right->colour : sibling->left->colour)
			{
				sibling->colour = RED;
				if (side)
				{
					sibling->left->colour = BLACK;
					rotateRight(sibling);
					sibling = x->parent->right;
				}
				else
				{
					sibling->right->colour = BLACK;
					rotateLeft(sibling);
					sibling = x->parent->left;
				}
			}

			sibling->colour = x->parent->colour;
			x->parent->colour = BLACK;
			if (side)
			{
				sibling->right->colour = BLACK;
				rotateLeft(x->parent);
			}
			else
			{
				sibling->left->colour = BLACK;
				rotateRight(x->parent);
			}

			x = root;
		}
	}
	if (x)
	{
		x->colour = BLACK;
	}
}

template<class t>
void Tree<t>::transplant(Node<t> *dest, Node<t> *src)
{
	if (!dest->parent)
	{
		root = src;
	}
	else if (dest == dest->parent->left)
	{
		dest->parent->left = src;
	}
	else
	{
		dest->parent->right = src;
	}

	if (src)
	{
		src->parent = dest->parent;
	}
}

template<class t>
Node<t> *Tree<t>::minimum(Node<t> *node)
{
	while (node->left)
	{
		node = node->left;
	}

	return node;
}

template<class t>
void Tree<t>::deleteNode(Node<t> *node)
{
	if (!node)
	{
		return;
	}

	if (node->left)
	{
		deleteNode(node->left);
	}

	if (node->right)
	{
		deleteNode(node->right);
	}

	delete node;
}

template<class t>
void Tree<t>::print(Node<t> *node, int tabs)
{
	if (!node)
	{
		return;
	}

	print(node->left, tabs + 1);

	for (int i = 0; i < tabs; ++i)
	{
		std::cout << "\t";
	}
	std::cout << node->value << (node->colour ? "B" : "R") << std::endl;

	print(node->right, tabs + 1);
}

template<class t>
void Tree<t>::print()
{
	print(root, 0);
}