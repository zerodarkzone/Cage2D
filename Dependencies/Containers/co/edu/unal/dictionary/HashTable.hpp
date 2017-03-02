#ifndef HASHTABLE_HPP_INCLUDED
#define HASHTABLE_HPP_INCLUDED
#include "Dictionary.hpp"
#include <cmath>
#include <stdexcept>
namespace JDict
{
	template <typename K, class E, class Hash = std::hash<K>, class Compare = Equals<K>> class HashTable : public Dictionary<K, E>
	{
	protected:
		template <typename KEY, class EL> class HashEntry;
		template <typename KEY, class EL> class HashEntry<KEY*, EL>;
		Hash _hash;
		int divisor;
		Compare c;
		HashEntry<K, E>** table;
		bool* neverUsed;
		int _size;

		template <typename KEY, class EL> class HashEntry
		{
			friend class HashTable;
		protected:
			KEY key;
			EL element;
			HashEntry() {}
			HashEntry (KEY theKey, EL theElement)
			{
				this->key = theKey;
				this->element = theElement;
			}
		};
		template <typename KEY, class EL> class HashEntry<KEY*, EL>
		{
			friend class HashTable;
		protected:
			KEY* key;
			EL element;
			HashEntry()
			{
				key = nullptr;
			}
			HashEntry (KEY* theKey, EL theElement)
			{
				this->key = theKey;
				this->element = theElement;
			}
		};
	public:
		HashTable (int theDivisor)
		{
			divisor = theDivisor + 2;
			table = new HashEntry<K, E>* [divisor];
			neverUsed = new bool[divisor];
			for (int i = 0; i < divisor; i++)
			{
				neverUsed[i] = true;
				table[i] = nullptr;
			}
			_size = 0;
		}
		HashTable() : HashTable (202) {}
		bool isEmpty()
		{
			return _size == 0;
		}
		int size()
		{
			return _size;
		}
		int max_size()
		{
			return divisor;
		}
		void resize (int new_size)
		{
			int n_size = new_size + 2;
			if (n_size < divisor)
				throw std::invalid_argument ("the new size must be greater than the original");
			HashEntry<K, E>** old = table;
			table = new HashEntry<K, E*>* [n_size];
			bool* n_old = neverUsed;
			neverUsed = new bool[n_size];
			for (int i = 0; i < n_size; i++)
			{
				neverUsed[i] = true;
				table[i] = nullptr;
			}
			for (int i = 0; i < divisor; i++)
			{
				table[i] = old[i];
				neverUsed[i] = n_old[i];
			}
			divisor = n_size;
			delete [] old;
			delete [] n_old;
		}
	private:
		int search (const K& theKey)
		{
			int i = ( (unsigned int) _hash (theKey)) % divisor; //Math.abs (theKey.hashCode()) % divisor;
			int j = i; //start at home bucket
			do
			{
				if (neverUsed[j] || (table[j] != nullptr &&
									 c (table[j]->key, theKey)))
					return j;
				j = (j + 1) % divisor; //next bucket
			}
			while (j != i);
			return j; //table full
		}
	public:
		E& get (const K& key) override
		{
			int b = search (key);
			// see if a match was found at table[b]
			if (neverUsed[b] || !c (table[b]->key, key))
			{
				throw Key_not_found ("The given key doesn't exist");
			}
			return table[b]->element;
		}
		E& operator[] (const K& key)
		{
			int b = search (key);
			// see if a match was found at table[b]
			if (neverUsed[b] || !c (table[b]->key, key))
			{
				put (key, E());
			}
			return table[b]->element;
		}
		/** insert an element with the specified key
		* overwrite old element if there is already an
		* element with the given key
		* @throws invalid_argument when the table is full
		* @return old element ( if any ) with key theKey */

		E put (const K& key, const E& element) override
		{
			// search the table for a matching element
			int b = search (key);
			// cheack if matching element found
			if (neverUsed[b])
			{
				table[b] = new HashEntry<K, E> (key, element);
				neverUsed[b] = false;
				_size ++;
				return element;
			}
			else
			{
				//check if duplicate or table full
				if (c (table[b]->key, key))
				{
					E elementToReturn = table[b]->element;
					table[b]->element = element;
					return elementToReturn;
				}
				else throw std::invalid_argument ("Table is full");
			}
		}
		/** remove from the hash table
		 * @return pointer to removed element */
		E remove (const K& key) override
		{
			//search the table for a matching element
			int b = search (key);
			E elementToReturn;
			if (neverUsed[b])
				return E();
			if (c (table[b]->key, key))
			{
				elementToReturn = table[b]->element;
				delete table[b];
				table[b] = nullptr;
				_size --;
			}
			return elementToReturn;
		}
		bool contains_key (const K& key)
		{
			int b = search (key);
			if (neverUsed[b] || !c (table[b]->key, key))
				return false;
			return true;
		}
		~HashTable()
		{
			for (int i = 0; i < divisor; i++)
			{
				if (table[i])
				{
					delete table[i];
				}
			}
			delete [] table;
			delete [] neverUsed;
		}
	};
	/**
	*   Partial specialization for pointers
	*/
	template <typename K, class E, class Hash, class Compare> class HashTable<K, E*, Hash, Compare> : public Dictionary<K, E*>
	{
	protected:
		template <typename KEY, class EL> class HashEntry;
		template <typename KEY, class EL> class HashEntry<KEY*, EL>;
		std::hash<K> _hash;
		int divisor;
		Compare c;
		HashEntry<K, E*>** table;
		bool* neverUsed;
		int _size;

		template <typename KEY, class EL> class HashEntry
		{
			friend class HashTable;
		protected:
			KEY key;
			EL element;
			HashEntry()
			{
				element = nullptr;
			}
			HashEntry (KEY theKey, EL theElement)
			{
				this->key = theKey;
				this->element = theElement;
			}
		};
		template <typename KEY, class EL> class HashEntry<KEY*, EL>
		{
			friend class HashTable;
		protected:
			KEY* key;
			EL element;
			HashEntry()
			{
				key = nullptr;
				element = nullptr;
			}
			HashEntry (KEY* theKey, EL theElement)
			{
				this->key = theKey;
				this->element = theElement;
			}
		};
	public:
		HashTable (int theDivisor)
		{
			divisor = theDivisor + 2;
			table = new HashEntry<K, E*>* [divisor];
			neverUsed = new bool[divisor];
			for (int i = 0; i < divisor; i++)
			{
				neverUsed[i] = true;
				table[i] = nullptr;
			}
			_size = 0;
		}
		HashTable() : HashTable (202) {}
		bool isEmpty()
		{
			return _size == 0;
		}
		int size()
		{
			return _size;
		}
		int max_size()
		{
			return divisor;
		}
		void resize (int new_size)
		{
			int n_size = new_size + 2;
			if (n_size < divisor)
				throw std::invalid_argument ("the new size must be greater than the original");
			HashEntry<K, E*>** old = table;
			table = new HashEntry<K, E*>* [n_size];
			bool* n_old = neverUsed;
			neverUsed = new bool[n_size];
			for (int i = 0; i < n_size; i++)
			{
				neverUsed[i] = true;
				table[i] = nullptr;
			}
			for (int i = 0; i < divisor; i++)
			{
				table[i] = old[i];
				neverUsed[i] = n_old[i];
			}
			divisor = n_size;
			delete [] old;
			delete [] n_old;
		}
	private:
		int search (const K& theKey)
		{
			int i = (unsigned int) _hash (theKey) % divisor;
			int j = i; //start at home bucket
			do
			{
				if (neverUsed[j] || (table[j] != nullptr &&
									 c (table[j]->key, theKey)))
					return j;
				j = (j + 1) % divisor; //next bucket
			}
			while (j != i);
			return j; //table full
		}
	public:
		E* get (const K& key)
		{
			int b = search (key);
			// see if a match was found at table[b]
			if (neverUsed[b] || !c (table[b]->key, key))
			{
				throw Key_not_found ("The key doesn't exist");
			}
			return table[b]->element;
		}
		E*& operator[] (const K& key)
		{
			int b = search (key);
			// see if a match was found at table[b]
			if (neverUsed[b] || !c (table[b]->key, key))
			{
				put (key, nullptr);
			}
			return table[b]->element;
		}
		/** insert an element with the specified key
		* overwrite old element if there is already an
		* element with the given key
		* @throws IllegalArgumentException when the table is full
		* @return old element ( if any ) with key theKey */

		E* put (const K& key, E* element)
		{
			// search the table for a matching element
			int b = search (key);
			// cheack if matching element found
			if (neverUsed[b])
			{
				table[b] = new HashEntry<K, E*> (key, element);
				neverUsed[b] = false;
				_size ++;
				return element;
			}
			else
			{
				//check if duplicate or table full
				if (c (table[b]->key, key))
				{
					E* elementToReturn = table[b]->element;
					table[b]->element = element;
					return elementToReturn;
				}
				else throw std::invalid_argument ("Table is full");
			}
		}
		/** remove from the hash table
		 * @return pointer to removed element */
		E* remove (const K& key)
		{
			E* elementToReturn = nullptr;
			//search the table for a matching element
			int b = search (key);
			if (neverUsed[b])
				return nullptr;
			if (c (table[b]->key, key))
			{
				elementToReturn = table[b]->element;
				delete table[b];
				table[b] = nullptr;
				_size --;
			}
			return elementToReturn;
		}
		bool contains_key (const K& key)
		{
			int b = search (key);
			if (neverUsed[b] || !c (table[b]->key, key))
				return false;
			return true;
		}
		~HashTable()
		{
			for (int i = 0; i < divisor; i++)
			{
				if (table[i])
				{
					delete table[i];
				}
			}
			delete [] table;
			delete [] neverUsed;
		}

	};
}
#endif // HASHTABLE_HPP_INCLUDED
