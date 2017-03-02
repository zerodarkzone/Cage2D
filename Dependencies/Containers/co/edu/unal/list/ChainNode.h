// *** ADDED BY HEADER FIXUP ***
#include <iostream>
// *** END ***
#ifndef CHAINNODE_H
#define CHAINNODE_H

namespace JList
{

	template <typename T> class ChainNode
	{
	public:
		T element;
		ChainNode<T>* next;
		ChainNode<T>* prev;

		ChainNode (T elem, ChainNode<T>* n, ChainNode<T>* p)
		{
			this->element = elem;
			this->next = n;
			this->prev = p;
		}

		ChainNode (T elem, ChainNode<T>* c)
		{
			this->element = elem;
			this->next = c;
			this->prev = nullptr;
		}
		ChainNode() : ChainNode ( T(), nullptr) { }
		ChainNode (T elem) : ChainNode (elem, nullptr) { }
		ChainNode (ChainNode<T>* c) : ChainNode (nullptr, c) { }
		~ChainNode()
		{
			//std::cout << "node Destroyed \n";
		}
	private:
	};

	template <typename T> class ChainNode<T*>
	{
	public:
		T* element;
		ChainNode<T*>* next;
		ChainNode<T*>* prev;

		ChainNode (T* elem, ChainNode<T*>* n, ChainNode<T*>* p)
		{
			this->element = elem;
			this->next = n;
			this->prev = p;
		}

		ChainNode (T* elem, ChainNode<T*>* c)
		{
			this->element = elem;
			this->next = c;
			this->prev = nullptr;
		}
		ChainNode() : ChainNode ( (T*) nullptr, nullptr) { }
		ChainNode (T* elem) : ChainNode (elem, nullptr) { }
		ChainNode (ChainNode<T*>* c) : ChainNode (nullptr, c) { }
		~ChainNode()
		{
			//std::cout << "node Destroyed \n";
		}
	private:
	};

}

#endif // CHAINNODE_H
