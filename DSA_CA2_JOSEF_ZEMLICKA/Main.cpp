#include "Tree.h"
#include "TreeIterator.h"
#include <string>
#include <iostream>
#include <queue>
#include <sstream>
#include <fstream>
#include <stack>

using namespace std;

template<typename T>
Tree<T>* loadXML(const string& xmlFilename) {
    cout << "loadXML started" << endl;

    ifstream file(xmlFilename);
    if (file.is_open()) {
        cout << "File is valid" << endl;

        Tree<T>* root = nullptr;
        Tree<T>* currentNode = nullptr;
        stack<Tree<T>*> nodeStack;
        string line;

        while (getline(file, line)) {
            size_t pos = 0;
            while ((pos = line.find('<', pos)) != string::npos) {
                size_t endPos = line.find('>', pos);
                if (endPos == string::npos) {
                    cerr << "Error: Invalid XML tag in line: " << line << endl;
                    return nullptr;
                }

                string tag = line.substr(pos + 1, endPos - pos - 1);

                // If the tag is not a closing tag
                if (tag[0] != '/') {
                    // If the tag is either a dir or file, create a new node and push it to the nodeStack
                    if (tag == "dir" || tag == "file") {
                        Tree<T>* newNode = new Tree<T>(tag);
                        newNode->parent = currentNode;

                        if (currentNode) {
                            currentNode->children->append(newNode);
                        }
                        else {
                            root = newNode;
                        }
                        if (tag == "dir") {
                            newNode->type = "dir";
                        }
                        nodeStack.push(newNode);
                        currentNode = newNode;
                        /*currentNode->children->printThis();*/
                    }
                    // If the tag is an attribute, add it to the currentNode as an attribute (name, type, length)
                    else {
                        if (currentNode) {
                            // Get the inner value of the tag
                            string value = line.substr(endPos + 1, line.find('<', endPos) - endPos - 1);

                            if (tag == "name") {
                                currentNode->name = value;
                            }
                            else if (tag == "length") {
                                // Save only the number part of the length
                                size_t spacePos = value.find(' ');
                                if (spacePos != string::npos) {
                                    value = value.substr(0, spacePos);
                                }
                                // Convert the length to an integer
                                int lengthValue = atoi(value.c_str());
                                currentNode->length = lengthValue;
                            }
                            else if (tag == "type") {
                                currentNode->type = value;
                            }
                        }
                    }
                }
                // Else the tag is a closing tag
                else {
                    if (!nodeStack.empty()) {
                        // Get the currentNode(closing tag) and the last opening tag from nodeStack
                        string closingTag = tag.substr(1);
                        currentNode = nodeStack.top();

                        // If the current closing tag is not dir or file, skip this iteration
                        if (closingTag == "dir" || closingTag == "file") {
                            if (closingTag != currentNode->getData()) {
                                cerr << "Error: Mismatched closing tag in line: " << line << endl;
                                return nullptr;
                            }
                            nodeStack.pop();
                            currentNode = currentNode->parent; // Move up the tree
                        }
                    }
                }
                pos = endPos + 1;
            }
        }
        return root;
    }
    else {
        cerr << "Error opening XML file: " << xmlFilename << endl;
        return nullptr;
    }
}


template <typename T>
void printTree(Tree<T>* node, int depth = 0) {
    if (node) {
        for (int i = 0; i < depth; ++i) cout << "  ";
        if (node->type == "dir")
            cout << node->getData() << " | name: " << node->name << endl;
        else {
             cout << node->getData() << " | name: " << node->name << " | size: " << node->length << endl;
        }
		DListIterator<Tree<T>*> iter = node->children->getIterator();
		while (iter.isValid()) {
			printTree(iter.item(), depth + 1);
			iter.advance();
		}
	}

}

template<class T>
void pruneTree(Tree<T>*& node)
{
    DListIterator<Tree<T>*> iter = node->children->getIterator();

    while (iter.isValid())
    {
        Tree<T>* child = iter.item();
        iter.advance();
        pruneTree(child);
    }

    if (node->children->isEmpty() && node->type == "dir")
    {
        if (node->parent != nullptr)
        {
            DListIterator<Tree<T>*> iter = node->parent->children->getIterator();
            while (iter.isValid() && iter.currentNode->data != node) {
                iter.advance();
            }
            if (iter.isValid())  // Check if the node was found in the parent's children
            {
                node->parent->children->remove(iter);
                delete node;
                node = nullptr;
            }
            // If the node is not found in the parent's children, it might have been already removed by a previous recursion level.
        }
        else
        {
            delete node;
            node = nullptr;
        }
    }
}

int main() {
	cout << "Started" << endl;

	// Path to XML file
	const string xmlFilename = "input/vs_sample.xml";

	// Load XML file into a type that can be used by the parse_xml function
	Tree<string>* xmlTree = loadXML<string>(xmlFilename);

	if (xmlTree) {
		cout << "XML Tree Structure:" << endl;
		printTree(xmlTree);
        // Wait for the tree to be printed
        cout << endl;

        cout << "XML Tree node count: " << xmlTree->count() << endl;
        cout << endl;
        cout << "Memory used by folder 'Debug': " << xmlTree->memoryUsage("Debug") << endl;
        cout << endl;
        pruneTree(xmlTree);
        cout << "Pruned XML Tree Info:" << endl;
        printTree(xmlTree);
        cout << "XML Tree node count: " << xmlTree->count() << endl;
        cout << endl;

        cout <<"Path to a file with name 'TestSListNode.obj': " << xmlTree->pathTo("TestSListNode.obj") << endl;

        cout << endl;
        cout << "Displaying contents of the 'objects' directory: " << endl;
        xmlTree->displayContents("objects");
	}
	else {
		cout << "Failed to load XML tree." << endl;
	}

	// Release
	delete xmlTree;

	return 0;
}