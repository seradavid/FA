#pragma once

template<class T>
class Node
{
public:
	T value;
	Node *next;

	Node()
	{
		value = 0;
		next = nullptr;
	}

	Node(T value)
	{
		this->value = value;
		next = nullptr;
	}
};
/*
template<class T>
inline bool operator==(const Node<T>& lhs, const Node<T>& rhs)
{
	return (lhs.value == rhs.value && lhs.next == rhs.next);
}

template<class T>
inline bool operator!=(const Node<T>& lhs, const Node<T>& rhs)
{
	return !operator==(lhs, rhs);
}
*/
template<class T>
inline bool operator<(const Node<T>& lhs, const Node<T>& rhs)
{
	return (lhs.value < rhs.value);
}

template<class T>
inline bool operator<=(const Node<T>& lhs, const Node<T>& rhs)
{
	return !operator>(lhs, rhs);
}

template<class T>
inline bool operator>(const Node<T>& lhs, const Node<T>& rhs)
{
	return operator<(rhs, lhs);
}

template<class T>
inline bool operator>=(const Node<T>& lhs, const Node<T>& rhs)
{
	return !operator<(lhs, rhs);
}