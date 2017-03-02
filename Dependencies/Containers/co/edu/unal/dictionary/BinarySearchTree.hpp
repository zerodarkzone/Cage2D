#ifndef BINARYSEARCHTREE_HPP_INCLUDED
#define BINARYSEARCHTREE_HPP_INCLUDED
#include "Dictionary.hpp"
#include <memory>
#include "co\edu\unal\BinaryTree\BinaryNode.h"

namespace JDict
{

#define BSTN(_T, _T2) b_tree::BinaryNode<std::shared_ptr<JDict::Data<_T,_T2>>>

template<typename K, typename T> struct Data
{
	T element;
	K key;
	Data ( K _key, T _element ) : element ( _element ), key ( _key ) {}
};
template<typename K, typename T, typename C = Comp<K>, typename Eq = Equals<K>> class BinarySearchTree : public Dictionary<K, T>
{
private:
	C c;
	Eq eq;
	BSTN( K, T ) *_root;
	std::shared_ptr<Data<K, T>> old;

	void destroy_subtree ( BSTN( K, T ) *&leaf )
	{

		if ( leaf != nullptr )
		{
			destroy_subtree ( leaf->left );
			destroy_subtree ( leaf->right );
			std::cout << "leaf deleted\n";
		}
		//delete leaf->element;
		//leaf->element = nullptr;
		delete leaf;
		leaf = nullptr;
	}
	void insert ( const std::shared_ptr<Data<K, T>> &data, BSTN( K, T ) *leaf )
	{
		if ( c ( data->key, leaf->element->key ) )
		{
			if ( leaf->left != nullptr )
			{
				insert ( data, leaf->left );
			}
			else
			{
				old = nullptr;
				leaf->left = new BSTN( K, T )();
				leaf->left->element = data;
				leaf->left->left = nullptr;
				leaf->left->right = nullptr;
				leaf->left->parent = leaf;
			}
		}
		else if ( eq ( data->key, leaf->element->key ) )
		{
			old = leaf->element;
			leaf->element = data;
		}
		else
		{
			old = nullptr;
			if ( leaf->right != nullptr )
			{
				insert ( data, leaf->right );
			}
			else
			{
				leaf->right = new BSTN( K, T )();
				leaf->right->element = data;
				leaf->right->right = nullptr;
				leaf->right->left = nullptr;
				leaf->right->parent = leaf;
			}
		}
	}
	BSTN( K, T ) *search ( K key, BSTN( K, T ) *leaf )
	{
		if ( leaf == nullptr )
		{
			return nullptr;
		}
		if ( eq ( leaf->element->key, key ) )
		{
			return leaf;
		}
		if ( c ( key, leaf->element->key ) )
		{
			return search ( key, leaf->left );
		}
		else
		{
			return search ( key, leaf->right );
		}
	}
	static void print_leaf ( BSTN( K, T ) *leaf )
	{
		std::cout << "[" << leaf->element->element << "]";
	}
	void in_order ( BSTN( K, T ) *leaf, void ( *func ) ( BSTN( K, T ) *leaf ) )
	{
		if ( leaf != nullptr )
		{
			in_order ( leaf->left, func );
			func ( leaf );
			in_order ( leaf->right, func );
		}
	}
	void pre_order ( BSTN( K, T ) *leaf, void ( *func ) ( BSTN( K, T ) *leaf ) )
	{
		if ( leaf != nullptr )
		{
			func ( leaf );
			pre_order ( leaf->left, func );
			pre_order ( leaf->right, func );
		}
	}
	void post_order ( BSTN( K, T ) *leaf, void ( *func ) ( BSTN( K, T ) *leaf ) )
	{
		if ( leaf != nullptr )
		{
			post_order ( leaf->left, func );
			post_order ( leaf->right, func );
			func ( leaf );
		}
	}

public:
	BinarySearchTree ( BSTN( K, T ) *node ) : _root ( node ), old ( nullptr ) {}
	BinarySearchTree() : _root ( nullptr ), old ( nullptr ) {}
	~BinarySearchTree()
	{
		destroy_tree();
	}
	void destroy_tree()
	{
		destroy_subtree ( _root );
		//_root = nullptr;
	}
	T put ( const K &key, const T &elem )
	{
		if ( _root == nullptr )
		{
			_root = new BSTN( K, T )();
			_root->element = std::shared_ptr<Data<K, T>>( new Data<K, T> ( key, elem ) ); //new Data<K, T> (key, elem);
			_root->left = nullptr;
			_root->right = nullptr;
			_root->parent = nullptr;
		}
		else
		{
			insert ( std::shared_ptr<Data<K, T>>( new Data<K, T> ( key, elem ) ), _root );
		}
		if ( old )
		{
			T ele = old->element;
			//delete old;
			old = nullptr;
			return ele;
		}
		return elem;
	}
	T &operator[] ( K key )
	{
		BSTN( K, T ) *elem = search ( key, _root );
		if ( !elem )
		{
			Data<K, T> *dat = new Data<K, T> ( key, T() );
			insert ( dat, _root );
			return dat->element->element;
		}
		return elem->element->element;
	}
	T &get ( const K &key )
	{
		BSTN( K, T ) *elem = search ( key, _root );
		if ( !elem )
		{
			throw Key_not_found ( "The given key doesn't exist" );
		}
		return elem->element->element;
	}


	T remove( const K &key )
	{
		BSTN( K, T ) *p = _root;
		BSTN( K, T ) *pp = nullptr;

		while ( p != nullptr && !eq( p->element->key, key ) )
		{
			pp = p;
			if ( c( key, p->element->key ) )
			{
				p = p->left;
			}
			else
			{
				p = p->right;
			}
		}

		if ( p == nullptr )
		{
			std::cout << "Key not found" << std::endl;
			return T(); //key not found
		}
		T elementToReturn = p->element->element;

		if ( p->left != nullptr && p->right != nullptr )
		{
			BSTN( K, T ) *s = p->left;
			BSTN( K, T ) *ps = p;
			while ( s->right != nullptr )
			{
				ps = s;
				s = s->right;
			}

			// move largest element from s to p
			p->element = s->element;
			p = s;
			pp = ps;
		}

		// p has at most one child, save this child in c
		BSTN( K, T ) *c;
		if ( p->left == nullptr )
		{
			c = p->right;
		}
		else
		{
			c = p->left;
		}

		// remove node p
		if ( p == _root )
		{
			_root = c;
		}
		else
		{
			if ( p == pp->left )
			{
				pp->left = c;
			}
			else
			{
				pp->right = c;
			}
		}

		delete p;
		return elementToReturn;
	}

	bool contains_key ( const K &key )
	{
		auto dat = search ( key, _root );
		if ( !dat )
		{
			return false;
		}
		return true;
	}
	void in_order ( void ( *func ) ( BSTN( K, T ) *leaf ) )
	{
		in_order ( _root, func );
	}
	void pre_order ( void ( *func ) ( BSTN( K, T ) *leaf ) )
	{
		pre_order ( _root, func );
	}
	void post_order ( void ( *func ) ( BSTN( K, T ) *leaf ) )
	{
		post_order ( _root, func );
	}
	void print()
	{
		in_order ( _root, &print_leaf );
	}
};

/**
*   Partial specialization for pointers
*/

template<typename K, typename T, typename C, typename Eq> class BinarySearchTree<K, T *, C, Eq> : public Dictionary<K, T *>
{
private:
	C c;
	Eq eq;
	BSTN( K, T * ) *_root;
	std::shared_ptr<Data<K, T *>> old;
	void destroy_subtree ( BSTN( K, T * ) *&leaf )
	{
		if ( leaf != nullptr )
		{
			destroy_subtree ( leaf->left );
			destroy_subtree ( leaf->right );
		}
		delete leaf;
		leaf = nullptr;
	}
	void insert ( const std::shared_ptr<Data<K, T *>> &data, BSTN( K, T * ) *leaf )
	{
		if ( c ( data->key, leaf->element->key ) )
		{
			if ( leaf->left != nullptr )
			{
				insert ( data, leaf->left );
			}
			else
			{
				old = nullptr;
				leaf->left = new BSTN( K, T * )();
				leaf->left->element = data;
				leaf->left->left = nullptr;
				leaf->left->right = nullptr;
				leaf->left->parent = leaf;
			}
		}
		else if ( eq ( data->key, leaf->element->key ) )
		{
			old =  leaf->element;
			leaf->element = data;
		}
		else
		{
			if ( leaf->right != nullptr )
			{
				insert ( data, leaf->right );
			}
			else
			{
				old = nullptr;
				leaf->right = new BSTN( K, T * )();
				leaf->right->element = data;
				leaf->right->right = nullptr;
				leaf->right->left = nullptr;
				leaf->right->parent = leaf;
			}
		}
	}
	BSTN( K, T * ) *search ( K key, BSTN( K, T * ) *leaf )
	{
		if ( leaf == nullptr )
		{
			return nullptr;
		}
		if ( eq ( leaf->element->key, key ) )
		{
			return leaf;
		}
		if ( c ( key, leaf->element->key ) )
		{
			return search ( key, leaf->left );
		}
		else
		{
			return search ( key, leaf->right );
		}
	}
	static void print_leaf ( BSTN( K, T * ) *leaf )
	{
		std::cout << "[" << ( *leaf->element->element ) << "]";
	}
	void in_order ( BSTN( K, T * ) *leaf, void ( *func ) ( BSTN( K, T * ) *leaf ) )
	{
		if ( leaf != nullptr )
		{
			in_order ( leaf->left, func );
			func ( leaf );
			in_order ( leaf->right, func );
		}
	}
	void pre_order ( BSTN( K, T * ) *leaf, void ( *func ) ( BSTN( K, T * ) *leaf ) )
	{
		if ( leaf != nullptr )
		{
			func ( leaf );
			pre_order ( leaf->left, func );
			pre_order ( leaf->right, func );
		}
	}
	void post_order ( BSTN( K, T * ) *leaf, void ( *func ) ( BSTN( K, T * ) *leaf ) )
	{
		if ( leaf != nullptr )
		{
			post_order ( leaf->left, func );
			post_order ( leaf->right, func );
			func ( leaf );
		}
	}

public:
	BinarySearchTree ( BSTN( K, T * ) *node ) : _root ( node ), old ( nullptr ) {}
	BinarySearchTree() : _root ( nullptr ), old ( nullptr ) {}
	~BinarySearchTree()
	{
		destroy_tree();
	}
	void destroy_tree()
	{
		destroy_subtree ( _root );
	}
	T *put ( const K &key, T *elem )
	{
		if ( _root == nullptr )
		{
			_root = new BSTN( K, T * )();
			_root->element = std::shared_ptr<Data<K, T *>>( new Data<K, T *> ( key, elem ) );
			_root->left = nullptr;
			_root->right = nullptr;
		}
		else
		{
			insert ( std::shared_ptr<Data<K, T *>>( new Data<K, T *> ( key, elem ) ), _root );
		}
		if ( old )
		{
			T *ele = old->element;
			//delete old;
			old = nullptr;
			return ele;
		}
		return elem;
	}
	T *operator[] ( K key )
	{
		auto elem = search ( key, _root );
		if ( !elem )
		{
			auto dat = new Data<K, T *> ( key, nullptr );
			insert ( dat, _root );
			return dat->element->element;
		}
		return elem->element->element;
	}
	T *get ( const K &key )
	{
		auto elem = search ( key, _root );
		if ( !elem )
		{
			throw Key_not_found ( "The given key doesn't exist" );
		}
		return elem->element->element;
	}


	T *remove( const K &key )
	{
		BSTN( K, T * ) *p = _root;
		BSTN( K, T * ) *pp = nullptr;

		while ( p != nullptr && !eq( p->element->key, key ) )
		{
			pp = p;
			if ( c( key, p->element->key ) )
			{
				p = p->left;
			}
			else
			{
				p = p->right;
			}
		}

		if ( p == nullptr )
		{
			return nullptr; //key not found
		}
		T *elementToReturn = p->element->element;

		if ( p->left != nullptr && p->right != nullptr )
		{
			BSTN( K, T * ) *s = p->left;
			BSTN( K, T * ) *ps = p;
			while ( s->right != nullptr )
			{
				ps = s;
				s = s->right;
			}

			// move largest element from s to p
			p->element = s->element;
			p = s;
			pp = ps;
		}

		// p has at most one child, save this child in c
		BSTN( K, T * ) *c;
		if ( p->left == nullptr )
		{
			c = p->right;
		}
		else
		{
			c = p->left;
		}

		// remove node p
		if ( p == _root )
		{
			_root = c;
		}
		else
		{
			if ( p == pp->left )
			{
				pp->left = c;
			}
			else
			{
				pp->right = c;
			}
		}
		delete p;
		return elementToReturn;
	}

	bool contains_key ( const K &key )
	{
		auto dat = search ( key, _root );
		if ( !dat )
		{
			return false;
		}
		return true;
	}
	void in_order ( void ( *func ) ( BSTN( K, T * ) *leaf ) )
	{
		in_order ( _root, func );
	}
	void pre_order ( void ( *func ) ( BSTN( K, T * ) *leaf ) )
	{
		pre_order ( _root, func );
	}
	void post_order ( void ( *func ) ( BSTN( K, T * ) *leaf ) )
	{
		post_order ( _root, func );
	}
	void print()
	{
		in_order ( _root, &print_leaf );
	}
};
}

#endif // BINARYSEARCHTREE_HPP_INCLUDED
