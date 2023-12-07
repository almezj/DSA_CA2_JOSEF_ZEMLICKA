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
		cout << "File exists" << endl;

		// Create: 
		// - root node -> root of the tree
		// - currentNode -> node that is currently being manipulated
		// - nodeStack -> used for validation of the XML file
		// - line -> current line of xml file
		Tree<T>* root = nullptr;
		Tree<T>* currentNode = nullptr;
		stack<Tree<T>*> nodeStack;
		stack<std::string> attributeStack;
		std::string lastAttribute;
		string line;
		string tagValue;

		// While line exists
		while (getline(file, line)) {
			size_t pos = 0;

			while ((pos = line.find('<', pos)) != string::npos) {
				size_t endPos = line.find('>', pos);

				if (endPos == string::npos) {
					cerr << "1Error: Invalid XML tag in line: " << line << endl;
					return nullptr;
				}

				string tag = line.substr(pos + 1, endPos - pos - 1);

				if (tag[0] == '/') {
					// Check if the closing tag is the same as the last opening tag
					if (tag == lastAttribute) {
						// Skip this iteration
						pos = endPos + 1;
						continue;
					}
					if (!nodeStack.empty()) {
						currentNode = nodeStack.top();
						nodeStack.pop();
					}
				}
				else {
					if (tag == "dir" || tag == "file") {
						Tree<T>* newNode = new Tree<T>(tag);

						if (currentNode) {
							currentNode->children->append(newNode);
							nodeStack.push(newNode);
						}
						else {
							root = newNode;
						}
						currentNode = newNode;
					}
					else {
						attributeStack.push(tag);

						size_t valueStartPos = line.find('>', pos);
						size_t valueEndPos = line.find('<', valueStartPos);

						// Extract tag value
						if (valueStartPos != string::npos && valueEndPos != string::npos) {
							tagValue = line.substr(valueStartPos + 1, valueEndPos - valueStartPos - 1);
						}
							

						// Set tag value for specific tags
						if (!tagValue.empty()) {
							if (tag == "name") {
								currentNode->name = tagValue;
							}
							else if (tag == "length") {
								// Save only the number part of the length
								size_t spacePos = tagValue.find(' ');
								if (spacePos != string::npos) {
									tagValue = tagValue.substr(0, spacePos);
								}
								// Convert the length to an integer
								int lengthValue = atoi(tagValue.c_str());
								currentNode->length = lengthValue;
							}
							else if (tag == "type") {
								currentNode->type = tagValue;
							}
						}
						lastAttribute = tag;
					}

				}

				pos = endPos + 1;
			}
		}

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
		cout << node->getData();
		if (!node->name.empty()) {
			cout << ": " << node->name;
		}
		if (node->length > 0) {
			cout << " (" << node->length << ")";
		}
		if (!node->type.empty())
		{
			cout << " [" << node->type << "]";
		}
		cout << endl;
		DListIterator<Tree<T>*> iter = node->children->getIterator();
		while (iter.isValid()) {
			printTree(iter.item(), depth + 1);
			iter.advance();
		}
	}
	// FIXME: Releasing iterator does not work for some reason
}

template <class T>
string findFileDFS(Tree<T>* node, const std::string& target) {
	// Find a given file/folder given a partial or complete filename (no path). Generate the path for the given file / folder(Depth first Search)
	// If the file/folder is not found, return an empty string
	// If the file/folder is found, return the path to the file/folder
	// If there are multiple files/folders with the same name, return the first one found
	string path = "";
	
}


int main_2() {
	cout << "Started" << endl;

	// Path to XML file
	const string xmlFilename = "C:/Users/Josef/Desktop/vs_sample_simple.xml";
	Tree<string>* xmlTree = loadXML<string>(xmlFilename);

	if (xmlTree) {
		cout << "XML Tree Structure:" << endl;
		printTree(xmlTree);
	}
	else {
		cout << "Failed to load XML tree." << endl;
	}

	//cout << "Memory used: " << xmlTree->memoryUsage(xmlTree) << " b" << endl;
	//cout << "Count: " << xmlTree->count() << endl;

	// Release
	delete xmlTree;

	return 0;
}