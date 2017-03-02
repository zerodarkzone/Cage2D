// *** ADDED BY HEADER FIXUP ***
#include <iostream>
#include <string>
// *** END ***
#ifndef ARRAYLINEARLISTIMPROVED_HPP_INCLUDED
#define ARRAYLINEARLISTIMPROVED_HPP_INCLUDED
#include "ArrayLinearList.hpp"
#include "LinearListImproved.hpp"
#include <fstream>
namespace JList
{
/**
*   class ArrayLinearlistImproved
*   subClass of ArrayLiniarList with extra methods:
*   save(),load(),
*   ///to do: sort()
*/
template <typename Q> class ArrayLinearListImproved: public ArrayLinearList<Q>, public LinearListImproved<Q>
{
protected:
	int *sav;
	int *lod;
	Q *ele;
	Q *elo;
public:
	/**
	*   ArrayLinearListImproved(int initialCapacity)
	*   creates a T type array with size equals to initialCapacity to store the elements in the list
	*   sets to Null pointer class members
	*   ArrayLinearListImproved()
	*   creates a T type array with size equals to 20 store the elements in the list
	*   sets to Null pointer class members
	*/
	ArrayLinearListImproved ( int initialCapacity ) : ArrayLinearList<Q> ( initialCapacity )
	{
		sav = nullptr;
		lod = nullptr;
		elo = nullptr;
		ele = nullptr;
	}
	ArrayLinearListImproved() : ArrayLinearList<Q>()
	{
		sav = nullptr;
		lod = nullptr;
		elo = nullptr;
		ele = nullptr;
	}

	/**
	*   void save(char *)
	*   saves the list items to a given file, if the file doesnot exist, it gets created
	*/
	virtual void save ( std::string path ) override
	{
		sav = &this->_size;
		///Saves the current size of the list to binary File
		std::ofstream fsalida ( path,
								std::ios::out | std::ios::binary );
		fsalida.write ( reinterpret_cast<char *> ( sav ),
						sizeof ( int ) );
		///Saves the elements in the list to binary File
		JIterator::Iterator<Q> *iter;
		//if Q is not a Pointer
		{
			for ( iter = this->iterator(); iter->hasNext(); )
			{
				ele = new Q;
				*ele = iter->next();
				fsalida.write ( reinterpret_cast<char *> ( ele ),
								sizeof ( Q ) );
				delete ele;
			}
		}
		fsalida.close();
		std::cout << "Save done :) " << std::endl;
		delete iter;
	}
	/**
	*   void load(char *)
	*   loads all elementes stored into a file and adds them to the list
	*   if the file does not exist or if it is empty, a runtime_error exception is throwed
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
		this->clear();
		///if Q is not a pointer
		{
			for ( int i = 0; i < *lod; i++ )
			{
				elo = new Q;
				fentrada.read ( reinterpret_cast<char *> ( elo ),
								sizeof ( Q ) );
				this->push_back ( *elo );
			}
		}
		fentrada.close();
		std::cout << "Load done <:-{D" << std::endl;
	}

	virtual JIterator::Iterator<Q> *iterator()
	{
		return ArrayLinearList<Q>::iterator();
	}
	~ArrayLinearListImproved()
	{
		this->clear();
	}
};

}
namespace JList
{
/**
*   class ArrayLinearlistImproved
*   subClass of ArrayLiniarList with extra methods:
*   save(),load(),
*   ///to do: sort()
*/
template <typename Q> class ArrayLinearListImproved<Q *>: public ArrayLinearList<Q *>, public LinearListImproved<Q *>
{
protected:
	int *sav;
	int *lod;
	Q *ele;
	Q *elo;
public:
	/**
	*   ArrayLinearListImproved(int initialCapacity)
	*   creates a T type array with size equals to initialCapacity to store the elements in the list
	*   sets to Null pointer class members
	*   ArrayLinearListImproved()
	*   creates a T type array with size equals to 20 store the elements in the list
	*   sets to Null pointer class members
	*/
	ArrayLinearListImproved ( int initialCapacity ) : ArrayLinearList<Q * > ( initialCapacity )
	{
		sav = nullptr;
		lod = nullptr;
		elo = nullptr;
		ele = nullptr;
	}
	ArrayLinearListImproved() : ArrayLinearList<Q * >()
	{
		sav = nullptr;
		lod = nullptr;
		elo = nullptr;
		ele = nullptr;
	}

	/**
	*   void save(char *)
	*   saves the list items to a given file, if the file doesnot exist, it gets created
	*/
	virtual void save ( std::string path ) override
	{
		sav = &this->_size;
		///Saves the current size of the list to binary File
		std::ofstream fsalida ( path,
								std::ios::out | std::ios::binary );
		fsalida.write ( reinterpret_cast<char *> ( sav ),
						sizeof ( int ) );
		///Saves the elements in the list to binary File
		JIterator::Iterator<Q *> *iter;
		for ( iter = this->iterator(); iter->hasNext(); )
		{
			fsalida.write ( reinterpret_cast<char *> ( iter->next() ),
							sizeof ( Q ) );
		}
		fsalida.close();
		std::cout << "Save done :) " << std::endl;
		delete iter;
	}
	/**
	*   void load(char *)
	*   loads all elementes stored into a file and adds them to the list
	*   if the file does not exist or if it is empty, a runtime_error exception is throwed
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
		this->clear();
		///if Q is a pointer
		if ( std::is_pointer<Q *>::value )
		{
			for ( int i = 0; i < *lod; i++ )
			{
				Q *lo = new Q();
				fentrada.read ( reinterpret_cast<char *> ( ( lo ) ),
								sizeof ( Q ) );
				this->push_back ( lo );
				lo = nullptr;
			}
		}
		fentrada.close();
		std::cout << "Load done <:-{D" << std::endl;
	}

	virtual JIterator::Iterator<Q *> *iterator()
	{
		return ArrayLinearList<Q *>::iterator();
	}
	~ArrayLinearListImproved()
	{
		this->clear();
	}
};

}
#endif // ARRAYLINEARLISTIMPROVED_HPP_INCLUDED
