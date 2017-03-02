// *** ADDED BY HEADER FIXUP ***
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
// *** END ***
#ifndef CIRCLECHAINEDLIST_H
#define CIRCLECHAINEDLIST_H

#include "LinearList.hpp"
#include "ChainNode.h"


namespace JList
{
	/**
	*   DoubleChaindedList
	*   Java like Container list class which uses a circular double linked list
	*   with a header and a tail to store elements
	*   This class has some utility functions, like sort, shuffle, save and load
	*   save and load does NOT work with elements which contains virtual functions
	*   Contains two different types of iterators, one similar to the ones you find
	*   in java, and the other similar to what you see in c++. Both can be used to
	*   traverse the list in reverse
	*   In some scenarios this container is slightly faster than the stl list
	*/
	template <typename T> class DoubleChainedList : public LinearList<T>, public LinearListImproved<T>, public JIterator::Iterable<T>
	{
	public:
		template <typename I> class ChainIterator;
		template<typename C>class MyIterator;
		template<typename C>class MyReverseIterator;
		typedef T _type;
		/**
		*   Constructor, takes no arguments
		*/
		DoubleChainedList()
		{
			headerNode = new ChainNode<T>();
			tailNode = new ChainNode<T>();
			headerNode->next = tailNode;
			headerNode->prev = tailNode;
			headerNode->element = T();
			tailNode->next = headerNode;
			tailNode->prev = headerNode;
			_size = 0;
			srand ( time ( NULL ) );
			sv = nullptr;
			lod = nullptr;
			ele = nullptr;
			elo = nullptr;
		}
		/**
		*   Copy Constructor
		*/
		DoubleChainedList ( const DoubleChainedList<T> &_list ) : DoubleChainedList()
		{
			for ( auto i : _list )
			{
				this->push_back ( i );
			}
		}
		/**
		*   bool isEmpty()
		*   returns whether or not the list is empty
		*/
		bool isEmpty() override
		{
			return _size == 0;
		}
		bool isEmpty() const
		{
			return _size == 0;
		}
		/**
		*   int getSize()
		*   returns current _size of the list
		*/
		int getSize() override
		{
			return _size;
		}
		int getSize() const
		{
			return _size;
		}
		/**
		*   T get(int index)
		*   returns list element on given index
		*/
		T &get ( int index ) override
		{
			checkIndex ( index );
			ChainNode<T> *p = headerNode->next;
			ChainNode<T> *q = tailNode->prev;
			int r_goal = _size - index - 1;
			int goal = index;
			for ( int i = 0, j = 0; i < goal && j < r_goal; i++ , j++ )
			{
				p = p->next;
				q = q->prev;
			}
			if ( index < _size / 2 )
			{
				return p->element;
			}
			return q->element;
		}
		/**
		*   Copy assignment operator
		*/
		DoubleChainedList &operator= ( const DoubleChainedList<T> &_list )
		{
			this->clear();
			for ( auto i : _list )
			{
				this->push_back ( i );
			}
			return *this;
		}
		/**
		*   Operator ==
		*/
		bool operator== ( const DoubleChainedList<T> &lt ) const
		{
			if ( this->_size != lt._size )
			{
				return false;
			}

			ChainNode<T> *currentNode = headerNode->next;
			ChainNode<T> *lt_Node = lt.headerNode->next;
			for ( int i = 0; i < _size; i++ )
			{
				if ( currentNode->element != lt_Node->element )
				{
					return false;
				}
				currentNode = currentNode->next;
				lt_Node = lt_Node->next;
			}
			return true;
		}
		/**
		*   int indexOf(T elem)
		*   returns index of given element
		*/
		int indexOf ( const T &theElement ) override
		{
			tailNode->element = theElement;
			int index = 0;
			ChainNode<T> *currentNode = headerNode->next;
			//if (currentNode == nullptr)
			//std::cout << "NULO";
			while ( currentNode->element != theElement )
			{
				//if (currentNode == nullptr)
				//std::cout << "NULO";
				currentNode = currentNode->next;
				index ++;
			}
			if ( currentNode == tailNode )
			{
				return -1;
			}
			else
			{
				return index;
			}
		}
		/**
		*   T remove(int index)
		*   removes list element on given index and returns it
		*/

		T remove ( int index ) override
		{
			checkIndex ( index );
			ChainNode<T> *p = headerNode;
			ChainNode<T> *q = tailNode;
			int goal = index;
			int r_goal = _size - index - 1;
			for ( int i = 0, j = 0; i < goal && j < r_goal; ++i, ++j )
			{
				p = p->next;
				q = q->prev;
			}
			if ( index < _size / 2 )
			{
				T removed = p->next->element;
				ChainNode<T> *t = p->next;
				p->next = p->next->next;
				p->next->prev = p;
				delete t;
				_size --;
				return removed;
			}
			else
			{
				T removed = q->prev->element;
				ChainNode<T> *t = q->prev;
				q->prev = q->prev->prev;
				q->prev->next = q;
				delete t;
				_size --;
				return removed;
			}
		}
		/**
		*   Removes the element pointed by the iterator
		*/
		T remove ( MyIterator<T> &iter )
		{
			ChainNode<T> *p = iter.node;
			T removed = ( *iter );
			--iter;
			p->prev->next = p->next;
			p->next->prev = p->prev;
			delete p;
			_size --;
			return removed;
		}
		/**
		*   Removes the element pointed by the iterator
		*/
		T remove ( MyReverseIterator<T> &iter )
		{
			ChainNode<T> *p = iter.node;
			T removed = ( *iter );
			--iter;
			p->prev->next = p->next;
			p->next->prev = p->prev;
			delete p;
			_size --;
			return removed;
		}
		/**
		*   T ObjectRemove(T elem)
		*   search the list for the given element and removes it
		*/
		T ObjectRemove ( const T &elem )
		{
			T removed = elem;
			if ( indexOf ( elem ) >= 0 )
			{
				remove ( indexOf ( elem ) );
				return removed;
			}
			std::cout << "The element Does not Exist\n";
			return removed;
		}
		/**
		*   T ObjectRemoveI(T elem)
		*   search the list for the given element and removes it.
		*   This method uses two pointers to traverse the list and
		*   look for the object.
		*   (worst case scenario is when the object is in the middle of the list)
		*   Note: Has not been strongly tested
		*/
		T ObjectRemoveI ( T elem )
		{
			T removed;
			tailNode->element = elem;
			headerNode->element = elem;
			ChainNode<T> *p = headerNode->next;
			ChainNode<T> *pb = headerNode;
			ChainNode<T> *q = tailNode->prev;
			ChainNode<T> *qb = tailNode;
			bool p_found = false;
			bool q_found = false;
			while ( true )
			{
				if ( p->element == elem )
				{
					//std::cout << "P \n";
					p_found = true;
					break;
				}
				else if ( q->element == elem )
				{
					//std::cout << "Q \n";
					q_found = true;
					break;
				}
				pb = p;
				p = p->next;
				qb = q;
				q = q->prev;
			}
			if ( p == tailNode || q == headerNode )
			{
				std::cout << "The element Does not Exist\n";
				return ( T ) nullptr;
			}
			if ( p_found )
			{
				removed = p->element;
				pb->next = pb->next->next;
				pb->next->prev = pb;
				_size --;
				delete p;
			}
			else if ( q_found )
			{
				removed = q->element;
				qb->prev = qb->prev->prev;
				qb->prev->next = qb;
				_size --;
				delete q;
			}
			return removed;
		}
		/**
		*   void add(int index,T elem)
		*   adds element to the list in the given index
		*/
		void add ( int index, const T &elem ) override
		{
			if ( index < 0 || index > _size )
			{
				throw except::Index_out_of_range ( "IndexOutOfBounds" );
			}
			if ( index < _size / 2 )
			{
				ChainNode<T> *p = headerNode;
				for ( int i = 0; i < index; i++ )
				{
					p = p->next;
				}
				p->next = new ChainNode<T> ( elem , p->next, p );
				ChainNode<T> *q = p->next;
				p = p->next->next;
				p->prev = q;
				p = nullptr;
				q = nullptr;
			}
			else
			{
				ChainNode<T> *p = tailNode;
				for ( int i = 0; i < _size - index; i++ )
				{
					p = p->prev;
				}
				p->prev = new ChainNode<T> ( elem, p, p->prev );
				ChainNode<T> *q = p->prev;
				p = p->prev->prev;
				p->next = q;
				p = nullptr;
				q = nullptr;
			}
			++ _size;
		}
		/**
		*   void add(ChainIterator<T>* iter)
		*   adds the element in the position of the current iterator
		*   (The position is in the next element)
		*/
		void add ( ChainIterator<T> &iter, T elem )
		{
			ChainNode<T> *p = iter.nextNode->prev;
			p->next = new ChainNode<T> ( elem , p->next, p );
			ChainNode<T> *q = p->next;
			p = p->next->next;
			p->prev = q;
			++ _size;
		}
		/**
		*   void add(ChainIterator<T>* iter)
		*   adds the element in the position of the current iterator
		*/
		void add ( MyIterator<T> &iter, T elem )
		{
			ChainNode<T> *p = iter.node->prev;
			p->next = new ChainNode<T> ( elem , p->next, p );
			ChainNode<T> *q = p->next;
			p = p->next->next;
			p->prev = q;
			++ _size;
		}
		void add ( MyReverseIterator<T> &iter, T elem )
		{
			ChainNode<T> *p = iter.node->next;
			p->prev = new ChainNode<T> ( elem, p, p->prev );
			ChainNode<T> *q = p->prev;
			p = p->prev->prev;
			p->next = q;
			++ _size;
		}
		/**
		*   removes all elements from the list
		*   Note: this function does NOT release the memory
		*/
		void clear() override
		{
			while ( _size > 0 )
			{
				pop_front();
			}
			//_size = 0;
			//std::cout << "ChainedList Cleared \n";
		}
		/**
		*   void pop_back()
		*   removes last element on the list and returns it
		*/
		T pop_back() override
		{
			return remove ( _size - 1 );
		}
		/**
		*   void pop_front()
		*   removes first element on the list and returns it
		*/
		T pop_front() override
		{
			return remove ( 0 );
		}
		/**
		*   void push_back(T elem)
		*   adds given element to the end of the list
		*/
		void push_back ( const T &elem ) override
		{
			add ( _size, elem );
		}
		/**
		*   void push_front(T elem)
		*   adds given element to the beginning of the list
		*/
		void push_front ( const T &elem ) override
		{
			add ( 0, elem );
		}

		/**
		*   void sort()
		*   Orders the list using the "natural" comparison of the objects contained by the list
		*   the elements must inherit the class Comparable from JComparator namespace and implement
		*   compareTo method
		*/
		void sort()
		{
			JComparator::ChainSort<T>::objectSort ( headerNode, 0, this->_size );
		}

		/**
		*   void sort(JComparator::Comparator<T>)
		*   Orders the list using a user defined comparator
		*/
		void sort ( JComparator::Comparator<T> *c )
		{
			JComparator::ChainSort<T>::objectSort ( headerNode, 0, this->_size, c );
		}

		/**
		*   void pSort()
		*   Orders the list in ascending order, this must be used instead of sort()
		*   to order primitive type of elements
		*/
		void psort()
		{
			JComparator::ChainSort<T>::primitiveSort ( headerNode, 0, this->_size );
		}

		virtual void print()
		{
			JIterator::Iterator<T> *it;
			for ( it = iterator(); it->hasNext(); )
			{
				T t = it->next();
				if ( std::is_pointer<T>::value )
				{
					std::cout << "*";
				}
				std::cout << "[" << t << "]  ";
			}
			if ( _size <= 0 )
			{
				std::cout << "The list is empty";
			}
			std::cout << "\n";
			delete it;
		}

		void shuffle()
		{
			for ( int i = 0; i < this->_size; i++ )
			{
				int j = randInt ( i, this->_size );
				swap ( j, i );
			}
		}
		/**
		*   void save(char *)
		*   saves the list items to a given file, if the file does not exist, it gets created
		*/
		void save ( std::string path ) override
		{
			sv = &this->_size;
			//Saves the current size of the list to binary File
			std::ofstream fsalida ( path,
									std::ios::out | std::ios::binary );
			fsalida.write ( reinterpret_cast<char *> ( sv ),
							sizeof ( int ) );
			//Saves the elements in the list to binary File
			for ( auto _ele : *this )
			{
				fsalida.write ( reinterpret_cast<char *> ( &_ele ),
								sizeof ( T ) );
			}
			fsalida.close();
			std::cout << "Save done :) " << std::endl;
		}

		/**
		*   void load(char *)
		*   loads all elements stored into a file and adds them to the list
		*   if the file does not exist or if it is empty, a runtime_error exception is thrown
		*/
		void load ( std::string path ) override
		{
			lod = new int;
			std::ifstream fentrada ( path,
									 std::ios::in | std::ios::binary | std::ios::ate );
			if ( !fentrada || fentrada.tellg() <= 0 )
			{
				throw std::runtime_error ( "Unable to find file or file is empty" );
			}
			fentrada.close();
			fentrada.open ( path, std::ios::in | std::ios::binary );
			fentrada.read ( reinterpret_cast<char *> ( lod ),
							sizeof ( int ) );
			//this->clear();
			//if Q is not a pointer
			{
				for ( int i = 0; i < *lod; i++ )
				{
					elo = new T;
					fentrada.read ( reinterpret_cast<char *> ( elo ),
									sizeof ( T ) );
					this->push_back ( *elo );
					elo = nullptr;
				}
			}
			fentrada.close();
			std::cout << "Load done <:-{D" << std::endl;
		}

		~DoubleChainedList()
		{
			clear();
			if ( headerNode )
			{
				delete headerNode;
			}
			if ( tailNode )
			{
				delete tailNode;
			}
			this->sv = nullptr;
			this->lod = nullptr;
			this->ele = nullptr;
			this->elo = nullptr;
			std::cout << "ChainedList Cleared \n";
		}


		/**
		*   Custom iterator class created for traverse the list
		*   Note: this class is not like a c++ standard iterator
		*   works with the three custom macros created for traverse the list
		*/
		template <typename I> class ChainIterator: public JIterator::Iterator<I>
		{
			friend class DoubleChainedList;
		private:
			DoubleChainedList<I> *list;
			ChainNode<I> *nextNode;
			bool isRev;

			//ChainNode<I>* currentNode;
		public:
			ChainIterator ( DoubleChainedList<I> *_list, bool _isRev = false ) :
				list ( _list ), nextNode ( !_isRev ? _list->headerNode->next : _list->tailNode->prev ), isRev ( _isRev )
			{
			}
			bool hasNext() override
			{
				return nextNode != list->tailNode;
			}
			bool hasPrev()
			{
				return nextNode != list->headerNode;
			}
			I &next() override
			{
				if ( nextNode == list->tailNode )
				{
					throw except::No_such_element ( "Element does not exist" );
				}
				else
				{
					//std::cout<<nextNode->element<<std::endl;
					I &elementToReturn = nextNode->element;
					nextNode = nextNode->next;
					return elementToReturn;
				}
			}
			I prev()
			{
				if ( nextNode == list->headerNode )
				{
					throw except::No_such_element ( "Element does not exist" );
				}
				else
				{
					//std::cout<<nextNode->element<<std::endl;
					I elementToReturn = nextNode->element;
					nextNode = nextNode->prev;
					return elementToReturn;
				}
			}

			I &current() override
			{
				throw except::Unsuported_operation ( "Operation not supported" );
			}

			void remove() override
			{
				//throw except::Unsuported_operation ("Operation not yet supported");
				if ( !isRev )
				{
					ChainNode<I> *p = nextNode->prev;
					if ( p == list->headerNode )
					{
						except::No_such_element ( "Trying to delete an element that doesn't exist" );
					}
					ChainNode<I> *q = nextNode->prev->prev;
					q->next = nextNode;
					nextNode->prev = q;
					list->_size -= 1;
					delete p;
				}
				else
				{
					ChainNode<I> *p = nextNode->next;
					if ( p == list->tailNode )
					{
						except::No_such_element ( "Trying to delete an element that doesn't exist" );
					}
					ChainNode<I> *q = nextNode->next->next;
					q->prev = nextNode;
					nextNode->next = q;
					list->_size -= 1;
					delete p;
				}
			}
			~ChainIterator()
			{
				//this->nextNode = nullptr;
			}
		};
		/**
		*   ChainIterator<T>* iterator()
		*   creates an ChainIterator<T>
		*/
		JIterator::Iterator<T> *iterator() override
		{
			JIterator::Iterator<T> *it = new ChainIterator<T> ( this );
			return it;
		}
		/**
		*   Creates a custom made iterator for traversal
		*/
		ChainIterator<T> _iterator ()
		{
			return ChainIterator<T> ( this );
		}
		/**
		*   Creates a custom made iterator for reverse traversal
		*/
		ChainIterator<T> reverse_iterator()
		{
			ChainIterator<T> it ( this, true );
			return it;
		}
		/**
		*   Custom iterator class created for traverse the list
		*   this class is like a c++ standard iterator
		*   it can be used with for_each and with a range based loop
		*/
		template<typename C>class MyIterator
		{
			friend class DoubleChainedList;
		protected:
			const DoubleChainedList<C> *list;
			ChainNode<C> *node;

		public:
			MyIterator() : list( nullptr ), node( nullptr ) {}
			MyIterator ( const DoubleChainedList<C> *_list, bool is_end = false ) :
				list ( _list ), node ( !is_end ? _list->headerNode->next : _list->tailNode )
			{}
			MyIterator ( const MyIterator &other ) : MyIterator ( other.list )
			{
				node = other.node;
			}
			bool operator == ( const MyIterator &other )
			{
				return node == other.node;
			}
			bool operator != ( const MyIterator &other )
			{
				return node != other.node;
			}
			const MyIterator &operator++()
			{
				node = node->next;
				return *this;
			}
			const MyIterator &operator--()
			{
				node = node->prev;
				return *this;
			}
			T &operator*() const
			{
				return node->element;
			}
		};
		template<typename C>class MyReverseIterator : public MyIterator<C>
		{
			friend class DoubleChainedList;
		protected:

		public:
			MyReverseIterator() : MyIterator<C>() {}
			MyReverseIterator ( const DoubleChainedList<C> *_list, bool is_end = false ) :
				MyIterator<C> ( _list, is_end )
			{
				this->node = ( is_end ? _list->headerNode : _list->tailNode->prev );
			}
			MyReverseIterator ( const MyIterator<C> &other ) : MyReverseIterator ( other.list )
			{
				this->node = other.node;
			}
			bool operator == ( const MyIterator<C> &other )
			{
				return this->node == other.node;
			}
			bool operator != ( const MyIterator<C> &other )
			{
				return this->node != other.node;
			}
			const MyIterator<C> &operator++()
			{
				this->node = this->node->prev;
				return *this;
			}
			const MyIterator<C> &operator--()
			{
				this->node = this->node->next;
				return *this;
			}
			T &operator*() const
			{
				return this->node->element;
			}
		};
		/**
		*   Returns an iterator pointing to the beginning of the list
		*/
		MyIterator<T> begin () const
		{
			return MyIterator<T> ( this );
		}
		/**
		*   Returns an iterator pointing to the end of the list
		*/
		MyIterator<T> end () const
		{
			return MyIterator<T> ( this, true );
		}
		/**
		*   Returns an iterator pointing to the end of the list
		*/
		MyReverseIterator<T> rbegin () const
		{
			return MyReverseIterator<T> ( this );
		}
		/**
		*   Returns an iterator pointing to the beginning of the list
		*/
		MyReverseIterator<T> rend () const
		{
			return MyReverseIterator<T> ( this, true );
		}
		/**
		*   Overload of the << operator to print all the elements on the list
		*/
		friend std::ostream &operator<< ( std::ostream &out, DoubleChainedList<T> *list )
		{
			list->osPrint ( out );
			return out;
		}
		friend std::ostream &operator<< ( std::ostream &out, DoubleChainedList<T> &list )
		{
			list.osPrint ( out );
			return out;
		}

	protected:
		/**
		*   void osPrint(std::ostream& out)
		*   prints the list to an ostream object
		*/
		virtual void osPrint ( std::ostream &out )
		{
			for ( T elem : *this )
			{
				out << "[" << elem << "]  ";
			}
			if ( _size <= 0 )
			{
				out << "The list is empty";
			}
			out << "\n";
		}

	private:
		int *sv;
		int *lod;
		T *ele;
		T *elo;
		int _size;
		ChainNode<T> *headerNode;
		ChainNode<T> *tailNode;
		void checkIndex ( int index )
		{
			if ( index < 0 || index >= _size )
			{
				throw except::Index_out_of_range ( "IndexOutOfBounds" );
			}
		}
		/**
		*   interchanges two elements from the list with the given
		*   indexes
		*/
		void swap ( int index, int index2 )
		{
			checkIndex ( index );
			checkIndex ( index2 );
			T elem;
			ChainNode<T> *p = headerNode;
			ChainNode<T> *q = headerNode;
			for ( int i = 0; i < index ; i++ )
			{
				p = p->next;
			}
			for ( int i = 0; i < index2 ; i++ )
			{
				q = q->next;
			}
			elem = p->next->element;
			p->next->element = q->next->element;
			q->next->element = elem;
			p = nullptr;
			q = nullptr;
		}

		int randInt ( int min, int max )
		{
			int randomNum = min + rand() % ( max - min );
			return randomNum;
		}
	};
}

namespace JList
{
	/**
	*   DoubleChaindedList
	*   Java like Container list class which uses a circular double linked list
	*   with a header and a tail to store elements
	*   This class has some utility functions, like sort, shuffle, save and load
	*   save and load does NOT work with elements which contains virtual functions
	*   Contains two different types of iterators, one similar to the ones you find
	*   in java, and the other similar to what you see in c++. Both can be used to
	*   traverse the list in reverse
	*   In some scenarios this container is slightly faster than the stl list
	*/
	template <typename T> class DoubleChainedList<T *> : public LinearList<T *>, public LinearListImproved<T>, public JIterator::Iterable<T *>
	{
	public:
		typedef T *_type;

		template <typename I> class ChainIterator;
		template<typename C>class MyIterator;
		template<typename C>class MyReverseIterator;
		/**
		*   Constructor, takes no arguments
		*/
		DoubleChainedList()
		{
			headerNode = new ChainNode<_type>();
			tailNode = new ChainNode<_type>();
			headerNode->next = tailNode;
			headerNode->prev = tailNode;
			headerNode->element = ( _type ) nullptr;
			tailNode->next = headerNode;
			tailNode->prev = headerNode;
			_size = 0;
			srand ( time ( NULL ) );
			sv = nullptr;
			lod = nullptr;
			ele = nullptr;
			elo = nullptr;
		}
		/**
		*   Copy Constructor
		*/
		DoubleChainedList ( const DoubleChainedList<_type> &_list ) : DoubleChainedList()
		{
			for ( auto i : _list )
			{
				this->push_back ( i );
			}
		}
		/**
		*   bool isEmpty()
		*   returns whether or not the list is empty
		*/
		bool isEmpty() override
		{
			return _size == 0;
		}
		bool isEmpty() const
		{
			return _size == 0;
		}
		/**
		*   int getSize()
		*   returns current _size of the list
		*/
		int getSize() override
		{
			return _size;
		}
		int getSize() const
		{
			return _size;
		}
		/**
		*   T get(int index)
		*   returns list element on given index
		*/
		_type get ( int index ) override
		{
			checkIndex ( index );
			ChainNode<_type> *p = headerNode->next;
			ChainNode<_type> *q = tailNode->prev;
			int r_goal = _size - index - 1;
			int goal = index;
			for ( int i = 0, j = 0; i < goal && j < r_goal; i++ , j++ )
			{
				p = p->next;
				q = q->prev;
			}
			if ( index < _size / 2 )
			{
				return p->element;
			}
			return q->element;
		}
		/**
		*   Copy assignment operator
		*/
		DoubleChainedList &operator= ( const DoubleChainedList<_type> &_list )
		{
			this->clear();
			for ( auto i : _list )
			{
				this->push_back ( i );
			}
			return *this;
		}
		/**
		*   Operator ==
		*/
		bool operator== ( const DoubleChainedList<_type> &lt ) const
		{
			if ( this->_size != lt._size )
			{
				return false;
			}

			ChainNode<_type> *currentNode = headerNode->next;
			ChainNode<_type> *lt_Node = lt.headerNode->next;
			for ( int i = 0; i < _size; i++ )
			{
				if ( currentNode->element != lt_Node->element )
				{
					return false;
				}
				currentNode = currentNode->next;
				lt_Node = lt_Node->next;
			}
			return true;
		}
		/**
		*   int indexOf(T elem)
		*   returns index of given element
		*/
		int indexOf ( _type theElement ) override
		{
			tailNode->element = theElement;
			int index = 0;
			ChainNode<_type> *currentNode = headerNode->next;
			//if (currentNode == nullptr)
			//std::cout << "NULO";
			while ( currentNode->element != theElement )
			{
				//if (currentNode == nullptr)
				//std::cout << "NULO";
				currentNode = currentNode->next;
				index ++;
			}
			if ( currentNode == tailNode )
			{
				return -1;
			}
			else
			{
				return index;
			}
		}
		/**
		*   T remove(int index)
		*   removes list element on given index and returns it
		*/

		_type remove ( int index ) override
		{
			checkIndex ( index );
			_type removed = ( _type ) nullptr;
			ChainNode<_type> *p = headerNode;
			ChainNode<_type> *q = tailNode;
			int goal = index;
			int r_goal = _size - index - 1;
			for ( int i = 0, j = 0; i < goal && j < r_goal; i++ , j++ )
			{
				p = p->next;
				q = q->prev;
			}
			if ( index < _size / 2 )
			{
				removed = p->next->element;
				ChainNode<_type> *t = p->next;
				p->next = p->next->next;
				p->next->prev = p;
				delete t;
			}
			else
			{
				removed = q->prev->element;
				ChainNode<_type> *t = q->prev;
				q->prev = q->prev->prev;
				q->prev->next = q;
				delete t;
			}
			_size --;
			return removed;
		}

		/**
		*   Removes the element pointed by the iterator
		*/
		_type remove ( MyIterator<_type> &iter )
		{
			ChainNode<_type> *p = iter.node;
			_type removed = ( *iter );
			--iter;
			p->prev->next = p->next;
			p->next->prev = p->prev;
			delete p;
			_size --;
			return removed;
		}
		_type remove ( MyReverseIterator<_type> &iter )
		{
			ChainNode<_type> *p = iter.node;
			_type removed = ( *iter );
			--iter;
			p->prev->next = p->next;
			p->next->prev = p->prev;
			delete p;
			_size --;
			return removed;
		}
		/**
		*   T ObjectRemove(T elem)
		*   search the list for the given element and removes it
		*/
		_type ObjectRemove ( _type elem )
		{
			_type removed = elem;
			int index = indexOf ( elem );
			if ( index >= 0 )
			{
				remove ( index );
				return removed;
			}
			std::cout << "The element Does not Exist\n";
			return removed;
		}
		/**
		*   T ObjectRemoveI(T elem)
		*   search the list for the given element and removes it.
		*   This method uses two pointers to traverse the list and
		*   look for the object.
		*   (worst case scenario is when the object is in the middle of the list)
		*   Note: Has not been strongly tested
		*/
		_type ObjectRemoveI ( _type elem )
		{
			T removed = ( _type  ) nullptr;
			tailNode->element = elem;
			headerNode->element = elem;
			ChainNode<_type > *p = headerNode->next;
			ChainNode<_type > *pb = headerNode;
			ChainNode<_type > *q = tailNode->prev;
			ChainNode<_type > *qb = tailNode;
			bool p_found = false;
			bool q_found = false;
			while ( true )
			{
				if ( p->element == elem )
				{
					//std::cout << "P \n";
					p_found = true;
					break;
				}
				else if ( q->element == elem )
				{
					//std::cout << "Q \n";
					q_found = true;
					break;
				}
				pb = p;
				p = p->next;
				qb = q;
				q = q->prev;
			}
			if ( p == tailNode || q == headerNode )
			{
				std::cout << "The element Does not Exist\n";
				return ( _type  ) nullptr;
			}
			if ( p_found )
			{
				removed = p->element;
				pb->next = pb->next->next;
				pb->next->prev = pb;
				_size --;
				delete p;
			}
			else if ( q_found )
			{
				removed = q->element;
				qb->prev = qb->prev->prev;
				qb->prev->next = qb;
				_size --;
				delete q;
			}
			return removed;
		}
		/**
		*   void add(int index,T elem)
		*   adds element to the list in the given index
		*/
		void add ( int index, _type elem ) override
		{
			if ( index < 0 || index > _size )
			{
				throw except::Index_out_of_range ( "IndexOutOfBounds" );
			}
			if ( index < _size / 2 )
			{
				ChainNode<_type> *p = headerNode;
				for ( int i = 0; i < index; i++ )
				{
					p = p->next;
				}
				p->next = new ChainNode<_type> ( elem , p->next, p );
				ChainNode<_type> *q = p->next;
				p = p->next->next;
				p->prev = q;
				p = nullptr;
				q = nullptr;
			}
			else
			{
				ChainNode<_type> *p = tailNode;
				for ( int i = 0; i < _size - index; i++ )
				{
					p = p->prev;
				}
				p->prev = new ChainNode<_type> ( elem, p, p->prev );
				ChainNode<_type> *q = p->prev;
				p = p->prev->prev;
				p->next = q;
				p = nullptr;
				q = nullptr;
			}
			++ _size;
		}
		/**
		*   void add(ChainIterator<T>* iter)
		*   adds the element in the position of the current iterator
		*   (The position is in the next element)
		*/
		void add ( ChainIterator<_type > &iter, _type elem )
		{
			ChainNode<_type > *p = iter.nextNode->prev;
			p->next = new ChainNode<_type > ( elem , p->next, p );
			ChainNode<_type > *q = p->next;
			p = p->next->next;
			p->prev = q;
			++ _size;
		}
		/**
		*   void add(ChainIterator<T>* iter)
		*   adds the element in the position of the current iterator
		*/
		void add ( MyIterator<T> &iter, _type elem )
		{
			ChainNode<_type > *p = iter.node->prev;
			p->next = new ChainNode<_type > ( elem , p->next, p );
			ChainNode<_type > *q = p->next;
			p = p->next->next;
			p->prev = q;
			++ _size;
		}
		void add ( MyReverseIterator<_type > &iter, _type elem )
		{
			ChainNode<_type > *p = iter.node->next;
			p->prev = new ChainNode<_type > ( elem, p, p->prev );
			ChainNode<_type > *q = p->prev;
			p = p->prev->prev;
			p->next = q;
			++ _size;
		}
		/**
		*   removes all elements from the list
		*   Note: this function does NOT release the memory
		*/
		void clear() override
		{
			while ( _size > 0 )
			{
				pop_front();
			}
			//_size = 0;
			//std::cout << "ChainedList Cleared \n";
		}
		/**
		*   void pop_back()
		*   removes last element on the list and returns it
		*/
		_type pop_back() override
		{
			return remove ( _size - 1 );
		}
		/**
		*   void pop_front()
		*   removes first element on the list and returns it
		*/
		_type pop_front() override
		{
			return remove ( 0 );
		}
		/**
		*   void push_back(T elem)
		*   adds given element to the end of the list
		*/
		void push_back ( _type elem ) override
		{
			add ( _size, elem );
		}
		/**
		*   void push_front(T elem)
		*   adds given element to the beginning of the list
		*/
		void push_front ( _type elem ) override
		{
			add ( 0, elem );
		}

		/**
		*   void sort()
		*   Orders the list using the "natural" comparison of the objects contained by the list
		*   the elements must inherit the class Comparable from JComparator namespace and implement
		*   compareTo method
		*/
		void sort()
		{
			JComparator::ChainSort<_type >::objectSort ( headerNode, 0, this->_size );
		}

		/**
		*   void sort(JComparator::Comparator<T>)
		*   Orders the list using a user defined comparator
		*/
		void sort ( JComparator::Comparator<_type > *c )
		{
			JComparator::ChainSort<_type >::objectSort ( headerNode, 0, this->_size, c );
		}

		/**
		*   void pSort()
		*   Orders the list in ascending order, this must be used instead of sort()
		*   to order primitive type of elements
		*/
		void psort()
		{
			JComparator::ChainSort<_type >::primitiveSort ( headerNode, 0, this->_size );
		}

		virtual void print()
		{
			JIterator::Iterator<_type > *it;
			for ( it = iterator(); it->hasNext(); )
			{
				_type t = it->next();
				if ( std::is_pointer<_type >::value )
				{
					std::cout << "*";
				}
				std::cout << "[" << t << "]  ";
			}
			if ( _size <= 0 )
			{
				std::cout << "The list is empty";
			}
			std::cout << "\n";
			delete it;
		}

		void shuffle()
		{
			for ( int i = 0; i < this->_size; i++ )
			{
				int j = randInt ( i, this->_size );
				swap ( j, i );
			}
		}
		/**
		*   void save(char *)
		*   saves the list items to a given file, if the file does not exist, it gets created
		*/
		void save ( std::string path ) override
		{
			sv = &this->_size;
			//Saves the current size of the list to binary File
			std::ofstream fsalida ( path,
									std::ios::out | std::ios::binary );
			fsalida.write ( reinterpret_cast<char *> ( sv ),
							sizeof ( int ) );
			//Saves the elements in the list to binary File
			JIterator::Iterator<_type > *iter;
			//if L is a Pointer
			for ( iter = this->iterator(); iter->hasNext(); )
			{
				fsalida.write ( reinterpret_cast<char *> ( iter->next() ),
								sizeof ( T ) );
			}
			fsalida.close();
			std::cout << "Save done :) " << std::endl;
			delete iter;
		}

		/**
		*   void load(char *)
		*   loads all elements stored into a file and adds them to the list
		*   if the file does not exist or if it is empty, a runtime_error exception is thrown
		*/
		void load ( std::string path ) override
		{
			lod = new int;
			std::ifstream fentrada ( path,
									 std::ios::in | std::ios::binary | std::ios::ate );
			if ( !fentrada  || fentrada.tellg() <= 0 )
			{
				throw std::runtime_error ( "Unable to find file or file is empty" );
			}
			fentrada.close();
			fentrada.open ( path, std::ios::in | std::ios::binary );
			fentrada.read ( reinterpret_cast<char *> ( lod ),
							sizeof ( int ) );
			//this->clear();
			for ( int i = 0; i < *lod; i++ )
			{
				_type lo = new T();
				fentrada.read ( reinterpret_cast<char *> ( ( lo ) ),
								sizeof ( T ) );
				this->push_back ( lo );
				lo = nullptr;
			}
			fentrada.close();
			std::cout << "Load done <:-{D" << std::endl;
		}

		~DoubleChainedList()
		{
			clear();
			if ( headerNode )
			{
				delete headerNode;
			}
			if ( tailNode )
			{
				delete tailNode;
			}
			this->sv = nullptr;
			this->lod = nullptr;
			this->ele = nullptr;
			this->elo = nullptr;
			std::cout << "ChainedList Cleared \n";
		}


		/**
		*   Custom iterator class created for traverse the list
		*   Note: this class is not like a c++ standard iterator
		*   works with the three custom macros created for traverse the list
		*/
		template <typename I> class ChainIterator: public JIterator::Iterator<I>
		{
			friend class DoubleChainedList;
		private:
			DoubleChainedList<I> *list;
			ChainNode<I> *nextNode;
			bool isRev;

			//ChainNode<I>* currentNode;
		public:
			ChainIterator ( DoubleChainedList<I> *_list, bool _isRev = false ) :
				list ( _list ), nextNode ( !_isRev ? _list->headerNode->next : _list->tailNode->prev ), isRev ( _isRev )
			{
			}
			bool hasNext() override
			{
				return nextNode != list->tailNode;
			}
			bool hasPrev()
			{
				return nextNode != list->headerNode;
			}
			I &next() override
			{
				if ( nextNode == list->tailNode )
				{
					throw except::No_such_element ( "Element does not exist" );
				}
				else
				{
					//std::cout<<nextNode->element<<std::endl;
					I &elementToReturn = nextNode->element;
					nextNode = nextNode->next;
					return elementToReturn;
				}
			}
			I prev()
			{
				if ( nextNode == list->headerNode )
				{
					throw except::No_such_element ( "Element does not exist" );
				}
				else
				{
					//std::cout<<nextNode->element<<std::endl;
					I elementToReturn = nextNode->element;
					nextNode = nextNode->prev;
					return elementToReturn;
				}
			}

			I &current() override
			{
				throw except::Unsuported_operation ( "Operation not supported" );
			}
			// Removes the element and release its memory, so be careful
			void remove() override
			{
				//throw except::Unsuported_operation ("Operation not yet supported");
				if ( !isRev )
				{
					ChainNode<I> *p = nextNode->prev;
					if ( p == list->headerNode )
					{
						except::No_such_element ( "Trying to delete an element that doesn't exist" );
					}
					ChainNode<I> *q = nextNode->prev->prev;
					q->next = nextNode;
					nextNode->prev = q;
					list->_size -= 1;
					delete p->element;
					delete p;
				}
				else
				{
					ChainNode<I> *p = nextNode->next;
					if ( p == list->tailNode )
					{
						except::No_such_element ( "Trying to delete an element that doesn't exist" );
					}
					ChainNode<I> *q = nextNode->next->next;
					q->prev = nextNode;
					nextNode->next = q;
					list->_size -= 1;
					delete p->element;
					delete p;
				}
			}
			~ChainIterator()
			{
				//this->nextNode = nullptr;
			}
		};
		/**
		*   ChainIterator<T>* iterator()
		*   creates an ChainIterator<T>
		*/
		JIterator::Iterator<_type > *iterator() override
		{
			JIterator::Iterator<_type > *it = new ChainIterator<_type > ( this );
			return it;
		}
		/**
		*   Creates a custom made iterator for traversal
		*/
		ChainIterator<_type > _iterator ()
		{
			return ChainIterator<_type > ( this );
		}
		/**
		*   Creates a custom made iterator for reverse traversal
		*/
		ChainIterator<_type > reverse_iterator()
		{
			ChainIterator<_type > it ( this, true );
			return it;
		}
		/**
		*   Custom iterator class created for traverse the list
		*   this class is like a c++ standard iterator
		*   it can be used with for_each and with a range based loop
		*/
		template<typename C>class MyIterator
		{
			friend class DoubleChainedList;
		protected:
			const DoubleChainedList<C> *list;
			ChainNode<C> *node;
		public:
			MyIterator() : list( nullptr ), node( nullptr ) {}
			MyIterator ( const DoubleChainedList<C> *_list, bool is_end = false ) :
				list ( _list ), node ( !is_end ? _list->headerNode->next : _list->tailNode )
			{}
			MyIterator ( const MyIterator &other ) : MyIterator ( other.list )
			{
				node = other.node;
			}
			bool operator == ( const MyIterator &other )
			{
				return node == other.node;
			}
			bool operator != ( const MyIterator &other )
			{
				return node != other.node;
			}
			const MyIterator &operator++()
			{
				node = node->next;
				return *this;
			}
			const MyIterator &operator--()
			{
				node = node->prev;
				return *this;
			}
			C &operator*() const
			{
				return node->element;
			}
		};
		template<typename C>class MyReverseIterator : public MyIterator<C>
		{
			friend class DoubleChainedList;
		protected:

		public:
			MyReverseIterator() : MyIterator<C>() {}
			MyReverseIterator ( const DoubleChainedList<C> *_list, bool is_end = false ) :
				MyIterator<C> ( _list, is_end )
			{
				this->node = ( is_end ? _list->headerNode : _list->tailNode->prev );
			}
			MyReverseIterator ( const MyIterator<C> &other ) : MyReverseIterator ( other.list )
			{
				this->node = other.node;
			}
			bool operator == ( const MyIterator<C> &other )
			{
				return this->node == other.node;
			}
			bool operator != ( const MyIterator<C> &other )
			{
				return this->node != other.node;
			}
			const MyIterator<C> &operator++()
			{
				this->node = this->node->prev;
				return *this;
			}
			const MyIterator<C> &operator--()
			{
				this->node = this->node->next;
				return *this;
			}
			_type &operator*() const
			{
				return this->node->element;
			}
		};
		/**
		*   Returns an iterator pointing to the beginning of the list
		*/
		MyIterator<_type > begin () const
		{
			return MyIterator<_type > ( this );
		}
		/**
		*   Returns an iterator pointing to the end of the list
		*/
		MyIterator<_type > end () const
		{
			return MyIterator<_type > ( this, true );
		}
		/**
		*   Returns an iterator pointing to the end of the list
		*/
		MyReverseIterator<_type > rbegin () const
		{
			return MyReverseIterator<_type > ( this );
		}
		/**
		*   Returns an iterator pointing to the beginning of the list
		*/
		MyReverseIterator<_type > rend () const
		{
			return MyReverseIterator<_type > ( this, true );
		}
		/**
		*   Overload of the << operator to print all the elements on the list
		*/
		friend std::ostream &operator<< ( std::ostream &out, DoubleChainedList<_type > *list )
		{
			list->osPrint ( out );
			return out;
		}
		friend std::ostream &operator<< ( std::ostream &out, DoubleChainedList<_type > &list )
		{
			list.osPrint ( out );
			return out;
		}

	protected:
		/**
		*   void osPrint(std::ostream& out)
		*   prints the list to an ostream object
		*/
		virtual void osPrint ( std::ostream &out )
		{
			_type elem = ( _type  ) nullptr;
			_for ( elem, ( *this ) )
			{
				out << "*";
				out << "[" << ( elem ) << "]  ";
			}
			if ( _size <= 0 )
			{
				out << "The list is empty";
			}
			out << "\n";
		}

	private:
		int *sv;
		int *lod;
		_type ele;
		_type elo;
		int _size;
		ChainNode<_type > *headerNode;
		ChainNode<_type > *tailNode;
		void checkIndex ( int index )
		{
			if ( index < 0 || index >= _size )
			{
				//throw std::out_of_range("IndexOutOfBounds");
				throw except::Index_out_of_range ( "IndexOutOfBounds" );
			}
		}
		/**
		*   interchanges two elements from the list with the given
		*   indexes
		*/
		void swap ( int index, int index2 )
		{
			checkIndex ( index );
			checkIndex ( index2 );
			_type elem;
			ChainNode<_type > *p = headerNode;
			ChainNode<_type > *q = headerNode;
			for ( int i = 0; i < index ; i++ )
			{
				p = p->next;
			}
			for ( int i = 0; i < index2 ; i++ )
			{
				q = q->next;
			}
			elem = p->next->element;
			p->next->element = q->next->element;
			q->next->element = elem;
			p = nullptr;
			q = nullptr;
		}

		int randInt ( int min, int max )
		{
			int randomNum = min + rand() % ( max - min );
			return randomNum;
		}
	};
}

// Macro use for reverse iteration
#ifndef r_for

#define r_for(elem, list) \
	for(auto _ITER(list->reverse_iterator());\
			_ITER.hasPrev() && ((elem = _ITER.prev()) || true);)\

#endif //r_for

#endif // CIRCLECHAINEDLIST_H
