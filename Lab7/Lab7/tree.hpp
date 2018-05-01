#pragma once
#include "node.hpp"

template<class t>
class Tree
{
private:
	Node<t> *root;
	int nrOp;

public:
	Tree();

	~Tree();

	void insert(const t&);

	void remove(const t&);

	Node<t> *select(const int&);

	void print();

	bool isEmpty();

	int getNrOp();

private:
	void rotateLeft(Node<t> *);

	void rotateRight(Node<t> *);

	void fixInsert(Node<t> *);

	void fixRemove(Node<t> *);

	void transplant(Node<t> *, Node<t> *);

	void updateSize(Node<t> *);

	Node<t>* minimum(Node<t> *);

	void deleteNode(Node<t> *);

	void print(Node<t> *, int);
};

template<class t>
Tree<t>::Tree()
	:root(nullptr)
{
	nrOp = 0;
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
	if (!y)
	{
		return;
	}
	x->right = y->left;
	nrOp += 2;

	if (y->left)
	{
		y->left->parent = x;
		nrOp++;
	}

	y->parent = x->parent;
	y->left = x;
	nrOp += 2;

	if (!x->parent)
	{
		root = y;
		nrOp++;
	}
	else if (x == x->parent->left)
	{
		x->parent->left = y;
		nrOp += 2;
	}
	else
	{
		x->parent->right = y;
		nrOp++;
	}
	x->parent = y;
	nrOp++;

	updateSize(x);
}

template<class t>
void Tree<t>::rotateRight(Node<t> *y)
{
	Node<t> *x;

	x = y->left;
	if (!x)
	{
		return;
	}
	y->left = x->right;
	nrOp += 2;

	if (x->right)
	{
		x->right->parent = y;
		nrOp++;
	}

	x->parent = y->parent;
	x->right = y;
	nrOp += 2;

	if (!y->parent)
	{
		root = x;
		nrOp++;
	}
	else if (y == y->parent->left)
	{
		y->parent->left = x;
		nrOp += 2;
	}
	else
	{
		y->parent->right = x;
		nrOp++;
	}
	y->parent = x;
	nrOp++;

	updateSize(y);
}

template<class t>
void Tree<t>::insert(const t& value)
{
	Node<t> *node, *parent, *z;

	parent = nullptr;
	node = root;
	while (node)
	{
		node->size++;
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
		z = root = new Node<t>(value);
		z->colour = BLACK;
	}
	else
	{
		z = new Node<t>(value);
		z->parent = parent;

		if (z->value < parent->value)
		{
			parent->left = z;

		}
		else
		{
			parent->right = z;
		}
		nrOp += 3;
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
		nrOp++;
		if ((side = (z->parent == z->parent->parent->left)))
		{
			uncle = z->parent->parent->right;
		}
		else
		{
			uncle = z->parent->parent->left;
		}
		nrOp++;

		if (uncle && uncle->colour == RED)
		{
			z->parent->colour = BLACK;
			uncle->colour = BLACK;
			z->parent->parent->colour = RED;
			z = z->parent->parent;
			nrOp += 2;
		}
		else
		{
			if (z == (side ? z->parent->right : z->parent->left))
			{
				z = z->parent;
				side ? rotateLeft(z) : rotateRight(z);
				nrOp += 2;
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
		node->size--;
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
			node->size++;
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
		nrOp++;
	}
	else if (!node->right)
	{
		transplant(node, x = node->left);
		nrOp += 2;
	}
	else
	{
		nrOp += 2;
		y = minimum(node->right);
		original = y->colour;
		x = y->right;
		nrOp++;

		if (y->parent == node && x)
		{
			x->parent = y;
			nrOp += 2;
		}
		else
		{
			nrOp++;
			transplant(y, y->right);
			y->right = node->right;
			nrOp++;

			updateSize(y);

			if (y->right)
			{
				y->right->parent = y;
				nrOp++;
			}
		}

		transplant(node, y);
		y->left = node->left;
		nrOp++;

		updateSize(y);

		if (y->left)
		{
			y->left->parent = y;
			nrOp++;
		}
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
		nrOp++;
		if ((side = (x == x->parent->left)))
		{
			sibling = x->parent->right;
		}
		else
		{
			sibling = x->parent->left;
		}
		nrOp++;

		if (sibling && sibling->colour == RED)
		{
			sibling->colour = BLACK;
			x->parent->colour = RED;
			side ? rotateLeft(x->parent) : rotateRight(x->parent);
			sibling = side ? x->parent->right : x->parent->left;
			//nrOp++;
		}

		if (sibling && sibling->left && sibling->left->colour == BLACK && sibling->right && sibling->right->colour == BLACK)
		{
			sibling->colour = RED;
			x = x->parent;
			nrOp += 2;
		}
		else
		{
			nrOp += 2;
			if (sibling && (side ? sibling->right : sibling->left))
			{
				if (BLACK == side ? sibling->right->colour : sibling->left->colour)
				{
					sibling->colour = RED;
					if (side)
					{
						if (sibling->left)
						{
							sibling->left->colour = BLACK;
						}
						rotateRight(sibling);
						sibling = x->parent->right;
						nrOp++;
					}
					else
					{
						if (sibling->right)
						{
							sibling->right->colour = BLACK;
						}
						rotateLeft(sibling);
						sibling = x->parent->left;
						nrOp++;
					}
				}
			}

			if (sibling)
			{
				sibling->colour = x->parent->colour;
			}
			x->parent->colour = BLACK;
			if (side)
			{
				if (sibling && sibling->right)
				{
					sibling->right->colour = BLACK;
				}
				rotateLeft(x->parent);
			}
			else
			{
				if (sibling && sibling->left)
				{
					sibling->left->colour = BLACK;
				}
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
		nrOp += 2;
	}
	else if (dest == dest->parent->left)
	{
		dest->parent->left = src;
		nrOp += 3;
	}
	else
	{
		dest->parent->right = src;
		nrOp += 3;
	}

	if (src)
	{
		src->parent = dest->parent;
		nrOp++;
	}

	updateSize(dest->parent);
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
Node<t> *Tree<t>::select(const int& index)
{
	if (index > root->size)
	{
		return nullptr;
	}

	Node<t> *n = root;
	int value = index;
	int r;

	while (n)
	{
		r = 1;
		if (n->left)
		{
			r += n->left->size;
		}

		if (value == r)
		{
			return n;
		}
		else if (value < r)
		{
			n = n->left;
		}
		else
		{
			n = n->right;
			value -= r;
		}
	}

	return n;
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
	std::cout << node->value << (node->colour ? "B" : "R") << node->size << std::endl;

	print(node->right, tabs + 1);
}

template<class t>
void Tree<t>::print()
{
	print(root, 0);
}

template<class t>
void Tree<t>::updateSize(Node<t> *node)
{
	if (node)
	{
		node->size = 1;
		if (node->left)
		{
			node->size += node->left->size;
		}
		if (node->right)
		{
			node->size += node->right->size;
		}
		if (node->parent)
		{
			if (node->parent->size != (node->parent->left ? node->parent->left->size : 0) + (node->parent->right ? node->parent->right->size : 0) + 1)
			{
				updateSize(node->parent);
			}
		}
	}
}

template<class t>
bool Tree<t>::isEmpty()
{
	return (root == nullptr);
}

template<class t>
int Tree<t>::getNrOp()
{
	return nrOp;
}
