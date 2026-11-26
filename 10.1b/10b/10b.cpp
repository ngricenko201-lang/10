// Lab 10.1
// Послідовний пошук в масиві структур
// Варіант 4
#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
using namespace std;

// Спеціальності
enum Specialty { CS = 0, INFORMATICS, MATH_ECON, PHYS_INF, LABOR };
const int SPECIALTY_COUNT = 5;

// Третя оцінка залежить від спеціальності
union ThirdMark
{
    int programming;   // Комп'ютерні науки
    int numMethods;    // Інформатика
    int pedagogy;      // інші спеціальності
};

struct Student
{
    string surname;
    int course;
    Specialty specialty;
    int physics;
    int math;
    ThirdMark third;
};

// Прототипи    
string SpecialtyName(Specialty s);
int ReadInt(const string& prompt, int minV, int maxV, bool& ok);
bool ReadSurname(const string& prompt, string& s);
bool ReadStudent(Student& st, int index);
Student* CreateStudents(int N);
bool FillStudents(Student* s, int N);
void PrintTable(const Student* s, int N);
bool IsExcellent(const Student& st);
int ThirdMarkOf(const Student& st);
int CountExcellent(const Student* s, int N);
double PercentPhysics5(const Student* s, int N);

int main()
{
    bool ok = true;
    int N = ReadInt("Enter number of students: ", 1, 1000, ok);
    if (!ok)
    {
        cout << "Input ended." << endl;
        return 0;
    }

    Student* s = CreateStudents(N);
    if (!FillStudents(s, N))
    {
        cout << "Input ended." << endl;
        delete[] s;
        return 0;
    }

    PrintTable(s, N);

    cout << "Number of excellent students: " << CountExcellent(s, N) << endl;
    cout << fixed << setprecision(2);
    cout << "Percent of students with physics mark 5: " << PercentPhysics5(s, N) << "%" << endl;

    delete[] s;
    return 0;
}

// Повертає назву спеціальності
string SpecialtyName(Specialty s)
{
    switch (s)
    {
    case CS: return "Computer Science";
    case INFORMATICS: return "Informatics";
    case MATH_ECON: return "Math and Economics";
    case PHYS_INF: return "Physics and Informatics";
    case LABOR: return "Labor Training";
    }
    return "";
}

// Зчитує ціле число з контролем помилок; ok=false якщо кінець вводу
int ReadInt(const string& prompt, int minV, int maxV, bool& ok)
{
    int v;
    ok = true;
    while (true)
    {
        cout << prompt;
        if (cin >> v && v >= minV && v <= maxV)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return v;
        }
        if (cin.eof())
        {
            ok = false;
            return 0;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input, try again." << endl;
    }
}

// Зчитує прізвище
bool ReadSurname(const string& prompt, string& s)
{
    while (true)
    {
        cout << prompt;
        if (!(cin >> s))
        {
            if (cin.eof())
                return false;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input, try again." << endl;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return true;
    }
}

// Зчитує одного студента
bool ReadStudent(Student& st, int index)
{
    bool ok;
    cout << "Student #" << index + 1 << endl;
    if (!ReadSurname("  Surname: ", st.surname)) return false;
    st.course = ReadInt("  Course (1-6): ", 1, 6, ok);
    if (!ok) return false;
    cout << "  Specialties: 0 - Computer Science, 1 - Informatics, 2 - Math and Economics, 3 - Physics and Informatics, 4 - Labor Training" << endl;
    st.specialty = (Specialty)ReadInt("  Specialty (0-4): ", 0, SPECIALTY_COUNT - 1, ok);
    if (!ok) return false;
    st.physics = ReadInt("  Physics mark (1-5): ", 1, 5, ok);
    if (!ok) return false;
    st.math = ReadInt("  Math mark (1-5): ", 1, 5, ok);
    if (!ok) return false;

    string prompt;
    switch (st.specialty)
    {
    case CS: prompt = "  Programming mark (1-5): "; break;
    case INFORMATICS: prompt = "  Numerical methods mark (1-5): "; break;
    default: prompt = "  Pedagogy mark (1-5): "; break;
    }
    int m = ReadInt(prompt, 1, 5, ok);
    if (!ok) return false;
    switch (st.specialty)
    {
    case CS: st.third.programming = m; break;
    case INFORMATICS: st.third.numMethods = m; break;
    default: st.third.pedagogy = m; break;
    }
    return true;
}

// Створює динамічний масив студентів
Student* CreateStudents(int N)
{
    return new Student[N];
}

// Заповнює масив з клавіатури
bool FillStudents(Student* s, int N)
{
    for (int i = 0; i < N; i++)
        if (!ReadStudent(s[i], i))
            return false;
    return true;
}

// Виводить масив у вигляді таблиці
void PrintTable(const Student* s, int N)
{
    const int W = 9 * 0;
    (void)W;
    string line(112, '-');
    cout << endl << "Students table" << endl;
    cout << line << endl;
    cout << "|" << setw(4) << "No"
        << "|" << setw(15) << "Surname"
        << "|" << setw(7) << "Course"
        << "|" << setw(25) << "Specialty"
        << "|" << setw(8) << "Physics"
        << "|" << setw(6) << "Math"
        << "|" << setw(12) << "Programming"
        << "|" << setw(9) << "NumMeth"
        << "|" << setw(9) << "Pedagogy" << "|" << endl;
    cout << line << endl;
    for (int i = 0; i < N; i++)
    {
        cout << "|" << setw(4) << i + 1
            << "|" << setw(15) << s[i].surname
            << "|" << setw(7) << s[i].course
            << "|" << setw(25) << SpecialtyName(s[i].specialty)
            << "|" << setw(8) << s[i].physics
            << "|" << setw(6) << s[i].math;
        switch (s[i].specialty)
        {
        case CS:
            cout << "|" << setw(12) << s[i].third.programming
                << "|" << setw(9) << "" << "|" << setw(9) << "" << "|";
            break;
        case INFORMATICS:
            cout << "|" << setw(12) << ""
                << "|" << setw(9) << s[i].third.numMethods << "|" << setw(9) << "" << "|";
            break;
        default:
            cout << "|" << setw(12) << ""
                << "|" << setw(9) << "" << "|" << setw(9) << s[i].third.pedagogy << "|";
            break;
        }
        cout << endl;
    }
    cout << line << endl;
}

// Повертає третю оцінку студента залежно від спеціальності
int ThirdMarkOf(const Student& st)
{
    switch (st.specialty)
    {
    case CS: return st.third.programming;
    case INFORMATICS: return st.third.numMethods;
    default: return st.third.pedagogy;
    }
}

// Перевіряє, чи студент вчиться на "відмінно"
bool IsExcellent(const Student& st)
{
    return st.physics == 5 && st.math == 5 && ThirdMarkOf(st) == 5;
}

// Повертає кількість студентів-відмінників
int CountExcellent(const Student* s, int N)
{
    int k = 0;
    for (int i = 0; i < N; i++)
        if (IsExcellent(s[i]))
            k++;
    return k;
}

// Повертає процент студентів з оцінкою 5 з фізики
double PercentPhysics5(const Student* s, int N)
{
    if (N <= 0) return 0;
    int k = 0;
    for (int i = 0; i < N; i++)
        if (s[i].physics == 5)
            k++;
    return 100.0 * k / N;
}