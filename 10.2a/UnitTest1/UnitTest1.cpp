#include "pch.h"
#include "CppUnitTest.h"
#include "../10.2a/10.2a.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestLab102
{
	// Допоміжна функція створення студента
	static Student Make(const char* name, int course, int p, int m, int i)
	{
		Student st;
		st.surname = name;
		st.course = course;
		st.specialty = COMPUTER_SCIENCE;
		st.physics = p;
		st.math = m;
		st.informatics = i;
		return st;
	}

	TEST_CLASS(UnitTestLab102)
	{
	public:

		TEST_METHOD(TestSumAndAverage)
		{
			Student st = Make("Ivanov", 1, 5, 4, 3);
			Assert::AreEqual(12, Sum(st));
			Assert::AreEqual(4.0, Average(st), 1e-9);
		}

		TEST_METHOD(TestSortPhysical)
		{
			Student s[4] = {
				Make("Petrov", 2, 5, 5, 5),
				Make("Ivanov", 1, 4, 4, 4),
				Make("Antonov", 1, 3, 3, 3),
				Make("Zinchenko", 1, 4, 4, 4)
			};
			SortPhysical(s, 4);
			// курс 1: середній 3 -> потім 4 (прізвища за спаданням) -> курс 2
			Assert::AreEqual(string("Antonov"), s[0].surname);
			Assert::AreEqual(string("Zinchenko"), s[1].surname);
			Assert::AreEqual(string("Ivanov"), s[2].surname);
			Assert::AreEqual(string("Petrov"), s[3].surname);
		}

		TEST_METHOD(TestBuildIndex)
		{
			Student s[4] = {
				Make("Petrov", 2, 5, 5, 5),
				Make("Ivanov", 1, 4, 4, 4),
				Make("Antonov", 1, 3, 3, 3),
				Make("Zinchenko", 1, 4, 4, 4)
			};
			int idx[4];
			BuildIndex(s, idx, 4);
			Assert::AreEqual(2, idx[0]);
			Assert::AreEqual(1, idx[1]);
			Assert::AreEqual(3, idx[2]);
			Assert::AreEqual(0, idx[3]);
			// сам масив не змінився
			Assert::AreEqual(string("Petrov"), s[0].surname);
		}

		TEST_METHOD(TestBinarySearchFound)
		{
			Student s[4] = {
				Make("Petrov", 2, 5, 5, 5),
				Make("Ivanov", 1, 4, 4, 4),
				Make("Antonov", 1, 3, 3, 3),
				Make("Zinchenko", 1, 4, 4, 4)
			};
			int idx[4];
			BuildIndex(s, idx, 4);
			int pos = BinarySearch(s, idx, 4, "Ivanov", 1, 4.0);
			Assert::AreEqual(1, pos);
			Assert::AreEqual(1, idx[pos]);
		}

		TEST_METHOD(TestBinarySearchNotFound)
		{
			Student s[3] = {
				Make("Petrov", 2, 5, 5, 5),
				Make("Ivanov", 1, 4, 4, 4),
				Make("Antonov", 1, 3, 3, 3)
			};
			int idx[3];
			BuildIndex(s, idx, 3);
			Assert::AreEqual(-1, BinarySearch(s, idx, 3, "Ivanov", 2, 4.0));
			Assert::AreEqual(-1, BinarySearch(s, idx, 3, "Sydorov", 1, 4.0));
			Assert::IsTrue(BinarySearch(s, idx, 3, "Petrov", 2, 5.0) >= 0);
		}
	};
}