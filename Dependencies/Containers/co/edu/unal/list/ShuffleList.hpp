// *** ADDED BY HEADER FIXUP ***
#include <cstdlib>
// *** END ***
#ifndef SHUFFLELIST_HPP_INCLUDED
#define SHUFFLELIST_HPP_INCLUDED
#include <ctime>
#include "ArrayLinearListImproved.hpp"
namespace JList
{

template <typename L>class ShuffleList: public ArrayLinearListImproved<L>
{
public:
	ShuffleList ( int initialCapacity ) : ArrayLinearListImproved<L> ( initialCapacity )
	{
		srand ( time ( NULL ) );
	}
	ShuffleList() : ArrayLinearListImproved<L>()
	{
		srand ( time ( NULL ) );
	}
	/**
		*   ArrayLinearList (std::initializer_list<T> il)
		*   creates a T type array with size = il.size to store the elements in the list
		*   and copy the elements of the initializer list to the array
		*/
	ShuffleList ( std::initializer_list<L> il ) : ArrayLinearListImproved<L> ( ( int ) il.size() )
	{
		for ( auto i : il )
		{
			this->push_back ( i );
		}
	}
	void shuffle()
	{
		for ( unsigned int i = 0; i < this->_size; i++ )
		{
			int j = randInt ( i, this->_size );
			swap ( j, i );
		}
	}

private:
	void swap ( int index1, int index2 )
	{
		if ( index1 < 0 || index1 > ( int ) this->_size || index2 < 0 || index2 > ( int ) this->_size )
		{
			throw except::Index_out_of_range ( "IndexOutOfBounds" );
		}
		L x = this->element[index1];
		this->element[index1] = this->element[index2];
		this->element[index2] = x;
	}

	int randInt ( int min, int max )
	{
		int randomNum = min + rand() % ( max - min );
		return randomNum;
	}
};

}
#endif // SHUFFLELIST_HPP_INCLUDED
