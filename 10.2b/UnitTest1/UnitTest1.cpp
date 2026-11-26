#include "pch.h"
#include "CppUnitTest.h"
#include "../10.2b/10.2b.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestLab102
{
	// Допоміжна функція створення студента
	Student Make(const string& surname, int course, Specialty spec, int ph, int m, int third)
	{
		Student s;
		s.surname = surname;
		s.course = course;
		s.spec = spec;
		s.physics = ph;
		s.math = m;
		SetThirdGrade(s, third);
		return s;
	}

	TEST_CLASS(UnitTestLab102)
	{
	public:

		TEST_METHOD(TestGetThirdGrade)
		{
			Student s = Make("A", 1, INFORMATICS, 5, 4, 3);
			Assert::AreEqual(3, GetThirdGrade(s));
			Assert::AreEqual(3, s.third.numerical);
		}

		TEST_METHOD(TestGetAvgKey)
		{
			Student s = Make("A", 1, COMPUTER_SCIENCE, 5, 4, 4);
			Assert::AreEqual(433, GetAvgKey(s));
		}

		TEST_METHOD(TestComparePhysical)
		{
			Student a = Make("Ivanov", 1, LABOR_TRAINING, 5, 5, 5);
			Student b = Make("Petrov", 1, LABOR_TRAINING, 5, 5, 5);
			// за спаданням прізвищ: Petrov має бути перед Ivanov
			Assert::IsTrue(ComparePhysical(b, a) < 0);
			Assert::IsTrue(CompareIndexed(a, b) < 0);
		}

		TEST_METHOD(TestPhysicalSort)
		{
			Student s[4];
			s[0] = Make("Ivanov", 2, COMPUTER_SCIENCE, 5, 5, 5);
			s[1] = Make("Petrov", 1, LABOR_TRAINING, 4, 4, 4);
			s[2] = Make("Andriv", 1, LABOR_TRAINING, 4, 4, 4);
			s[3] = Make("Sydor", 1, INFORMATICS, 3, 3, 3);
			PhysicalSort(s, 4);
			Assert::AreEqual(string("Sydor"), s[0].surname);
			Assert::AreEqual(string("Petrov"), s[1].surname);
			Assert::AreEqual(string("Andriv"), s[2].surname);
			Assert::AreEqual(string("Ivanov"), s[3].surname);
		}

		TEST_METHOD(TestBuildIndexArray)
		{
			Student s[4];
			s[0] = Make("Ivanov", 2, COMPUTER_SCIENCE, 5, 5, 5);
			s[1] = Make("Petrov", 1, LABOR_TRAINING, 4, 4, 4);
			s[2] = Make("Andriv", 1, LABOR_TRAINING, 4, 4, 4);
			s[3] = Make("Sydor", 1, INFORMATICS, 3, 3, 3);
			int* idx = BuildIndexArray(s, 4);
			Assert::AreEqual(3, idx[0]);
			Assert::AreEqual(2, idx[1]);
			Assert::AreEqual(1, idx[2]);
			Assert::AreEqual(0, idx[3]);
			// вихідний масив не змінився
			Assert::AreEqual(string("Ivanov"), s[0].surname);
			delete[] idx;
		}

		TEST_METHOD(TestBinarySearch)
		{
			Student s[4];
			s[0] = Make("Ivanov", 2, COMPUTER_SCIENCE, 5, 5, 5);
			s[1] = Make("Petrov", 1, LABOR_TRAINING, 4, 4, 4);
			s[2] = Make("Andriv", 1, LABOR_TRAINING, 4, 4, 4);
			s[3] = Make("Sydor", 1, INFORMATICS, 3, 3, 3);
			int* idx = BuildIndexArray(s, 4);
			Assert::AreEqual(1, BinarySearch(s, idx, 4, "Andriv", 1, 400));
			Assert::AreEqual(3, BinarySearch(s, idx, 4, "Ivanov", 2, 500));
			Assert::AreEqual(-1, BinarySearch(s, idx, 4, "Andriv", 2, 400));
			delete[] idx;
		}
	};
}