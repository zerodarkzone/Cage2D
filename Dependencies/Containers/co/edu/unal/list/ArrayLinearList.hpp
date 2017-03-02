// *** ADDED BY HEADER FIXUP ***
#include <iostream>
// *** END ***
#ifndef ARRAYLINEARLIST_HPP_INCLUDED
#define ARRAYLINEARLIST_HPP_INCLUDED
#include "LinearList.hpp"
#include "jcomparator\JComparator.hpp"

namespace JList
{

	/**
	*   class ArrayLinearlist
	*   List that use a dynamic vector to hold the elements
	*   it has a Java like custom iterator, and also
	*   a c++ standard iterator
	*/

	template <typename T>class ArrayLinearList: public LinearList<T>, public JIterator::Iterable<T>
	{
	public:
		typedef T _type;
		template<typename C> class MyIterator;
		template<typename C> class MyReverseIterator;
	private:
		inline void checkIndex ( int index ) const
		{
			if ( index < 0 || index >= _size )
			{
				throw except::Index_out_of_range ( "IndexOutOfBounds" );
			}
		}
	protected:
		T *element;
		int _size;
		int length;


		/**
		*   void osPrint(std::ostream& out)
		*   prints the list to an ostream object
		*/
		virtual void osPrint ( std::ostream &out )
		{
			for ( auto elem : ( *this ) )
			{
				out << "[" << elem << "]  ";
			}
			if ( _size <= 0 )
			{
				out << "The list is empty";
			}
		}

	public:

		/**
		*   ArrayLinearList(int initialCapacity)
		*   creates a T type array with size = initialCapacity to store the elements in the list
		*/
		ArrayLinearList ( int initialCapacity )
		{
			if ( initialCapacity <= 0 )
			{
				initialCapacity = 1;
			}
			element = new T[initialCapacity];
			_size = 0;
			length = initialCapacity;
		}
		ArrayLinearList ( const ArrayLinearList<T> &_list ) : ArrayLinearList ( _list.getSize() )
		{
			for ( auto i : _list )
			{
				this->push_back ( i );
			}
		}
		/**
		*   ArrayLinearList (std::initializer_list<T> il)
		*   creates a T type array with size = il.size to store the elements in the list
		*   and copy the elements of the initializer list to the array
		*/
		ArrayLinearList ( std::initializer_list<T> il )
		{
			element = new T[ ( int ) il.size()];
			_size = 0;
			length = il.size();
			for ( auto i : il )
			{
				this->push_back ( i );
			}
		}
		/**
		*   ArrayLinearList()
		*   creates a T type array with size = 20 to store the elements in the list
		*/
		ArrayLinearList() : ArrayLinearList ( 20 ) {}
		/**
		*   Copy assignment operator
		*/
		ArrayLinearList &operator= ( const ArrayLinearList &_list )
		{
			this->clear();
			for ( auto i : _list )
			{
				this->push_back ( i );
			}
			return *this;
		}
		/**
		*   Initializer list assignment
		*/
		ArrayLinearList &operator= ( std::initializer_list<T> il )
		{
			this->clear();
			for ( auto i : il )
			{
				this->push_back ( i );
			}
			return *this;
		}
		/**
		*   Operator ==
		*/
		inline bool operator== ( const ArrayLinearList &lt ) const
		{
			if ( this->_size != lt._size )
			{
				return false;
			}
			for ( int i = 0; i < _size; i++ )
			{
				if ( ( *this )[i] != lt[i] )
				{
					return false;
				}
			}
			return true;
		}
		/**
		*   Changes the maximum size of the list
		*/
		void operator() ( int _cap )
		{
			if ( _cap > length )
			{
				T *old = element;
				element = new T[_cap];
				std::copy( old, old + _size, element );
				delete[] old;
				length = _cap;
			}
		}
		/**
		*   Allocates space for a certain number of objects, increases the size of the list.
		*/
		void allocate_size ( int sis )
		{
			if ( ( _size + sis ) > length )
			{
				throw except::Index_out_of_range ( "The size is larger than the length, please increase the size of the list" );
			}
			_size += sis;
		}
		/**
		*   bool isEmpty()
		*   returns wheter or not the list is empty
		*/
		bool isEmpty() override
		{
			return _size == 0;
		}
		/**
		*   int getSize()
		*   returns current size of the list
		*/
		int getSize() override
		{
			return this->_size;
		}
		int getSize() const
		{
			return this->_size;
		}
		/**
		*   T get(int index)
		*   returns list element on given index
		*   Checks if the index is inside of bounds
		*/
		T &get ( int index ) override
		{
			checkIndex ( index );
			return element[index];
		}
		const T &get( int index ) const
		{
			checkIndex( index );
			return element[index];
		}
		/**
		*   T operator [] (int index)
		*   returns list element on given index
		*   Does not check if the index is inside of bounds
		*/
		T &operator[] ( int index )
		{
			checkIndex ( index );
			return element[index];
		}
		/**
		*   T operator [] (int index)
		*   returns list element on given index
		*   Does not check if the index is inside of bounds
		*/
		const T &operator[] ( int index ) const
		{
			checkIndex ( index );
			return element[index];
		}
		/**
		*   int indexOf(T elem)
		*   returns index of given element
		*/
		int indexOf ( const T &elem ) override
		{
			for ( int i = 0; i < _size; ++i )
				if ( elem == element[i] )
				{
					return i;
				}
			return -1;
		}
		/**
		*   T remove(int index)
		*   remove list element on given index and returns such element
		*/
		T remove ( int index ) override
		{
			checkIndex ( index );
			T removed = element[index];
			/*for ( int i = index; i < _size - 1; i++ )
			{
				element[ i ] = element[ i + 1 ];
			}*/
			std::copy( element + index + 1, element + _size,  element + index );
			//element[ --_size ] = (const T) nullptr;
			--_size;
			return removed;
		}
		/**
		*   T remove(const MyIterator<T> &iter)
		*   remove list element on given index and returns such element
		*/
		T remove( MyIterator<T> &iter )
		{
			return remove( iter._index );
		}
		/**
		*   T remove(const MyReverseIterator<T> &iter)
		*   remove list element on given index and returns such element
		*/
		T remove( MyReverseIterator<T> &iter )
		{
			return remove( iter._index );
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
		void add ( int index, const T &elem ) override
		{
			if ( index < 0 || index > _size )
			{
				throw except::Index_out_of_range ( "IndexOutOfBounds" );
			}
			if ( _size == length )
			{
				T *old = element;
				element = new T[2 * _size];
				length *= 2;
				for ( int i = 0; i < _size; i++ )
				{
					element[i] = old[i];
				}
				delete[] old;
				//std::cout << "duplicate \n";
			}
			for ( int i = _size - 1; i >= index; i-- )
			{
				element[i + 1] = element[i];
			}
			element[index] = elem;
			_size++;
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
		*   void sort()
		*   Orders the list using the "natural" comparation of the objects contained by the list
		*   the elements must inherit the class Comparable from JComparator namespace and implements
		*   compareTo method
		*/
		void sort()
		{
			JComparator::ArraySort<T>::quick_sort ( element, 0, this->_size - 1 );
		}
		void hSort()
		{
			JComparator::ArraySort<T>::heap_sort ( element, _size );
		}
		template<typename C> void hSort()
		{
			JComparator::ArraySort<T>:: template _heap_sort<C> ( element, _size );
		}
		/**
		*   void pSort()
		*   Orders the list in asscending order, this must be used instead of sort()
		*   to order primitive type of elements
		*/
		void pSort()
		{
			JComparator::ArraySort<T>::primitiveArraySort ( element, 0, this->_size - 1 );
		}
		/**
		*   void sort(JComparator::Comparator<T>)
		*   Orders the list using a user defined comparator
		*/
		void sort ( JComparator::Comparator<T> *c )
		{
			JComparator::ArraySort<T>::quick_sort ( element, 0, this->_size - 1, c );
		}
		/**
		*   void clear()
		*   remove all elements stored in the list
		*/
		void clear()
		{
			if ( element )
			{
				delete[] element;
			}
			element = new T[length];
			_size = 0;
			//std::cout << "List Cleared \n";
		}
		/**
		*   void print()
		*   prints the list to console
		*/
		virtual void print()
		{
			JIterator::Iterator<T> *iter;
			for ( iter = this->iterator(); iter->hasNext(); )
			{
				T t = iter->next();
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
			delete iter;
		}

		/**
		*   class ArrayLinearListIterator
		*   Java like iterator to iterate the list
		*/
		template <typename G>class ArrayLinearListIterator: public JIterator::Iterator<G>
		{
		private:
			ArrayLinearList<G> *list;
			int nextIndex;
		public:

			ArrayLinearListIterator ( ArrayLinearList *_list )
			{
				this->list = _list;
				nextIndex = 0;
			}
			bool hasNext()
			{
				return nextIndex < list->_size;
			}

			G &next()
			{
				if ( ! ( nextIndex < list->_size ) )
				{
					throw except::No_such_element ( "Element does not exist" );
				}
				return list->element[ nextIndex++ ];
			}
			G &current()
			{
				if ( nextIndex == list->getSize() )
				{
					throw except::No_such_element ( "Element does not exist" );
				}
				return list->element[ nextIndex ];
			}

			void remove()
			{
				//throw except::Unsuported_operation ("Not yet supported operation");
				int cur = nextIndex - 1;
				list->checkIndex ( cur );
				//T removed = element[index];
				for ( int i = cur; i < list->_size - 1; i++ )
				{
					list->element[ i ] = list->element[ i + 1 ];
				}
				//list->element[ --list->_size ] = (const T) nullptr;
				--list->_size;
			}
		};

		/**
		*   ArrayLinearListIterator<T>* iterator()
		*   creates an ArrayLinearListIterator<T>
		*/
		virtual JIterator::Iterator<T> *iterator() override
		{
			JIterator::Iterator<T> *iter = new ArrayLinearListIterator<T> ( this );
			return iter;
		}
		ArrayLinearListIterator<T> _iterator ()
		{
			return ArrayLinearListIterator<T> ( this );
		}
		template<typename C>class MyIterator
		{
			friend class ArrayLinearList<C>;
		protected:
			const ArrayLinearList<C> *list;
			int _index;

		public:
			MyIterator() : list( nullptr ), _index( 0 ) {}
			MyIterator ( const ArrayLinearList<C> *_list, bool is_end = false ) :
				list ( _list ), _index ( !is_end ? 0 : _list->_size )
			{}
			MyIterator( const MyIterator &other ) : MyIterator( other.list )
			{
				_index = other._index;
			}

			bool operator == ( const MyIterator &other )
			{
				return _index == other._index;
			}
			bool operator != ( const MyIterator &other )
			{
				return _index != other._index;
			}
			const MyIterator &operator++()
			{
				++_index;
				return *this;
			}
			T &operator*() const
			{
				return list->element[_index];
			}
		};
		template<typename C>class MyReverseIterator : public MyIterator<C>
		{
			friend class ArrayLinearList<C>;
		public:
			MyReverseIterator() : MyIterator<C>() {}
			MyReverseIterator ( const ArrayLinearList<C> *_list, bool is_end = false ) :
				MyIterator<C> ( _list, is_end )
			{
				this->_index = is_end ? -1 : _list->_size - 1;
			}
			MyReverseIterator( const MyReverseIterator &other ) : MyReverseIterator( other.list )
			{
				this->_index = other._index;
			}
			bool operator == ( const MyIterator<C> &other )
			{
				return this->_index == other._index;
			}
			bool operator != ( const MyIterator<C> &other )
			{
				return this->_index != other._index;
			}
			const MyIterator<C> &operator++()
			{
				--this->_index;
				return *this;
			}
			T &operator*() const
			{
				return this->list->element[this->_index];
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
		*   Returns an iterator pointing to the beginning of the list
		*/
		MyReverseIterator<T> rbegin () const
		{
			return MyReverseIterator<T> ( this );
		}
		/**
		*   Returns an iterator pointing to the end of the list
		*/
		MyReverseIterator<T> rend () const
		{
			return MyReverseIterator<T> ( this, true );
		}
		/**
		*   Overload of the << operator to print all the elements on the list
		*/
		friend std::ostream &operator<< ( std::ostream &out, ArrayLinearList<T> *list )
		{
			list->osPrint ( out );
			return out;
		}

		friend std::ostream &operator<< ( std::ostream &out, ArrayLinearList<T> &list )
		{
			list.osPrint ( out );
			return out;
		}

		~ArrayLinearList()
		{
			clear();
			if ( element )
			{
				delete[] element;
			}
		}
	};
}

namespace JList
{

	/**
	*   Pointer specialization of the
	*   ArrayLinearList class
	*/
	template <typename T>class ArrayLinearList<T *>: public LinearList<T *>, public JIterator::Iterable<T *>
	{
	public:
		typedef T *_type;
		template<typename C> class MyIterator;
		template<typename C> class MyReverseIterator;

	private:
		inline void checkIndex ( int index ) const
		{
			if ( index < 0 || index >= _size )
			{
				throw except::Index_out_of_range ( "IndexOutOfBounds" );
			}
		}
	protected:
		_type *element;
		int _size;
		int length;


		/**
		*   void osPrint(std::ostream& out)
		*   prints the list to an ostream object
		*/
		virtual void osPrint ( std::ostream &out )
		{
			_type elem = ( _type  ) nullptr;
			_for ( elem, ( *this ) )
			{
				if ( std::is_pointer<_type >::value )
				{
					out << "*";
				}
				out << "[" << elem << "]  ";
			}
			if ( _size <= 0 )
			{
				out << "The list is empty";
			}
		}

	public:

		/**
		*   ArrayLinearList(int initialCapacity)
		*   creates a T type array with size = initialCapacity to store the elements in the list
		*/
		ArrayLinearList ( int initialCapacity )
		{
			if ( initialCapacity <= 0 )
			{
				initialCapacity = 1;
			}
			element = new T*[initialCapacity];
			_size = 0;
			length = initialCapacity;
		}
		ArrayLinearList ( const ArrayLinearList<_type > &_list ) : ArrayLinearList ( _list.getSize() )
		{
			for ( auto i : _list )
			{
				this->push_back ( i );
			}
		}
		/**
		*   ArrayLinearList (std::initializer_list<T> il)
		*   creates a T type array with size = il.size to store the elements in the list
		*   and copy the elements of the initializer list to the array
		*/
		ArrayLinearList ( std::initializer_list<_type > il )
		{
			element = new T*[ ( int ) il.size()];
			_size = 0;
			length = il.size();
			for ( auto i : il )
			{
				this->push_back ( i );
			}
		}
		/**
		*   ArrayLinearList()
		*   creates a T type array with size = 20 to store the elements in the list
		*/
		ArrayLinearList() : ArrayLinearList ( 20 ) {}
		/**
		*   Copy assignment operator
		*/
		ArrayLinearList &operator= ( const ArrayLinearList &_list )
		{
			this->clear();
			for ( auto i : _list )
			{
				this->push_back ( i );
			}
			return *this;
		}
		/**
		*   Initializer list assignment
		*/
		ArrayLinearList &operator= ( std::initializer_list<_type > il )
		{
			this->clear();
			for ( auto i : il )
			{
				this->push_back ( i );
			}
			return *this;
		}
		/**
		*   Operator ==
		*/
		bool operator== ( const ArrayLinearList &lt )
		{
			if ( this->_size != lt._size )
			{
				return false;
			}
			for ( int i = 0; i < _size; i++ )
			{
				if ( ( *this )[i] != lt[i] )
				{
					return false;
				}
			}
			return true;
		}
		/**
		*   Changes the maximum size of the list
		*/
		void operator() ( int _cap )
		{
			if ( _cap > length )
			{
				_type *old = element;
				element = new T*[_cap];
				std::copy( old, old + _size, element );
				delete[] old;
				length = _cap;
			}
		}
		/**
		*   Allocates space for a certain number of objects, increases the size of the list.
		*   Note: Default constructor is used
		*/
		void allocate_size ( int sis )
		{
			if ( ( _size + sis ) > length )
			{
				throw except::Index_out_of_range ( "The size is larger than the length, please increase the size of the list" );
			}
			for ( int i = 0; i < sis; i++ )
			{
				push_back ( new T() );
			}
		}
		/**
		*   bool isEmpty()
		*   returns wheter or not the list is empty
		*/
		bool isEmpty() override
		{
			return _size == 0;
		}
		/**
		*   int getSize()
		*   returns current size of the list
		*/
		int getSize() override
		{
			return this->_size;
		}
		int getSize() const
		{
			return this->_size;
		}
		/**
		*   T get(int index)
		*   returns list element on given index
		*   Checks if the index is inside of bounds
		*/
		_type get ( int index ) override
		{
			checkIndex ( index );
			return element[index];
		}
		_type get ( int index ) const
		{
			checkIndex ( index );
			return element[index];
		}
		/**
		*   T operator [] (int index)
		*   returns list element on given index
		*   Does not check if the index is inside of bounds
		*/
		_type &operator[] ( int index )
		{
			checkIndex ( index );
			return element[index];
		}
		/**
		*   T operator [] (int index)
		*   returns list element on given index
		*   Does not check if the index is inside of bounds
		*/
		const _type &operator[] ( int index ) const
		{
			checkIndex ( index );
			return element[index];
		}
		/**
		*   int indexOf(T elem)
		*   returns index of given element
		*/
		int indexOf ( _type elem ) override
		{
			for ( int i = 0; i < _size; ++i )
				if ( elem == element[i] )
				{
					return i;
				}
			return -1;
		}
		/**
		*   T remove(int index)
		*   remove list element on given index and returns such element
		*/
		_type remove ( int index ) override
		{
			checkIndex ( index );
			_type removed = element[index];
			for ( int i = index; i < _size - 1; i++ )
			{
				element[ i ] = element[ i + 1 ];
			}
			element[ --_size ] = ( _type  ) nullptr;
			return removed;
		}
		/**
		*   T remove(int const MyIterator<_type> &iter)
		*   remove list element on given index and returns such element
		*/
		_type remove( MyIterator<_type> &iter )
		{
			return remove( iter._index );
		}
		/**
		*   T remove(const MyReverseIterator<T> &iter)
		*   remove list element on given index and returns such element
		*/
		_type remove( MyReverseIterator<_type> &iter )
		{
			return remove( iter._index );
		}
		/**
		*   T ObjectRemove(T elem)
		*   search the list for the given element and removes it
		*/
		_type ObjectRemove ( _type elem )
		{
			_type removed = ( _type  ) nullptr;
			if ( indexOf ( elem ) >= 0 )
			{
				removed = remove ( indexOf ( elem ) );
				return removed;
			}
			std::cout << "The element Does not Exist\n";
			return removed;
		}
		/**
		*   void add(int index,T elem)
		*   adds given element to the list in given index
		*/
		void add ( int index, _type elem ) override
		{
			if ( index < 0 || index > _size )
			{
				throw except::Index_out_of_range ( "IndexOutOfBounds" );
			}
			if ( _size == length )
			{
				_type *old = element;
				element = new T*[2 * _size];
				length *= 2;
				for ( int i = 0; i < _size; i++ )
				{
					element[i] = old[i];
				}

				delete[] old;
				//std::cout << "duplicate \n";
			}
			for ( int i = _size - 1; i >= index; i-- )
			{
				element[i + 1] = element[i];
			}
			element[index] = elem;
			_size++;
		}
		/**
		*   void push_back(T elem)
		*   adds given element to the end of the list
		*/
		void push_back ( _type elem )
		{
			add ( _size, elem );
		}
		/**
		*   void push_front(T elem)
		*   adds given element to the beginning of the list
		*/
		void push_front ( _type elem )
		{
			add ( 0, elem );
		}
		/**
		*   void pop_back()
		*   removes last element on the list and returns it
		*/
		_type pop_back()
		{
			return remove ( _size - 1 );
		}
		/**
		*   void pop_front()
		*   removes first element on the list and returns it
		*/
		_type pop_front()
		{
			return remove ( 0 );
		}
		/**
		*   void sort()
		*   Orders the list using the "natural" comparation of the objects contained by the list
		*   the elements must inherit the class Comparable from JComparator namespace and implements
		*   compareTo method
		*/
		void sort()
		{
			JComparator::ArraySort<_type >::quick_sort ( element, 0, this->_size - 1 );
		}
		/**
		*   void pSort()
		*   Orders the list in asscending order, this must be used instead of sort()
		*   to order primitive type of elements
		*/
		void pSort()
		{
			JComparator::ArraySort<_type >::primitiveArraySort ( element, 0, this->_size - 1 );
		}
		/**
		*   void sort(JComparator::Comparator<T>)
		*   Orders the list using a user defined comparator
		*/
		void sort ( JComparator::Comparator<_type > *c )
		{
			JComparator::ArraySort<_type >::quick_sort ( element, 0, this->_size - 1, c );
		}
		void hSort()
		{
			JComparator::ArraySort<_type>::heap_sort ( element, _size );
		}
		template<typename C> void hSort()
		{
			JComparator::ArraySort<_type>:: template _heap_sort<C> ( element, _size );
		}
		/**
		*   void clear()
		*   remove all elements stored in the list
		*/
		void clear()
		{
			if ( element )
			{
				delete[] element;
			}
			element = new _type[length];
			_size = 0;
			//std::cout << "List Cleared \n";
		}
		/**
		*   void print()
		*   prints the list to console
		*/
		virtual void print()
		{
			JIterator::Iterator<_type > *iter;
			for ( iter = this->iterator(); iter->hasNext(); )
			{
				_type t = iter->next();
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
			delete iter;
		}

		/**
		*   class ArrayLinearListIterator
		*   Java like iterator to iterate the list
		*/
		template <typename G>class ArrayLinearListIterator: public JIterator::Iterator<G>
		{
		private:
			ArrayLinearList<G> *list;
			int nextIndex;
		public:

			ArrayLinearListIterator ( ArrayLinearList *_list )
			{
				this->list = _list;
				nextIndex = 0;
			}
			bool hasNext()
			{
				return nextIndex < list->_size;
			}

			G &next()
			{
				if ( ! ( nextIndex < list->_size ) )
				{
					throw except::No_such_element ( "Element does not exist" );
				}
				return list->element[ nextIndex++ ];
			}
			G &current()
			{
				if ( nextIndex == list->getSize() )
				{
					throw except::No_such_element ( "Element does not exist" );
				}
				return list->element[ nextIndex ];
			}

			void remove() override
			{
				//throw except::Unsuported_operation ("Not yet supported operation");
				int cur = nextIndex - 1;
				list->checkIndex ( cur );
				//T removed = element[index];
				for ( int i = cur; i < list->_size - 1; i++ )
				{
					list->element[ i ] = list->element[ i + 1 ];
				}
				list->element[ --list->_size ] = ( _type  ) nullptr;
			}
		};

		/**
		*   ArrayLinearListIterator<T>* iterator()
		*   creates an ArrayLinearListIterator<T>
		*/
		virtual JIterator::Iterator<_type > *iterator() override
		{
			JIterator::Iterator<_type > *iter = new ArrayLinearListIterator<_type > ( this );
			return iter;
		}
		ArrayLinearListIterator<_type > _iterator ()
		{
			return ArrayLinearListIterator<_type > ( this );
		}
		template<typename C>class MyIterator
		{
			friend class ArrayLinearList<C>;
		protected:
			const ArrayLinearList<C> *list;
			int _index;

		public:
			MyIterator() : list( nullptr ), _index( 0 ) {}
			MyIterator ( const ArrayLinearList<C> *_list, bool is_end = false ) :
				list ( _list ), _index ( !is_end ? 0 : _list->_size )
			{}
			MyIterator( const MyIterator &other ) : MyIterator( other.list )
			{
				_index = other._index;
			}
			bool operator == ( const MyIterator &other )
			{
				return _index == other._index;
			}
			bool operator != ( const MyIterator &other )
			{
				return _index != other._index;
			}
			const MyIterator &operator++()
			{
				++_index;
				return *this;
			}
			_type &operator*() const
			{
				return list->element[_index];
			}
		};
		template<typename C>class MyReverseIterator : public MyIterator<C>
		{
			friend class ArrayLinearList<C>;
		public:
			MyReverseIterator() : MyIterator<C>() {}
			MyReverseIterator ( const ArrayLinearList<C> *_list, bool is_end = false ) :
				MyIterator<C> ( _list, is_end )
			{
				this->_index = is_end ? -1 : _list->_size - 1;
			}
			MyReverseIterator( const MyReverseIterator &other ) : MyReverseIterator( other.list )
			{
				this->_index = other._index;
			}
			bool operator == ( const MyIterator<C> &other )
			{
				return this->_index == other._index;
			}
			bool operator != ( const MyIterator<C> &other )
			{
				return this->_index != other._index;
			}
			const MyIterator<C> &operator++()
			{
				--this->_index;
				return *this;
			}
			T &operator*() const
			{
				return this->list->element[this->_index];
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
		*   Returns an iterator pointing to the beginning of the list
		*/
		MyReverseIterator<_type > rbegin () const
		{
			return MyReverseIterator<_type > ( this );
		}
		/**
		*   Returns an iterator pointing to the end of the list
		*/
		MyReverseIterator<_type > rend () const
		{
			return MyReverseIterator<_type > ( this, true );
		}
		/**
		*   Overload of the << operator to print all the elements on the list
		*/
		friend std::ostream &operator<< ( std::ostream &out, ArrayLinearList<_type > *list )
		{
			list->osPrint ( out );
			return out;
		}

		friend std::ostream &operator<< ( std::ostream &out, ArrayLinearList<_type > &list )
		{
			list.osPrint ( out );
			return out;
		}

		~ArrayLinearList()
		{
			clear();
			if ( element )
			{
				delete[] element;
			}
		}
	};
}
#endif // ARRAYLINEARLIST_HPP_INCLUDED
