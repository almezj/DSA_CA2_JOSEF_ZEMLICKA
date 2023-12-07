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
			Tree<string>* tree = loadXML<string>("C:/Users/Josef/Desktop/invalid.xml");
			Assert::IsNull(tree);
		}

		TEST_METHOD(TestLoadValidXML)
		{
			Tree<string>* tree = loadXML<string>("C:/Users/Josef/Desktop/vs_sample_simple.xml");
			Assert::IsNotNull(tree);
		}
	};
}
