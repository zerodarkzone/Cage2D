#ifndef JCOMPARATOR_HPP_INCLUDED
#define JCOMPARATOR_HPP_INCLUDED
#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(P){ (void)(P);}
#endif
#include "..\..\queue\MaxHeap.hpp"
namespace JList
{
	template <typename T> class ChainNode;
}
using JList::ChainNode;
namespace JComparator
{
	/**
	*  in this moment, this abstract class makes that the elements on the list
	*   cannot be saved
	*/
	template <class T> class Comparable
	{
	public:
		int compareTo (T o);
		~Comparable() {}
	};

	template <class T>int Comparable<T>:: compareTo (T o)
	{
		UNREFERENCED_PARAMETER (o);
		throw std::invalid_argument ("The object to be compared must define compareTo method");
	}
	template <class T> class Comparator
	{
	public:
		virtual int compare (T o1 , T o2) = 0;
		virtual bool equals (T obj)
		{
			UNREFERENCED_PARAMETER (obj);
			return false;
		}
		virtual ~Comparator() {}
	};

	template <class T> class ArraySort
	{
	private:
		static int part (T* input, int fromIndex, int toIndex)
		{
			T pivot = input[toIndex];
			while (fromIndex < toIndex)
			{
				while (input[fromIndex]->compareTo (pivot) < 0)
					fromIndex++;
				while (input[toIndex]->compareTo (pivot) > 0)
					toIndex--;
				if (input[fromIndex]->compareTo (input[toIndex]) == 0)
					fromIndex++;
				else if (fromIndex < toIndex)
				{
					T tmp = input[fromIndex];
					input[fromIndex] = input[toIndex];
					input[toIndex] = tmp;
				}
			}
			return toIndex;
		}
		static int part (T* input, int fromIndex, int toIndex, Comparator<T>* c)
		{
			T pivot = input[toIndex];
			while (fromIndex < toIndex)
			{
				while (c->compare (input[fromIndex], pivot) < 0)
					fromIndex++;
				while (c->compare (input[toIndex], pivot) > 0)
					toIndex--;
				if (c->compare (input[fromIndex], input[toIndex]) == 0)
					fromIndex++;
				else if (fromIndex < toIndex)
				{
					T tmp = input[fromIndex];
					input[fromIndex] = input[toIndex];
					input[toIndex] = tmp;
				}
			}
			return toIndex;
		}
	public:
		static void quick_sort (T* input, int fromIndex, int toIndex, Comparator<T>* c)
		{
			if (fromIndex < toIndex)
			{
				int j = part (input, fromIndex, toIndex, c);
				quick_sort (input, fromIndex, j - 1, c);
				quick_sort (input, j + 1, toIndex, c);
			}
		}
		static void heap_sort (T* input, int _size)
		{
			MaxHeap<T> h;
			h.initialize (input, _size);
			for (int i = _size - 1; i >= 0 ; --i)
			{
				input[i] = h.removeMax();
			}
		}
		template<typename C>static void _heap_sort (T* input, int _size)
		{
			MaxHeap<T, C> h;
			h.initialize (input, _size);
			for (int i = _size - 1; i >= 0 ; --i)
			{
				input[i] = h.removeMax();
			}
		}
		static void heap_sort (JList::LinearList<T>& _list, int _size)
		{
			MaxHeap<T> h;
			h.initialize (&_list);
			_list.clear();
			for (int i = 0; i < _size ; ++i)
			{
				_list.push_front (h.removeMax());
			}
		}
		template<typename C>static void _heap_sort (JList::LinearList<T>& _list, int _size)
		{
			MaxHeap<T, C> h;
			h.initialize (&_list);
			_list.clear();
			for (int i = 0; i < _size ; ++i)
			{
				_list.push_front (h.removeMax());
			}
		}
		static void quick_sort (T* input, int fromIndex, int toIndex)
		{
			if (fromIndex < toIndex)
			{
				int j = part (input, fromIndex, toIndex);
				quick_sort (input, fromIndex, j - 1);
				quick_sort (input, j + 1, toIndex);
			}
		}

		static void sortArrayList (T* a, int fromIndex, int toIndex)
		{
			T x = (T) nullptr;
			int j;
			for (int i = 1; i < toIndex; i++)
			{
				j = i;
				while (j > fromIndex && a[j - 1]->compareTo (a[j]) > 0)
				{
					x = a[j];
					a[j] = a[j - 1];
					a[j - 1] = x;
					--j;
				}
			}
			x = nullptr;
			delete x;
		}

		static void sortArrayList (T* a, int fromIndex, int toIndex, Comparator<T>* c)
		{
			T x = (T) nullptr;
			int j;
			for (int i = 1; i < toIndex; i++)
			{
				j = i;
				while (j > fromIndex && c->compare (a[j - 1], a[j]) > 0)
				{
					x = a[j];
					a[j] = a[j - 1];
					a[j - 1] = x;
					--j;
				}
			}
			delete c;
		}

		static void primitiveArraySort (T* a, int fromIndex, int toIndex)
		{
			T x;
			int j;
			for (int i = 1; i < toIndex; i++)
			{
				j = i;
				while (j > fromIndex && a[j - 1]  > a[j])
				{
					x = a[j];
					a[j] = a[j - 1];
					a[j - 1] = x;
					--j;
				}
			}
		}
	};

	template <class T> class ChainSort
	{
	public:
		static void primitiveSort (ChainNode<T>* first, int fromIndex, int toIndex)
		{
			ChainNode<T>* p = nullptr;
			ChainNode<T>* q = nullptr;
			T x = 0;
			int j = 0;
			for (int i = 1; i < toIndex; i++)
			{
				j = i;
				p = first;
				q = first;
				for (int h = 0; h < j; h++)
					p = p->next;
				for (int k = 0; k < (j + 1); k++)
					q = q->next;
				while (j > fromIndex && p->element > q->element)
				{
					x = q->element;
					q->element = p->element;
					p->element = x;
					--j;
					p = first;
					q = first;
					for (int h = 0; h < j; h++)
						p = p->next;
					for (int k = 0; k < (j + 1); k++)
						q = q->next;
				}
			}
			p = nullptr;
			q = nullptr;
			delete p;
			delete q;
		}
		static void objectSort (ChainNode<T>* first, int fromIndex, int toIndex)
		{
			ChainNode<T>* p = nullptr;
			ChainNode<T>* q = nullptr;
			T x = nullptr;
			int j = 0;
			for (int i = 1; i < toIndex; i++)
			{
				j = i;
				p = first;
				q = first;
				for (int h = 0; h < j; h++)
					p = p->next;
				for (int k = 0; k < (j + 1); k++)
					q = q->next;
				while (j > fromIndex && p->element->compareTo (q->element) > 0)
				{
					x = q->element;
					q->element = p->element;
					p->element = x;
					--j;
					p = first;
					q = first;
					for (int h = 0; h < j; h++)
						p = p->next;
					for (int k = 0; k < (j + 1); k++)
						q = q->next;
				}
			}
			p = nullptr;
			q = nullptr;
			x = nullptr;
			delete x;
			delete p;
			delete q;
		}

		static void objectSort (ChainNode<T>* first, int fromIndex, int toIndex, Comparator<T>* c)
		{
			ChainNode<T>* p = nullptr;
			ChainNode<T>* q = nullptr;
			T x = (T) nullptr;
			int j = 0;
			for (int i = 1; i < toIndex; i++)
			{
				j = i;
				p = first;
				q = first;
				for (int h = 0; h < j; h++)
					p = p->next;
				for (int k = 0; k < (j + 1); k++)
					q = q->next;
				while (j > fromIndex && c->compare (p->element, q->element) > 0.0)
				{
					x = q->element;
					q->element = p->element;
					p->element = x;
					--j;
					p = first;
					q = first;
					for (int h = 0; h < j; h++)
						p = p->next;
					for (int k = 0; k < (j + 1); k++)
						q = q->next;
				}
			}
			p = nullptr;
			q = nullptr;
			x = nullptr;
			delete x;
			delete p;
			delete q;
		}
	};

}


#endif // JCOMPARATOR_HPP_INCLUDED
