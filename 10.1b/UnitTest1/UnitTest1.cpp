#include "pch.h"
#include "CppUnitTest.h"
#include "../10b/10b.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest101
{
    TEST_CLASS(UnitTest101)
    {
    public:

        // Заповнює студента тестовими даними
        static Student Make(Specialty sp, int ph, int ma, int th)
        {
            Student st;
            st.surname = "A";
            st.course = 1;
            st.specialty = sp;
            st.physics = ph;
            st.math = ma;
            switch (sp)
            {
            case CS: st.third.programming = th; break;
            case INFORMATICS: st.third.numMethods = th; break;
            default: st.third.pedagogy = th; break;
            }
            return st;
        }

        TEST_METHOD(TestCountExcellent)
        {
            Student* s = new Student[4];
            s[0] = Make(CS, 5, 5, 5);
            s[1] = Make(INFORMATICS, 5, 5, 4);
            s[2] = Make(LABOR, 5, 5, 5);
            s[3] = Make(MATH_ECON, 4, 5, 5);
            Assert::AreEqual(2, CountExcellent(s, 4));
            delete[] s;
        }

        TEST_METHOD(TestPercentPhysics5)
        {
            Student* s = new Student[4];
            s[0] = Make(CS, 5, 3, 3);
            s[1] = Make(INFORMATICS, 4, 5, 4);
            s[2] = Make(LABOR, 5, 5, 5);
            s[3] = Make(PHYS_INF, 3, 5, 5);
            Assert::AreEqual(50.0, PercentPhysics5(s, 4), 1e-9);
            delete[] s;
        }

        TEST_METHOD(TestThirdMarkOf)
        {
            Student a = Make(INFORMATICS, 3, 3, 4);
            Student b = Make(LABOR, 3, 3, 5);
            Assert::AreEqual(4, ThirdMarkOf(a));
            Assert::AreEqual(5, ThirdMarkOf(b));
        }

        TEST_METHOD(TestIsExcellent)
        {
            Assert::IsTrue(IsExcellent(Make(CS, 5, 5, 5)));
            Assert::IsFalse(IsExcellent(Make(CS, 5, 4, 5)));
        }
    };
}