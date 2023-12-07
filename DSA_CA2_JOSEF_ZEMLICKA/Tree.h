#pragma once
#include "DList.h"
#include <string>
#include <queue>

template<class T>
class Tree
{
public:
	T data;
	std::string name;
	std::string type;
	int length;
	Tree<T>* parent;
	DList<Tree<T>*>* children;
	Tree(T item);
	int count();
	T getData();
	int memoryUsage(Tree<T>* node);
	void pruneTree(Tree<T>* node);
};
template <class T>
T Tree<T>::getData()
{
	return data;
}

template<class T>
int Tree<T>::memoryUsage(Tree<T>* node)
{
	int sum = 0;

	// Queue for BFS
	std::queue<Tree<T>*> bfsQueue;
	bfsQueue.push(node);

	while (!bfsQueue.empty()) {
		Tree<T>* current = bfsQueue.front();
		bfsQueue.pop();

		// Add the length of the current node if it has the "length" attribute
		sum += current->length;

		// Enqueue child nodes for further exploration
		DListIterator<Tree<T>*> iter = current->children->getIterator();
		while (iter.isValid()) {
			bfsQueue.push(iter.item());
			iter.advance();
		}
	}

	return sum;
}

template<class T>
void Tree<T>::pruneTree(Tree<T>* node)
{
	if (node->children->isEmpty())
	{
		if (node->parent != nullptr)
		{
			node->parent->children->remove(node);
		}
	}
	else
	{
		DListIterator<Tree<T>*> iter = node->children->getIterator();
		while (iter.isValid())
		{
			pruneTree(iter.item());
			iter.advance();
		}
	}
}

template <class T>
Tree<T>::Tree(T item)
{
	data = item;
	children = new DList<Tree<T>*>();
	parent = nullptr;
}

template <class T>
int Tree<T>::count()
{
	int c = 1;
	DListIterator<Tree<T>*> iter = children->getIterator();
	while (iter.isValid())
	{
		c += iter.item()->count();
		iter.advance();
	}
	return c;
}

