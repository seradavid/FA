#include "Heap.h"

template<class T>
void Heap<T>::insert(const T& value)
{

}

template<class T>
void Heap<T>::heapify()
{

}

template<class T>
void Heap<T>::remove()
{
	if (top == nullptr)
	{
		return;
	}

	std::swap(top, last);

	Heap<T> *tmp = last;
	last = last.parent;
	delete tmp;
}

template<class T>
T Heap<T>::peek()
{
	return top;
}