#include "Tree.h"
#include "TreeIterator.h"
#include <string>
#include <iostream>
#include <queue>
#include <fstream>
#include <stack>

using namespace std;

// BFS Printing function copied from Trees.cpp to help visualizing BFS algorithm
//void printBFS(Tree<string> tree)
//{
//	queue<Tree<string>> queue;
//	queue.push(tree);
//	while (!queue.empty())
//	{
//		DListIterator<Tree<string>*> iter = queue.front().children->getIterator();
//		while (iter.isValid())
//		{
//			queue.push(*iter.item());
//			iter.advance();
//		}
//		cout << queue.front().data << ", ";
//		queue.pop();
//
//	}
//
//}

template<typename T>
Tree<T>* loadXML(const string& xmlFilename) {
	cout << "loadXML started" << endl;

	// Create the filestream from filename argument
	ifstream file(xmlFilename);

	// If the file can be opened
	if (file.is_open()) {
		cout << "File is valid" << endl;

		// Create: 
		// - root node -> root of the tree
		// - currentNode -> node that is currently being manipulated
		// - nodeStack -> used for validation of the XML file
		// - line -> current line of xml file
		Tree<T>* root = nullptr;
		Tree<T>* currentNode = nullptr;
		stack<Tree<T>*> nodeStack;
		string line;

		// While line exists
		while (getline(file, line)) {
			cout << "newline" << endl;

			// Index of the char of current line (size_t = unsigned integer type used to represent size or indeces of objects in memory)
			size_t pos = 0;

			// While the '<' symbol's position is NOT npos (npos = value commonly used to indicate the end of string or not found in substrings)
			while ((pos = line.find('<', pos)) != string::npos) {

				// Find the index of '>' character
				size_t endPos = line.find('>', pos);

				// If the index is not found, xml tag is not valid
				if (endPos == string::npos) {
					cerr << "Error: Invalid XML tag in line: " << line << endl;
					return nullptr;
				}

				// Tag valiadated -> store it in a string variable
				string tag = line.substr(pos + 1, endPos - pos - 1);


				// If the tag is a closing tag
				if (tag[0] == '/') {
					// If nodeStack is NOT empty
					if (!nodeStack.empty()) {
						// Pop the most recent tag from nodeStack
						currentNode = nodeStack.top();
						nodeStack.pop();
					}
				}
				// Else (The current tag is an opening tag)
				else {
					Tree<T>* newNode = new Tree<T>(tag);

					// New node parent is currentNode (nullptr or the currentNode)
					newNode->parent = currentNode;

					// If currentNode exists, new node gets added to its children nodes and to the nudeStack
					if (currentNode) {
						currentNode->children->append(newNode);
						nodeStack.push(currentNode);
					}
					// currentNode is nullptr -> new node is the new root
					else {
						root = newNode;
					}
					currentNode = newNode;
				}

				// Move the index to last index + 1 -> that should be '<' of the new tag
				pos = endPos + 1;
			}
		}

		// Return the Tree's root
		return root;
	}
	// File cannot be opened, throw error
	else {
		cerr << "Error opening XML file: " << xmlFilename << endl;
	}

	return nullptr;
}

template <typename T>
void printTree(Tree<T>* node, int depth = 0) {
	if (node) {
		for (int i = 0; i < depth; ++i) cout << "  ";
		cout << node->getData() << endl;
		DListIterator<Tree<T>*> iter = node->children->getIterator();
		while (iter.isValid()) {
			printTree(iter.item(), depth + 1);
			iter.advance();
		}
	}
	
}

int main() {
	cout << "Started" << endl;
	const string xmlFilename = "C:/Users/Josef/Desktop/vs_sample_simple.xml";
	Tree<string>* xmlTree = loadXML<string>(xmlFilename);

	if (xmlTree) {
		cout << "XML Tree Structure:" << endl;
		printTree(xmlTree);
	}
	else {
		cout << "Failed to load XML tree." << endl;
	}

	// Release
	delete xmlTree;

	return 0;
}