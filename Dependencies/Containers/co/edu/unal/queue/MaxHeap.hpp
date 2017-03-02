#ifndef MAXHEAP_HPP_INCLUDED
#define MAXHEAP_HPP_INCLUDED
#include "MaxPriorityQueue.hpp"
#include <vector>
#include "../comp/Comp.hpp"
#include "../list/LinearList.hpp"

//template <typename T> struct Comp;

template <typename T, class Compare = Comp<T>> class MaxHeap: public MaxPriorityQueue<T>
{
private:
	T* heap;
	int size;
	int length;
	Compare c;
public:
	MaxHeap (int initial_capacity)
	{
		if (initial_capacity <= 0)
			initial_capacity = 1;
		heap = new T[initial_capacity + 1];
		size = 0;
		length = initial_capacity;
	}
	MaxHeap() : MaxHeap (10)
	{
	}
	bool isEmpty() override
	{
		return size == 0;
	}
	int getSize() override
	{
		return size;
	}
	T getMax() override
	{
		return (isEmpty() ? ( (const T) nullptr) : heap[1]);
	}
	void put (T element) override
	{
		if (size == length - 1)
		{
			T* old = heap;
			heap =  new T[2 * length];
			for (int i = 0; i < length; i++)
			{
				heap[i] = old[i];
			}
			length *= 2;
		}
		int currentNode = ++ size;
		while (currentNode != 1 && c (heap[currentNode / 2], element))
		{
			heap[currentNode] = heap[currentNode / 2];
			currentNode /= 2;
		}
		heap[currentNode] = element;
	}

	T removeMax() override
	{
		if (isEmpty()) return (const T) nullptr;
		T maxElement = heap[1];
		T lastElement = heap[size--];
		int currentNode = 1,
			child = 2;
		while (child <= size)
		{
			if (child < size && c (heap[child], heap[child + 1]))
				child ++;
			if (!c (lastElement, heap[child]))
				break;
			heap[currentNode] = heap[child];
			currentNode = child;
			child *= 2;
		}
		heap[currentNode] = lastElement;
		return maxElement;
	}

	void initialize (T* theHeap, int t_size)
	{
		int theSize = t_size;
		std::cout << "the size is: " << theSize << std::endl;
		heap = new T[theSize + 1];
		for (int i = 1; i <= theSize; i ++)
		{
			heap[i] = theHeap[i - 1];
		}
		size = theSize;
		for (int root = size / 2; root >= 1; root --)
		{
			T rootElement = heap[root];
			int child = 2 * root;
			while (child <= size)
			{
				if (child < size && c (heap[child], heap[child + 1]))
					child ++;
				if (!c (rootElement, heap[child]))
					break;
				heap[child / 2] = heap[child];
				child *= 2;
			}
			heap[child / 2] = rootElement;
		}
	}

	void initialize (JList::LinearList<T>* theHeap)
	{
		int theSize = theHeap->getSize();
		std::cout << "the size is: " << theSize << std::endl;
		heap = new T[theSize + 1];
		for (int i = 1; i <= theSize; i ++)
		{
			heap[i] = theHeap->get (i - 1);
		}
		size = theSize;
		for (int root = size / 2; root >= 1; root --)
		{
			T rootElement = heap[root];
			int child = 2 * root;
			while (child <= size)
			{
				if (child < size && c (heap[child], heap[child + 1]))
					child ++;
				if (!c (rootElement, heap[child]))
					break;
				heap[child / 2] = heap[child];
				child *= 2;
			}
			heap[child / 2] = rootElement;
		}
	}
	void initialize (const std::vector<T>& theHeap)
	{
		int theSize = theHeap.size();
		std::cout << "the size is: " << theSize << std::endl;
		heap = new T[theSize + 1];
		for (int i = 1; i <= theSize; i ++)
		{
			heap[i] = theHeap[i - 1];
		}
		size = theSize;
		for (int root = size / 2; root >= 1; root --)
		{
			T rootElement = heap[root];
			int child = 2 * root;
			while (child <= size)
			{
				if (child < size && c (heap[child], heap[child + 1]))
					child ++;
				if (!c (rootElement, heap[child]))
					break;
				heap[child / 2] = heap[child];
				child *= 2;
			}
			heap[child / 2] = rootElement;
		}
	}
	~MaxHeap()
	{
		delete [] heap;
	}
};

#endif // MAXHEAP_HPP_INCLUDED
