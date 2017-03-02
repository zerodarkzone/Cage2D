#ifndef BINARYNODE_H_INCLUDED
#define BINARYNODE_H_INCLUDED

namespace b_tree
{
	template<typename T>struct BinaryNode
	{
		BinaryNode<T>* left;
		BinaryNode<T>* right;
		BinaryNode<T>* parent;
		T element;
	};
}

#endif // BINARYNODE_H_INCLUDED
