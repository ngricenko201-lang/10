#include "pch.h"
#include "CppUnitTest.h"
#include "../10a/10a.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestLab101
{
	TEST_CLASS(UnitTestLab101)
	{
	public:

		TEST_METHOD(TestIsExcellent)
		{
			Student a = { "Ivanov", 1, COMPUTER_SCIENCE, 5, 5, 5 };
			Student b = { "Petrov", 2, INFORMATICS, 5, 4, 5 };
			Assert::IsTrue(IsExcellent(a));
			Assert::IsFalse(IsExcellent(b));
		}

		TEST_METHOD(TestCountExcellent)
		{
			const int N = 4;
			Student* s = new Student[N];
			s[0] = { "A", 1, COMPUTER_SCIENCE, 5, 5, 5 };
			s[1] = { "B", 2, INFORMATICS, 5, 4, 5 };
			s[2] = { "C", 3, LABOR_TRAINING, 5, 5, 5 };
			s[3] = { "D", 1, MATH_ECONOMICS, 3, 5, 5 };
			Assert::AreEqual(2, CountExcellent(s, N));
			delete[] s;
		}

		TEST_METHOD(TestCountExcellentNone)
		{
			const int N = 2;
			Student* s = new Student[N];
			s[0] = { "A", 1, COMPUTER_SCIENCE, 4, 5, 5 };
			s[1] = { "B", 2, INFORMATICS, 3, 3, 3 };
			Assert::AreEqual(0, CountExcellent(s, N));
			delete[] s;
		}

		TEST_METHOD(TestPercentPhysics5)
		{
			const int N = 4;
			Student* s = new Student[N];
			s[0] = { "A", 1, COMPUTER_SCIENCE, 5, 3, 4 };
			s[1] = { "B", 2, INFORMATICS, 4, 4, 4 };
			s[2] = { "C", 3, LABOR_TRAINING, 5, 5, 5 };
			s[3] = { "D", 1, MATH_ECONOMICS, 3, 5, 5 };
			Assert::AreEqual(50.0, PercentPhysics5(s, N), 1e-9);
			delete[] s;
		}

		TEST_METHOD(TestPercentPhysics5Empty)
		{
			Assert::AreEqual(0.0, PercentPhysics5(nullptr, 0), 1e-9);
		}
	};
}