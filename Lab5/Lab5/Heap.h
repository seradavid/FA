#pragma once

template<class T>
class Heap
{
private:
	T value;
	Heap *parent;
	Heap *left;
	Heap *right;

	void heapify();

public:
	Heap()
	{
		size = 0;
		parent = nullptr;
		left = nullptr;
		right = nullptr;
	}

	Heap(T value)
	{
		this->value = value;
		parent = nullptr;
		left = nullptr;
		right = nullptr;
	}

	void insert(const T& value);
	void remove();
	T peek();
};

template<class T>
Heap<T> top;

template<class T>
Heap<T>last;