// *** ADDED BY HEADER FIXUP ***
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
// *** END ***
#ifndef CIRCLEHEADERCHAINDEDLIST_H
#define CIRCLEHEADERCHAINDEDLIST_H

#include "LinearList.hpp"
#include "ChainNode.h"
namespace JList
{
	/**
	*   CircleHeaderChaindedList
	*   Container list class which uses a circular linked list with header to store elements
	*   This class has some util functions, like sort, shuffle, save and load
	*   save and load does NOT work with elements which contains virtual functions
	*/
	template <typename T> class CircleHeaderChaindedList : public LinearList<T>, public LinearListImproved<T>, public JIterator::Iterable<T>
	{
	public:
	    template<typename C> class MyIterator;

		/**
		*   Constructor, takes no arguments
		*/
		CircleHeaderChaindedList()
		{
			headerNode = new ChainNode<T>();
			headerNode->next = headerNode;
			headerNode->element = T();
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
		CircleHeaderChaindedList ( const CircleHeaderChaindedList<T> &_list ) : CircleHeaderChaindedList()
		{
			for ( auto i : _list )
			{
				this->push_back ( i );
			}
		}
		/**
		*   bool isEmpty()
		*   returns wheter or not the list is empty
		*/
		bool isEmpty()
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
		T &get ( int index )
		{
			checkIndex ( index );
			ChainNode<T> *currentNode = headerNode->next;
			for ( int i = 0; i < index; i++ )
			{
				currentNode = currentNode->next;
			}
			return currentNode->element;
		}
		/**
		*   Copy assignment operator
		*/
		CircleHeaderChaindedList &operator= ( const CircleHeaderChaindedList<T> &_list )
		{
			this->clear();
			for ( auto i : _list )
			{
				this->push_back ( i );
			}
		}
		/**
		*   int indexOf(T elem)
		*   returns index of given element
		*/
		int indexOf ( const T &theElement )
		{
			headerNode->element = theElement;
			int index = 0;
			ChainNode<T> *currentNode = headerNode->next;
			while ( currentNode->element != theElement )
			{
				currentNode = currentNode->next;
				index ++;
			}
			if ( currentNode == headerNode )
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

		T remove ( int index )
		{
			checkIndex ( index );
			ChainNode<T> *p = headerNode;
			for ( int i = 0; i < index; i++ )
			{
				p = p->next;
			}
			T removed = p->next->element;
			ChainNode<T> *q = p->next;
			//std::cout << "the element removed is" << q->element << "  " << removed << std::endl;
			p->next = p->next->next;
			delete q;
			q = nullptr;
			p = nullptr;
			_size --;
			return removed;
		}

		/**
		*   removes the element pointed by the iterator
		*/

		T remove( MyIterator<T> &iter )
		{
			ChainNode<T> *in = iter.node;
			T removed = ( *iter );
			ChainNode<T> *p = headerNode;
			ChainNode<T> *q = headerNode->next;
			while ( q != in )
			{
				p = p->next;
				q = q->next;
			}
			MyIterator<T> it( this, p );
			iter = it;
			p->next = q->next;
			_size--;
			delete q;
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
		*   void add(int index,T elem)
		*   adds given element to the list in given index
		*/
		void add ( int index, const T &elem )
		{
			if ( index < 0 || index > _size )
			{
				throw except::Index_out_of_range ( "IndexOutOfBounds" );
			}
			ChainNode<T> *p = headerNode;
			for ( int i = 0; i < index; i++ )
			{
				p = p->next;
			}
			p->next = new ChainNode<T> ( elem , p->next );
			_size ++;
		}

		void clear()
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
		T pop_back()
		{
			return remove ( _size - 1 );
		}
		/**
		*   void pop_front()
		*   removes first element on the list and returns it
		*/
		T pop_front()
		{
			return remove ( 0 );
		}
		/**
		*   void push_back(T elem)
		*   adds given element to the end of the list
		*/
		void push_back ( const T &elem )
		{
			add ( _size, elem );
		}
		/**
		*   void push_front(T elem)
		*   adds given element to the beginning of the list
		*/
		void push_front ( const T &elem )
		{
			add ( 0, elem );
		}

		/**
		*   void sort()
		*   Orders the list using the "natural" comparation of the objects contained by the list
		*   the elements must inherit the class Comparable from JComparator namespace and implements
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
		/**
		*   heap sort. this is the fastest implementation of all
		*/
		void hSort()
		{
			JComparator::ArraySort<T>::heap_sort ( *this, _size );
		}
		template<typename C> void hSort()
		{
			JComparator::ArraySort<T>:: template _heap_sort<C> ( *this, _size );
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
			///Saves the current size of the list to binary File
			std::ofstream fsalida ( path,
			                        std::ios::out | std::ios::binary );
			fsalida.write ( reinterpret_cast<char *> ( sv ),
			                sizeof ( int ) );
			///Saves the elements in the list to binary File
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
			///if Q is not a pointer
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

		~CircleHeaderChaindedList()
		{
			clear();
			if ( headerNode )
			{
				delete headerNode;
			}
			this->sv = nullptr;
			this->lod = nullptr;
			this->ele = nullptr;
			this->elo = nullptr;
			std::cout << "ChainedList Cleared \n";
		}



		template <typename I> class ChainIterator: public JIterator::Iterator<I>
		{
		private:
			CircleHeaderChaindedList<I> *list;
			ChainNode<I> *nextNode;
			ChainNode<I> *currentNode;
		public:
			ChainIterator ( CircleHeaderChaindedList<I> *_list )
			{
				this->list = _list;
				nextNode = list->headerNode->next;
				currentNode = nextNode;
			}
			bool hasNext()
			{
				return nextNode != list->headerNode;
			}

			I &next()
			{
				if ( nextNode == list->headerNode )
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

			I &current()
			{
				currentNode = nextNode;
				if ( currentNode != list->headerNode )
				{
					return currentNode->element;
				}
				throw except::No_such_element ( "Element does not exist" );
			}

			void remove()
			{
				//throw except::Unsuported_operation ("Operation not yet supported");
				ChainNode<I> *p = nextNode->prev;
				ChainNode<I> *q = nextNode->prev->prev;
				q->next = nextNode;
				nextNode->prev = q;
				list->_size -= 1;
				delete p;
			}
			~ChainIterator()
			{
				this->nextNode = nullptr;
				this->currentNode = nullptr;
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
		ChainIterator<T> _iterator ()
		{
			ChainIterator<T> it ( this );
			return it;
		}
		template<typename C>class MyIterator
		{
			friend class CircleHeaderChaindedList < C >;
			const CircleHeaderChaindedList<C> *list;
			ChainNode<C> *node;

		public:
			MyIterator() : list( nullptr ), node( nullptr ) {}
			MyIterator( const CircleHeaderChaindedList<C> *_list, ChainNode<C> *_node ) :
				list( _list ), node( _node ) {}
			MyIterator ( const CircleHeaderChaindedList<C> *_list, bool is_end = false ) :
				list ( _list ), node ( !is_end ? _list->headerNode->next : _list->headerNode )
			{}
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
			const MyIterator operator++( int )
			{
				MyIterator tmp( *this );
				operator++();
				return tmp;
			}
			T &operator*() const
			{
				return node->element;
			}
		};
		/**
		*   Returns an iterator pointing to the beginning of the list
		*/
		MyIterator<T> begin () const
		{
			return MyIterator<T> ( *this );
		}
		/**
		*   Returns an iterator pointing to the end of the list
		*/
		MyIterator<T> end () const
		{
			return MyIterator<T> ( *this, true );
		}
		/**
		*   Overload of the << operator to print all the elements on the list
		*/
		friend std::ostream &operator<< ( std::ostream &out, CircleHeaderChaindedList<T> *list )
		{
			list->osPrint ( out );
			return out;
		}
		friend std::ostream &operator<< ( std::ostream &out, CircleHeaderChaindedList<T> &list )
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
			for ( auto elem : *this )
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
	*   CircleHeaderChaindedList
	*   Container list class which uses a circular linked list with header to store elements
	*   This class has some util functions, like sort, shuffle, save and load
	*   save and load does NOT work with elements which contains virtual functions
	*/
	template <typename T> class CircleHeaderChaindedList<T *> : public LinearList<T *>, public LinearListImproved<T *>, public JIterator::Iterable<T *>
	{
	public:
	    template<typename C> class MyIterator;

		/**
		*   Constructor, takes no arguments
		*/
		CircleHeaderChaindedList()
		{
			headerNode = new ChainNode<T *>();
			headerNode->next = headerNode;
			headerNode->element = ( T * ) nullptr;
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
		CircleHeaderChaindedList ( const CircleHeaderChaindedList<T *> &_list ) : CircleHeaderChaindedList()
		{
			for ( auto i : _list )
			{
				this->push_back ( i );
			}
		}
		/**
		*   bool isEmpty()
		*   returns wheter or not the list is empty
		*/
		bool isEmpty()
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
		T *get ( int index )
		{
			checkIndex ( index );
			ChainNode<T *> *currentNode = headerNode->next;
			for ( int i = 0; i < index; i++ )
			{
				currentNode = currentNode->next;
			}
			return currentNode->element;
		}
		/**
		*   Copy assignment operator
		*/
		CircleHeaderChaindedList &operator= ( const CircleHeaderChaindedList<T *> &_list )
		{
			this->clear();
			for ( auto i : _list )
			{
				this->push_back ( i );
			}
		}
		/**
		*   int indexOf(T elem)
		*   returns index of given element
		*/
		int indexOf ( T *theElement )
		{
			headerNode->element = theElement;
			int index = 0;
			ChainNode<T *> *currentNode = headerNode->next;
			while ( currentNode->element != theElement )
			{
				currentNode = currentNode->next;
				index ++;
			}
			if ( currentNode == headerNode )
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

		T *remove ( int index )
		{
			checkIndex ( index );
			T *removed = ( T * ) nullptr;
			ChainNode<T *> *p = headerNode;
			for ( int i = 0; i < index; i++ )
			{
				p = p->next;
			}
			removed = p->next->element;
			ChainNode<T *> *q = p->next;
			//std::cout << "the element removed is" << q->element << "  " << removed << std::endl;
			p->next = p->next->next;
			delete q;
			q = nullptr;
			p = nullptr;
			_size --;
			return removed;
		}
		/**
		*   removes the element pointed by the iterator
		*/

		T *remove( MyIterator<T *> &iter )
		{
			ChainNode<T *> *in = iter.node;
			T *removed = ( *iter );
			ChainNode<T *> *p = headerNode;
			ChainNode<T *> *q = headerNode->next;
			while ( q != in )
			{
				p = p->next;
				q = q->next;
			}
			MyIterator<T *> it( this, p );
			iter = it;
			p->next = q->next;
			delete q;
			_size--;
			return removed;
		}

		/**
		*   T* ObjectRemove(T elem)
		*   search the list for the given element and removes it
		*/
		T *ObjectRemove ( T *elem )
		{
			T *removed = ( T * ) nullptr;
			int index = indexOf ( elem );
			if ( index >= 0 )
			{
				removed = remove ( index );
				return removed;
			}
			std::cout << "The element Does not Exist\n";
			return removed;
		}
		/**
		*   void add(int index,T elem)
		*   adds given element to the list in given index
		*/
		void add ( int index, T *elem )
		{
			if ( index < 0 || index > _size )
			{
				throw except::Index_out_of_range ( "IndexOutOfBounds" );
			}
			ChainNode<T *> *p = headerNode;
			for ( int i = 0; i < index; i++ )
			{
				p = p->next;
			}
			p->next = new ChainNode<T *> ( elem , p->next );
			_size ++;
		}

		void clear()
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
		T *pop_back()
		{
			return remove ( _size - 1 );
		}
		/**
		*   void pop_front()
		*   removes first element on the list and returns it
		*/
		T *pop_front()
		{
			return remove ( 0 );
		}
		/**
		*   void push_back(T elem)
		*   adds given element to the end of the list
		*/
		void push_back ( T *elem )
		{
			add ( _size, elem );
		}
		/**
		*   void push_front(T elem)
		*   adds given element to the beginning of the list
		*/
		void push_front ( T *elem )
		{
			add ( 0, elem );
		}

		/**
		*   void sort()
		*   Orders the list using the "natural" comparation of the objects contained by the list
		*   the elements must inherit the class Comparable from JComparator namespace and implements
		*   compareTo method
		*/
		void sort()
		{
			JComparator::ChainSort<T *>::objectSort ( headerNode, 0, this->_size );
		}

		/**
		*   void sort(JComparator::Comparator<T>)
		*   Orders the list using a user defined comparator
		*/
		void sort ( JComparator::Comparator<T *> *c )
		{
			JComparator::ChainSort<T *>::objectSort ( headerNode, 0, this->_size, c );
		}

		/**
		*   void pSort()
		*   Orders the list in ascending order, this must be used instead of sort()
		*   to order primitive type of elements
		*/
		void psort()
		{
			JComparator::ChainSort<T *>::primitiveSort ( headerNode, 0, this->_size );
		}

		void hSort()
		{
			JComparator::ArraySort<T>::heap_sort ( *this, _size );
		}
		template<typename C> void hSort()
		{
			JComparator::ArraySort<T>:: template _heap_sort<C> ( *this, _size );
		}

		virtual void print()
		{
			JIterator::Iterator<T *> *it;
			for ( it = iterator(); it->hasNext(); )
			{
				T *t = it->next();
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
			///Saves the current size of the list to binary File
			std::ofstream fsalida ( path,
			                        std::ios::out | std::ios::binary );
			fsalida.write ( reinterpret_cast<char *> ( sv ),
			                sizeof ( int ) );
			///Saves the elements in the list to binary File
			JIterator::Iterator<T *> *iter;
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
		    std::string msg = "path: " + path + " Operation not yet supported";
			throw except::Unsuported_operation( msg.c_str() );
			/*lod = new int;
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
			for ( int i = 0; i < *lod; i++ )
			{
				T *lo = new T();
				fentrada.read ( reinterpret_cast<char *> ( ( lo ) ),
				                sizeof ( T ) );
				this->push_back ( ( T * ) lo );
				lo = nullptr;
			}
			fentrada.close();
			std::cout << "Load done <:-{D" << std::endl;*/
		}

		~CircleHeaderChaindedList()
		{
			clear();
			if ( headerNode )
			{
				delete headerNode;
			}
			this->sv = nullptr;
			this->lod = nullptr;
			this->ele = nullptr;
			this->elo = nullptr;
			std::cout << "ChainedList Cleared \n";
		}



		template <typename I> class ChainIterator: public JIterator::Iterator<I>
		{
		private:
			CircleHeaderChaindedList<I> *list;
			ChainNode<I> *nextNode;
			ChainNode<I> *currentNode;
		public:
			ChainIterator ( CircleHeaderChaindedList<I> *_list )
			{
				this->list = _list;
				nextNode = list->headerNode->next;
				currentNode = nextNode;
			}
			bool hasNext()
			{
				return nextNode != list->headerNode;
			}

			I &next()
			{
				if ( nextNode == list->headerNode )
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

			I &current()
			{
				currentNode = nextNode;
				if ( currentNode != list->headerNode )
				{
					return currentNode->element;
				}
				throw except::No_such_element ( "Element does not exist" );
			}

			void remove()
			{
				//throw except::Unsuported_operation ("Operation not yet supported");
				ChainNode<I> *p = nextNode->prev;
				ChainNode<I> *q = nextNode->prev->prev;
				q->next = nextNode;
				nextNode->prev = q;
				list->_size -= 1;
				delete p;
			}
			~ChainIterator()
			{
				this->nextNode = nullptr;
				this->currentNode = nullptr;
			}
		};
		/**
		*   ChainIterator<T>* iterator()
		*   creates an ChainIterator<T>
		*/
		JIterator::Iterator<T *> *iterator() override
		{
			JIterator::Iterator<T *> *it = new ChainIterator<T *> ( this );
			return it;
		}
		ChainIterator<T *> _iterator ()
		{
			ChainIterator<T *> it ( this );
			return it;
		}
		template<typename C>class MyIterator
		{
			friend class CircleHeaderChaindedList < C > ;
			const CircleHeaderChaindedList<C> *list;
			ChainNode<C> *node;

		public:
			MyIterator() : list( nullptr ), node( nullptr ) {}
			MyIterator( const CircleHeaderChaindedList<C> *_list, ChainNode<C> *_node ) :
				list( _list ), node( _node ) {}
			MyIterator ( const CircleHeaderChaindedList<C> *_list, bool is_end = false ) :
				list ( _list ), node ( !is_end ? _list->headerNode->next : _list->headerNode )
			{}
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
			const MyIterator operator++( int )
			{
				MyIterator tmp( *this );
				operator++();
				return tmp;
			}
			T *&operator*() const
			{
				return node->element;
			}
		};
		/**
		*   Returns an iterator pointing to the beginning of the list
		*/
		MyIterator<T *> begin () const
		{
			return MyIterator<T *> ( this );
		}
		/**
		*   Returns an iterator pointing to the end of the list
		*/
		MyIterator<T *> end () const
		{
			return MyIterator<T *> ( this, true );
		}
		/**
		*   Overload of the << operator to print all the elements on the list
		*/
		friend std::ostream &operator<< ( std::ostream &out, CircleHeaderChaindedList<T *> *list )
		{
			list->osPrint ( out );
			return out;
		}
		friend std::ostream &operator<< ( std::ostream &out, CircleHeaderChaindedList<T *> &list )
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
			T *elem = ( T * ) nullptr;
			_for ( elem, ( *this ) )
			{
				if ( std::is_pointer<T *>::value )
				{
					out << "*";
				}
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
		ChainNode<T *> *headerNode;
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
			T *elem;
			ChainNode<T *> *p = headerNode;
			ChainNode<T *> *q = headerNode;
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

#endif // CIRCLEHEADERCHAINDEDLIST_H
