#ifndef BINARYTREE_HPP_INCLUDED
#define BINARYTREE_HPP_INCLUDED
#include "BinaryNode.h"
#include "co/edu/unal/comp/Comp.hpp"

namespace b_tree
{
	template<typename T, typename C = Comp<T>, typename Eq = Equals<T>>class BinaryTree
	{
	private:
		C c;
		Eq eq;

		BinaryNode<T>* _root;

		void destroy_subtree (BinaryNode<T>* leaf)
		{
			if (leaf != nullptr)
			{
				destroy_subtree (leaf->left);
				destroy_subtree (leaf->right);
				delete leaf;
				leaf = nullptr;
			}
		}
		void insert (T elem, BinaryNode<T>* leaf)
		{
			if (c (elem, leaf->element))
			{
				if (leaf->left != nullptr)
					insert (elem, leaf->left);
				else
				{
					leaf->left = new BinaryNode<T>();
					leaf->left->element = elem;
					leaf->left->left = nullptr;
					leaf->left->right = nullptr;
					leaf->left->parent = leaf;
				}
			}
			else
			{
				if (leaf->right != nullptr)
					insert (elem, leaf->right);
				else
				{
					leaf->right = new BinaryNode<T>;
					leaf->right->element = elem;
					leaf->right->right = nullptr;
					leaf->right->left = nullptr;
					leaf->right->parent = leaf;
				}
			}
		}
		BinaryNode<T>* search (T elem, BinaryNode<T>* leaf)
		{
			if (leaf == nullptr)
				return nullptr;
			if (eq (leaf->element, elem))
			{
				return leaf;
			}
			if (c (elem, leaf->element))
			{
				return search (elem, leaf->left);
			}
			else
			{
				return search (elem, leaf->right);
			}
		}
		static void print_leaf (BinaryNode<T>* leaf)
		{
			std::cout << "[" << leaf->element << "]";
		}
		void in_order (BinaryNode<T>* leaf, void (*func) (BinaryNode<T>* leaf))
		{
			if (leaf != nullptr)
			{
				in_order (leaf->left, func);
				func (leaf);
				in_order (leaf->right, func);
			}
		}
		void pre_order (BinaryNode<T>* leaf, void (*func) (BinaryNode<T>* leaf))
		{
			if (leaf != nullptr)
			{
				func (leaf);
				pre_order (leaf->left, func);
				pre_order (leaf->right, func);
			}
		}
		void post_order (BinaryNode<T>* leaf, void (*func) (BinaryNode<T>* leaf))
		{
			if (leaf != nullptr)
			{
				post_order (leaf->left, func);
				post_order (leaf->right, func);
				func (leaf);
			}
		}

	public:
		BinaryTree (BinaryNode<T>* node) : _root (node) {}
		BinaryTree() : _root (nullptr) {}
		~BinaryTree()
		{
			destroy_tree();
		}
		void destroy_tree()
		{
			destroy_subtree (_root);
		}
		void insert (T elem)
		{
			if (_root == nullptr)
			{
				_root = new BinaryNode<T>;
				_root->element = elem;
				_root->left = nullptr;
				_root->right = nullptr;
				_root->parent = nullptr;
			}
			else
				insert (elem, _root);
		}
		BinaryNode<T>* search (T elem)
		{
			return search (elem, _root);
		}
		void in_order (void (*func) (BinaryNode<T>* leaf))
		{
			in_order (_root, func);
		}
		void pre_order (void (*func) (BinaryNode<T>* leaf))
		{
			pre_order (_root, func);
		}
		void post_order (void (*func) (BinaryNode<T>* leaf))
		{
			post_order (_root, func);
		}
		void print()
		{
			in_order (_root, &print_leaf);
		}
	};
}

#endif // BINARYTREE_HPP_INCLUDED
