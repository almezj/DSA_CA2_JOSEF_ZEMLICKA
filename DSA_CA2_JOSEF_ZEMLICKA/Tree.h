#pragma once
#include "DList.h"
#include "DListIterator.h"
//#include "Main.h"
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
	int memoryUsage(string folderName);
	string pathTo(string name);
	void displayContents(string folderName, bool print = false);
};
template <class T>
T Tree<T>::getData()
{
	return data;
}

template<class T>
int Tree<T>::memoryUsage(string folderName)
{
	if (folderName == "") {
		return 0;
	}

	int sum = 0;
	Tree<T>* correctFolder = nullptr;

	// Queue for BFS
	std::queue<Tree<T>*> bfsQueue;
	bfsQueue.push(this);

	while (!bfsQueue.empty()) {
		if(correctFolder != nullptr){
			DListIterator<Tree<T>*> iter2 = correctFolder->children->getIterator();
			while (iter2.isValid()) {
				sum += iter2.item()->length;
				iter2.advance();
			}
			return sum;
		}


		Tree<T>* current = bfsQueue.front();
		bfsQueue.pop();

		// Enqueue child nodes for further exploration
		DListIterator<Tree<T>*> iter = current->children->getIterator();
		while (iter.isValid()) {
			bfsQueue.push(iter.item());
			iter.advance();
		}

		if(current->name.find(folderName) != string::npos){
			correctFolder = current;
		}
	}

	return 0;
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
void Tree<T>::displayContents(string folderName, bool print) {
	if (folderName == "") {
		return;
	}
	else {
		if (this->name.find(folderName) != string::npos) {
			print = true;
		}
	}

	if (print) {
		printTree(this);
		return;
	}

	DListIterator<Tree<T>*> iter = children->getIterator();
	while (iter.isValid()) {
		iter.item()->displayContents(folderName, print);
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
	if(this == nullptr) {
		return 0;
	}

	int c = 1;
	DListIterator<Tree<T>*> iter = children->getIterator();
	while (iter.isValid())
	{
		c += iter.item()->count();
		iter.advance();
	}
	return c;
}

