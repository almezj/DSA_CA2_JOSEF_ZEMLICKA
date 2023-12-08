#pragma once
#include "DList.h"
#include "DListIterator.h"
#include <string>
#include <queue>
#include <iostream>

using namespace std;
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
	string pathTo(string name);
	void displayContents(string folderName);
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
	DListIterator<Tree<T>*> iter(node->children);

	cout << "Children size: " << node->children->size() << endl;

	if (!iter.isValid()) {
		cout << "No children" << endl;
		return;
	}

	while (iter.isValid())
	{
		Tree<T>* child = iter.item();
		
		cout << "Processing child: " << child->name << endl;

		if (child->children->isEmpty() && child->getData() == "dir")
		{
			cout << "Removing " << child->name << endl;
			iter = node->children->remove(iter);
		}
		else
		{
			cout << "Pruning " << child->name << endl;
			pruneTree(child);
			if (iter.isValid()) {
				iter.advance();
			}
		}
	}
}
template <class T>
string Tree<T>::pathTo(string name)
{
	if(name == ""){
		return "";
	}

	string path = "";
	bool found = false;
	if (this->name.find(name) != string::npos)
	{
		path = this->name;
		return path;
	}
	else
	{
		DListIterator<Tree<T>*> iter = children->getIterator();
		while (iter.isValid())
		{
			path = iter.item()->pathTo(name);
			if (path != "")
			{
				path = this->name + "/" + path;
				return path;
			}
			iter.advance();
		}
	}
	return path;
};

template<class T>
void Tree<T>::displayContents(string folderName)
{
	if (folderName == "")
	{
		return;
	}
	else
	{
		if (this->name.find(folderName) != string::npos)
		{
			cout << this->name << endl;
		}
	}
	DListIterator<Tree<T>*> iter = children->getIterator();
	while (iter.isValid())
	{
		if (iter.item()->type == "dir")
		{
			cout << iter.item()->name << endl;
		}
		else if (iter.item()->type == "file")
		{
			cout << iter.item()->name << " " << iter.item()->length << endl;
		}
		iter.item()->displayContents(folderName);
		iter.advance();
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

