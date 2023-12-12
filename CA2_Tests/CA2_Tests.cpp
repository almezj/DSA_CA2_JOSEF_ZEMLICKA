#include "pch.h"
#include "CppUnitTest.h"
#include "../DSA_CA2_JOSEF_ZEMLICKA/Tree.h"
#include "../DSA_CA2_JOSEF_ZEMLICKA/TreeIterator.h"
#include "../DSA_CA2_JOSEF_ZEMLICKA/DList.h"
#include "../DSA_CA2_JOSEF_ZEMLICKA/DListNode.h"
#include "../DSA_CA2_JOSEF_ZEMLICKA/DListIterator.h"
#include "../DSA_CA2_JOSEF_ZEMLICKA/Main.cpp"




using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CA2Tests
{
	TEST_CLASS(CA2Tests)
	{
	public:

		TEST_METHOD(TestLoadInvalidXML)
		{
			Tree<string>* tree = loadXML<string>("invalid.xml");
			Assert::IsNull(tree);
		}

		TEST_METHOD(TestLoadValidXML)
		{
			Tree<string>* tree = loadXML<string>("C:/Users/Josef/Desktop/vs_sample.xml");
			Assert::IsNotNull(tree);
		}

		TEST_METHOD(TestFullTreeCount)
		{
			Tree<string>* tree = new Tree<string>("dir");
			tree->children->append(new Tree<string>("file"));
			tree->children->append(new Tree<string>("file"));
			tree->children->append(new Tree<string>("file"));

			Assert::AreEqual(tree->count(), 4);
		}

		TEST_METHOD(TestEmptyTreeCount)
		{
			Tree<string>* tree = nullptr;
			Assert::AreEqual(tree->count(), 0);
		}

		TEST_METHOD(TestFullTreeMemoryUsage)
		{
			Tree<string>* tree = new Tree<string>("dir");
			tree->name = "root";
			Tree<string>* t1 = new Tree<string>("file");
			t1->length = 1;
			Tree<string>* t2 = new Tree<string>("file");
			t2->length = 2;
			Tree<string>* t3 = new Tree<string>("file");
			t3->length = 3;

			tree->children->append(t1);
			tree->children->append(t2);
			tree->children->append(t3);

			Assert::AreEqual(tree->memoryUsage("root"), 6);
		}

		TEST_METHOD(TestEmptyTreeMemoryUsage) {
			Tree<string>* tree = new Tree<string>("dir");
			tree->name = "root";
			Assert::AreEqual(tree->memoryUsage("root"), 0);
		}

		TEST_METHOD(TestPruneTreeFull) {
			Tree<string>* d1 = new Tree<string>("dir");
			Tree<string>* d2 = new Tree<string>("dir");
			Tree<string>* d3 = new Tree<string>("dir");
			Tree<string>* f1 = new Tree<string>("file");
			Tree<string>* f2 = new Tree<string>("file");

			d1->children->append(f1);
			f1->parent = d1;
			d1->children->append(f2);
			f2->parent = d1;
			d1->children->append(d2);
			d2->parent = d1;
			d2->children->append(d3);
			d3->parent = d2;

			pruneTree(d1);

			Assert::AreEqual(d1->count(), 5);
		}

		TEST_METHOD(TestPruneTreeEmpty) {
			Tree<string>* d1 = new Tree<string>("dir");
			pruneTree(d1);
			Assert::AreEqual(d1->count(), 1);
		}

		TEST_METHOD(TestFindOneExisitngFileFullName) {
			Tree<string>* root = new Tree<string>("dir");
			root->name = "www";
			Tree<string>* d1 = new Tree<string>("dir");
			d1->name = "static";
			Tree<string>* d2 = new Tree<string>("dir");
			d2->name = "css";
			Tree<string>* f1 = new Tree<string>("file");
			f1->name = "style.css";
			Tree<string>* f2 = new Tree<string>("file");
			f2->name = "index.html";
			Tree<string>* f3 = new Tree<string>("file");
			f3->name = "main.js";

			root->children->append(d1);
			d1->parent = root;
			d1->children->append(d2);
			d2->parent = root;
			d2->children->append(f1);
			f1->parent = d2;
			d1->children->append(f2);
			f2->parent = d1;
			d1->children->append(f3);
			f3->parent = d1;

			std::string result = root->pathTo("style.css");
			std::string expected = "www/static/css/style.css";
			Assert::AreEqual(result, expected);

		}

		TEST_METHOD(TestFindOneExistingFilePartName) {
			Tree<string>* root = new Tree<string>("dir");
			root->name = "www";
			Tree<string>* d1 = new Tree<string>("dir");
			d1->name = "static";
			Tree<string>* d2 = new Tree<string>("dir");
			d2->name = "css";
			Tree<string>* f1 = new Tree<string>("file");
			f1->name = "style.css";
			Tree<string>* f2 = new Tree<string>("file");
			f2->name = "index.html";
			Tree<string>* f3 = new Tree<string>("file");
			f3->name = "main.js";

			root->children->append(d1);
			d1->parent = root;
			d1->children->append(d2);
			d2->parent = root;
			d2->children->append(f1);
			f1->parent = d2;
			d1->children->append(f2);
			f2->parent = d1;
			d1->children->append(f3);
			f3->parent = d1;

			std::string result = root->pathTo("sty");
			std::string expected = "www/static/css/style.css";
			Assert::AreEqual(result, expected);
		}

		TEST_METHOD(TestFindMultipleExistingFilesFullName) {
			Tree<string>* root = new Tree<string>("dir");
			root->name = "www";
			Tree<string>* d1 = new Tree<string>("dir");
			d1->name = "static";
			Tree<string>* d2 = new Tree<string>("dir");
			d2->name = "css";
			Tree<string>* f1 = new Tree<string>("file");
			f1->name = "custom_style.css";
			Tree<string>* f2 = new Tree<string>("file");
			f2->name = "style.css";
			Tree<string>* f3 = new Tree<string>("file");
			f3->name = "index.html";
			Tree<string>* f4 = new Tree<string>("file");
			f4->name = "main.js";

			root->children->append(d1);
			d1->parent = root;
			d1->children->append(d2);
			d2->parent = root;
			d2->children->append(f1);
			f1->parent = d2;
			d1->children->append(f2);
			f2->parent = d1;
			d1->children->append(f3);
			f3->parent = d1;
			d1->children->append(f4);
			f4->parent = d1;

			std::string result = root->pathTo("custom_style.css");
			std::string expected = "www/static/css/custom_style.css";
			Assert::AreEqual(result, expected);
		}

		TEST_METHOD(TestFindMultipleExistingFilesPartName) {
			Tree<string>* root = new Tree<string>("dir");
			root->name = "www";
			Tree<string>* d1 = new Tree<string>("dir");
			d1->name = "static";
			Tree<string>* d2 = new Tree<string>("dir");
			d2->name = "css";
			Tree<string>* f1 = new Tree<string>("file");
			f1->name = "custom_style.css";
			Tree<string>* f2 = new Tree<string>("file");
			f2->name = "style.css";
			Tree<string>* f3 = new Tree<string>("file");
			f3->name = "index.html";
			Tree<string>* f4 = new Tree<string>("file");
			f4->name = "main.js";

			root->children->append(d1);
			d1->parent = root;
			d1->children->append(d2);
			d2->parent = root;
			d2->children->append(f1);
			f1->parent = d2;
			d1->children->append(f2);
			f2->parent = d1;
			d1->children->append(f3);
			f3->parent = d1;
			d1->children->append(f4);
			f4->parent = d1;

			std::string result = root->pathTo("styl");
			std::string expected = "www/static/css/custom_style.css";
			Assert::AreEqual(result, expected);
		}

		TEST_METHOD(TestFindNonExistingFile) {
			Tree<string>* root = new Tree<string>("dir");
			root->name = "www";
			Tree<string>* d1 = new Tree<string>("dir");
			d1->name = "static";
			Tree<string>* d2 = new Tree<string>("dir");
			d2->name = "css";
			Tree<string>* f1 = new Tree<string>("file");
			f1->name = "custom_style.css";
			Tree<string>* f2 = new Tree<string>("file");
			f2->name = "style.css";
			Tree<string>* f3 = new Tree<string>("file");
			f3->name = "index.html";
			Tree<string>* f4 = new Tree<string>("file");
			f4->name = "main.js";

			root->children->append(d1);
			d1->parent = root;
			d1->children->append(d2);
			d2->parent = root;
			d2->children->append(f1);
			f1->parent = d2;
			d1->children->append(f2);
			f2->parent = d1;
			d1->children->append(f3);
			f3->parent = d1;
			d1->children->append(f4);
			f4->parent = d1;

			std::string result = root->pathTo("style.csss");
			std::string expected = "";
			Assert::AreEqual(result, expected);
		}

		TEST_METHOD(TestFindExistingFolderFullName) {
			Tree<string>* root = new Tree<string>("dir");
			root->name = "www";
			Tree<string>* d1 = new Tree<string>("dir");
			d1->name = "static";
			Tree<string>* d2 = new Tree<string>("dir");
			d2->name = "css";
			Tree<string>* f1 = new Tree<string>("file");
			f1->name = "custom_style.css";
			Tree<string>* f2 = new Tree<string>("file");
			f2->name = "style.css";
			Tree<string>* f3 = new Tree<string>("file");
			f3->name = "index.html";
			Tree<string>* f4 = new Tree<string>("file");
			f4->name = "main.js";

			root->children->append(d1);
			d1->parent = root;
			d1->children->append(d2);
			d2->parent = root;
			d2->children->append(f1);
			f1->parent = d2;
			d1->children->append(f2);
			f2->parent = d1;
			d1->children->append(f3);
			f3->parent = d1;
			d1->children->append(f4);
			f4->parent = d1;

			std::string result = root->pathTo("css");
			std::string expected = "www/static/css";
			Assert::AreEqual(result, expected);
		}

		TEST_METHOD(TestFindExistingFolderPartName) {
			Tree<string>* root = new Tree<string>("dir");
			root->name = "www";
			Tree<string>* d1 = new Tree<string>("dir");
			d1->name = "static";
			Tree<string>* d2 = new Tree<string>("dir");
			d2->name = "css";
			Tree<string>* f1 = new Tree<string>("file");
			f1->name = "custom_style.css";
			Tree<string>* f2 = new Tree<string>("file");
			f2->name = "style.css";
			Tree<string>* f3 = new Tree<string>("file");
			f3->name = "index.html";
			Tree<string>* f4 = new Tree<string>("file");
			f4->name = "main.js";

			root->children->append(d1);
			d1->parent = root;
			d1->children->append(d2);
			d2->parent = root;
			d2->children->append(f1);
			f1->parent = d2;
			d1->children->append(f2);
			f2->parent = d1;
			d1->children->append(f3);
			f3->parent = d1;
			d1->children->append(f4);
			f4->parent = d1;

			std::string result = root->pathTo("cs");
			std::string expected = "www/static/css";
			Assert::AreEqual(result, expected);
		}

		TEST_METHOD(TestFindNonExistingFolder) {
			Tree<string>* root = new Tree<string>("dir");
			root->name = "www";
			Tree<string>* d1 = new Tree<string>("dir");
			d1->name = "static";
			Tree<string>* d2 = new Tree<string>("dir");
			d2->name = "css";
			Tree<string>* f1 = new Tree<string>("file");
			f1->name = "custom_style.css";
			Tree<string>* f2 = new Tree<string>("file");
			f2->name = "style.css";
			Tree<string>* f3 = new Tree<string>("file");
			f3->name = "index.html";
			Tree<string>* f4 = new Tree<string>("file");
			f4->name = "main.js";

			root->children->append(d1);
			d1->parent = root;
			d1->children->append(d2);
			d2->parent = root;
			d2->children->append(f1);
			f1->parent = d2;
			d1->children->append(f2);
			f2->parent = d1;
			d1->children->append(f3);
			f3->parent = d1;
			d1->children->append(f4);
			f4->parent = d1;

			std::string result = root->pathTo("thisdoesnotexist");
			std::string expected = "";
			Assert::AreEqual(result, expected);
		}

		TEST_METHOD(TestFindEmpty) {
			Tree<string>* root = new Tree<string>("dir");
			root->name = "www";
			Tree<string>* d1 = new Tree<string>("dir");
			d1->name = "static";
			Tree<string>* d2 = new Tree<string>("dir");
			d2->name = "css";
			Tree<string>* f1 = new Tree<string>("file");
			f1->name = "custom_style.css";
			Tree<string>* f2 = new Tree<string>("file");
			f2->name = "style.css";
			Tree<string>* f3 = new Tree<string>("file");
			f3->name = "index.html";
			Tree<string>* f4 = new Tree<string>("file");
			f4->name = "main.js";

			root->children->append(d1);
			d1->parent = root;
			d1->children->append(d2);
			d2->parent = root;
			d2->children->append(f1);
			f1->parent = d2;
			d1->children->append(f2);
			f2->parent = d1;
			d1->children->append(f3);
			f3->parent = d1;
			d1->children->append(f4);
			f4->parent = d1;

			std::string result = root->pathTo("");
			std::string expected = "";
			Assert::AreEqual(result, expected);
		}


	};
}
