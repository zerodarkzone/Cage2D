// *** ADDED BY HEADER FIXUP ***
#include <cstdlib>
#include <fstream>
#include <iostream>
// *** END ***
#ifndef RANDOMITERATIONLIST_HPP_INCLUDED
#define RANDOMITERATIONLIST_HPP_INCLUDED
#include <ctime>
#include "ArrayLinearListImproved.hpp"

namespace JList
{

template <typename L>class RandomIterationList: public ArrayLinearListImproved<L>
{

public:
	RandomIterationList ( int initialCapacity ) : ArrayLinearListImproved<L> ( initialCapacity ) {}
	RandomIterationList() : ArrayLinearListImproved<L>() {}

	template <typename J>class RandomIterationListIterator: public JIterator::Iterator<J>
	{
	private:
		RandomIterationList<J> *list;
		int nextIndex;
		int randValue;
		int *randomList;
		int ranCounter;
		int randlenght;
		bool repeated;

	public:

		RandomIterationListIterator ( RandomIterationList *_list )
		{
			this->list = _list;
			nextIndex = 0;
			ranCounter = 0;
			randlenght = 0;
			randValue = 0;
			uint32_t n = list->getSize();
			randomList = new int [n];
			repeated = true;
			srand ( time ( NULL ) );
			for ( uint32_t i = 0; i < n ; i++ )
			{
				randomList[i] = -1;
			}
		}

		bool hasNext()
		{
			return nextIndex < list->_size;
		}

		J &next()
		{
			if ( ! ( nextIndex < list->_size ) )
			{
				throw except::No_such_element ( "NoSuchElement" );
			}
			while ( repeated )
			{
				randValue = randInt ( 0, list->getSize() );
				for ( int i = 0; i <= randlenght; i++ )
				{
					if ( randomList[i] == randValue )
					{
						repeated = true;
						break;
					}
					else
					{
						repeated = false;
					}
				}
				if ( !repeated )
				{
					randomList[ ranCounter ] = randValue;
					randlenght ++;
					ranCounter ++;
				}
			}
			repeated = true;
			nextIndex ++;
			return list->element[ randValue ];
		}
		J &current()
		{
			return list->element[ nextIndex ];
		}

		void remove()
		{
			throw except::Unsuported_operation ( "Not yet supported operation" );
		}

		int randInt ( int min, int max )
		{
			int randomNum = min + rand() % ( max - min );
			return randomNum;
		}
		~RandomIterationListIterator()
		{
			delete []randomList;
		}
	};


	/**
	*   RandomIterationListIterator<T>* iterator()
	*   creates an ArrayLinearListIterator<T>
	*/

	virtual JIterator::Iterator<L> *iterator() override
	{
		JIterator::Iterator<L> *iter = new RandomIterationListIterator<L> ( this );
		return iter;
	}
	RandomIterationListIterator<L> iterator ( bool f )
	{
		UNREFERENCED_PARAMETER ( f )
		RandomIterationListIterator<L> it ( this );
		return it;
	}

	JIterator::Iterator<L> *normalIterator()
	{
		return ArrayLinearList<L>::iterator();
	}


	void save ( char *path )
	{
		char *file = ( char * ) path;
		this->sav = &this->size;
		///Saves the current size of the list to binary File
		std::ofstream fsalida ( file,
								std::ios::out | std::ios::binary );
		fsalida.write ( reinterpret_cast<char *> ( this->sav ),
						sizeof ( int ) );
		///Saves the elements in the list to binary File
		JIterator::Iterator<L> *iter;
		//if Q is a Pointer
		if ( std::is_pointer<L>::value )
		{
			for ( iter = this->normalIterator(); iter->hasNext(); )
			{
				fsalida.write ( reinterpret_cast<char *> ( iter->next() ),
								sizeof ( L ) );
			}
		}
		//if Q is not a Pointer
		else
		{
			for ( iter = this->normalIterator(); iter->hasNext(); )
			{
				this->ele = new L;
				*this->ele = iter->next();
				fsalida.write ( reinterpret_cast<char *> ( this->ele ),
								sizeof ( L ) );
				delete this->ele;
			}
		}
		fsalida.close();
		std::cout << "Save done :) " << std::endl;
		delete iter;
	}

protected:
	/**
	*   void osPrint(std::ostream& out)
	*   prints the list to an ostream object
	*/
	virtual void osPrint ( std::ostream &out )
	{
		L elem = ( L ) nullptr;
		_for_each ( elem, this )
		{
			if ( std::is_pointer<L>::value )
			{
				out << "*";
			}
			out << "[" << elem << "]  ";
		}
		if ( this->_size <= 0 )
		{
			out << "The list is empty";
		}
		out << "\n";
	}


};


}


#endif // RANDOMITERATIONLIST_HPP_INCLUDED
