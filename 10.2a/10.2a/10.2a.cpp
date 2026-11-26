// Lab 10.2
// Впорядкування та бінарний пошук в масиві структур
// Варіант 4
#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <cmath>
#include <cstdlib>
using namespace std;

// Перелік спеціальностей
enum Specialty { COMPUTER_SCIENCE, INFORMATICS, MATH_ECONOMICS, PHYSICS_INFORMATICS, LABOR_TRAINING };

const int SPECIALTY_COUNT = 5;

// Структура студента (рівень А: без об'єднань)
struct Student
{
    string surname;      // прізвище
    int course;          // курс
    Specialty specialty; // спеціальність
    int physics;         // оцінка з фізики
    int math;            // оцінка з математики
    int informatics;     // оцінка з інформатики
};

// Прототипи
const char* SpecialtyName(Specialty sp);
int Sum(const Student& st);
double Average(const Student& st);
int InputInt(const char* prompt, int minV, int maxV);
double InputDouble(const char* prompt, double minV, double maxV);
string InputSurname(const char* prompt);
Specialty InputSpecialty();
void InputStudents(Student* s, int n);
void PrintTable(const Student* s, int n);
void PrintIndexTable(const Student* s, const int* idx, int n);
bool LessPhysical(const Student& a, const Student& b);
void SortPhysical(Student* s, int n);
int CompareIndexOrder(const Student& a, const Student& b);
void BuildIndex(const Student* s, int* idx, int n);
int CompareToKey(const Student& st, int course, int sum, const string& surname);
int BinarySearch(const Student* s, const int* idx, int n, const string& surname, int course, double avg);
void ShowMenu();

int main()
{
    Student* s = nullptr; // динамічний масив студентів
    int* idx = nullptr;   // індексний масив
    int n = 0;
    bool indexBuilt = false;
    int choice;

    do
    {
        ShowMenu();
        // читаємо пункт меню
        choice = InputInt("Your choice: ", 0, 6);

        switch (choice)
        {
        case 1: // створення масиву та введення даних
            delete[] s; // звільняємо старі масиви, якщо були
            delete[] idx;
            s = nullptr;
            idx = nullptr;
            n = InputInt("Number of students: ", 1, 1000);
            s = new Student[n]; // виділяємо пам'ять під студентів і під індекс
            idx = new int[n];
            InputStudents(s, n);
            indexBuilt = false; // новий масив - індекс ще не побудований
            break;
        case 2: // вивід таблиці
            if (n == 0) cout << "No data. Create the array first." << endl;
            else PrintTable(s, n);
            break;
        case 3: // фізичне впорядкування
            if (n == 0) cout << "No data. Create the array first." << endl;
            else
            {
                SortPhysical(s, n);
                indexBuilt = false; // порядок змінився - індекс недійсний
                cout << "Array was physically sorted." << endl;
            }
            break;
        case 4: // побудова індексного масиву
            if (n == 0) cout << "No data. Create the array first." << endl;
            else
            {
                BuildIndex(s, idx, n);
                indexBuilt = true;
                cout << "Index array was built." << endl;
            }
            break;
        case 5: // вивід таблиці за індексом
            if (!indexBuilt) cout << "Index array is not built. Use item 4." << endl;
            else PrintIndexTable(s, idx, n);
            break;
        case 6: // бінарний пошук
            if (!indexBuilt) cout << "Index array is not built. Use item 4." << endl;
            else
            {
                string surname = InputSurname("Surname: ");
                int course = InputInt("Course (1-6): ", 1, 6);
                double avg = InputDouble("Average mark (2-5): ", 2.0, 5.0);
                // pos - позиція в індексному масиві, idx[pos] - в основному
                int pos = BinarySearch(s, idx, n, surname, course, avg);
                if (pos >= 0)
                    cout << "Student found: position " << idx[pos] + 1 << " in the array." << endl;
                else
                    cout << "Student not found." << endl;
            }
            break;
        }
    } while (choice != 0);

    delete[] s; // звільняємо пам'ять перед виходом
    delete[] idx;
    return 0;
}

// Повертає назву спеціальності
const char* SpecialtyName(Specialty sp)
{
    static const char* names[SPECIALTY_COUNT] = {
        "Computer Science", "Informatics", "Math & Economics",
        "Physics & Informatics", "Labor Training" };
    return names[sp];
}

// Повертає суму трьох оцінок
int Sum(const Student& st)
{
    return st.physics + st.math + st.informatics;
}

// Повертає середній бал студента
double Average(const Student& st)
{
    return Sum(st) / 3.0;
}

// Введення цілого числа з перевіркою діапазону
int InputInt(const char* prompt, int minV, int maxV)
{
    int v;
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
            cout << "\nInput ended. Exit." << endl;
            exit(0);
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Try again." << endl;
    }
}

// Введення дійсного числа з перевіркою діапазону
double InputDouble(const char* prompt, double minV, double maxV)
{
    double v;
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
            cout << "\nInput ended. Exit." << endl;
            exit(0);
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Try again." << endl;
    }
}

// Введення прізвища (одне слово, непорожнє)
string InputSurname(const char* prompt)
{
    string str;
    while (true)
    {
        cout << prompt;
        if (cin >> str)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return str;
        }
        if (cin.eof())
        {
            cout << "\nInput ended. Exit." << endl;
            exit(0);
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Try again." << endl;
    }
}

// Введення спеціальності з меню
Specialty InputSpecialty()
{
    for (int i = 0; i < SPECIALTY_COUNT; i++)
        cout << "  " << i << " - " << SpecialtyName((Specialty)i) << endl;
    return (Specialty)InputInt("Specialty: ", 0, SPECIALTY_COUNT - 1);
}

// Введення даних усіх студентів
void InputStudents(Student* s, int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << "Student #" << i + 1 << endl;
        s[i].surname = InputSurname("Surname: ");
        s[i].course = InputInt("Course (1-6): ", 1, 6);
        s[i].specialty = InputSpecialty();
        s[i].physics = InputInt("Physics (2-5): ", 2, 5);
        s[i].math = InputInt("Math (2-5): ", 2, 5);
        s[i].informatics = InputInt("Informatics (2-5): ", 2, 5);
    }
}

// Вивід шапки таблиці
static void PrintHeader()
{
    cout << "                    STUDENTS" << endl;
    cout << string(86, '=') << endl;
    cout << "|" << setw(4) << "No" << "|" << setw(15) << left << "Surname" << "|"
        << setw(7) << "Course" << "|" << setw(23) << "Specialty" << "|"
        << setw(8) << "Physics" << "|" << setw(6) << "Math" << "|"
        << setw(12) << "Informatics" << "|" << right << endl;
    cout << string(86, '-') << endl;
}

// Вивід рядка таблиці
static void PrintRow(int no, const Student& st)
{
    cout << "|" << setw(4) << no << "|" << setw(15) << left << st.surname << "|"
        << setw(7) << st.course << "|" << setw(23) << SpecialtyName(st.specialty) << "|"
        << setw(8) << st.physics << "|" << setw(6) << st.math << "|"
        << setw(12) << st.informatics << "|" << right << endl;
}

// Вивід масиву у вигляді таблиці
void PrintTable(const Student* s, int n)
{
    PrintHeader();
    for (int i = 0; i < n; i++)
        PrintRow(i + 1, s[i]);
    cout << string(86, '=') << endl;
}

// Вивід масиву у вигляді таблиці в порядку індексного масиву
void PrintIndexTable(const Student* s, const int* idx, int n)
{
    PrintHeader();
    for (int i = 0; i < n; i++)
        PrintRow(i + 1, s[idx[i]]);
    cout << string(86, '=') << endl;
}

// Чи має a стояти перед b при фізичному впорядкуванні:
// курс (зростання), середній бал (зростання), прізвище (спадання)
bool LessPhysical(const Student& a, const Student& b)
{
    if (a.course != b.course) return a.course < b.course;
    if (Sum(a) != Sum(b)) return Sum(a) < Sum(b);
    return a.surname > b.surname;
}

// Фізичне впорядкування масиву (обмін елементів)
void SortPhysical(Student* s, int n)
{
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - 1 - i; j++)
            if (LessPhysical(s[j + 1], s[j])) // сусідні елементи стоять не в тому порядку
            {
                Student tmp = s[j];
                s[j] = s[j + 1];
                s[j + 1] = tmp;
            }
}

// Порівняння для індексного впорядкування:
// курс, середній бал, прізвище (усе за зростанням).
// Повертає від'ємне, 0 або додатне значення
int CompareIndexOrder(const Student& a, const Student& b)
{
    if (a.course != b.course) return a.course < b.course ? -1 : 1;
    if (Sum(a) != Sum(b)) return Sum(a) < Sum(b) ? -1 : 1;
    if (a.surname != b.surname) return a.surname < b.surname ? -1 : 1;
    return 0;
}

// Побудова індексного масиву
void BuildIndex(const Student* s, int* idx, int n)
{
    for (int i = 0; i < n; i++)
        idx[i] = i; // початково індекси йдуть по порядку
    // сортування вставками: переставляємо індекси, а не самих студентів
    for (int i = 1; i < n; i++)
    {
        int value = idx[i];
        int j = i - 1;
        while (j >= 0 && CompareIndexOrder(s[value], s[idx[j]]) < 0)
        {
            idx[j + 1] = idx[j];
            j--;
        }
        idx[j + 1] = value;
    }
}

// Порівняння студента з ключем пошуку (курс, сума оцінок, прізвище)
int CompareToKey(const Student& st, int course, int sum, const string& surname)
{
    if (st.course != course) return st.course < course ? -1 : 1;
    if (Sum(st) != sum) return Sum(st) < sum ? -1 : 1;
    if (st.surname != surname) return st.surname < surname ? -1 : 1;
    return 0;
}

// Бінарний пошук в індексно впорядкованому масиві.
// Повертає позицію в індексному масиві або -1, якщо не знайдено
int BinarySearch(const Student* s, const int* idx, int n, const string& surname, int course, double avg)
{
    // середній бал = сума / 3, тому знаходимо відповідну суму
    int sum = (int)lround(avg * 3); // переводимо середній бал у суму
    if (fabs(avg - sum / 3.0) > 0.005)
        return -1; // такого середнього балу бути не може

    int lo = 0, hi = n - 1; // межі пошуку
    while (lo <= hi)
    {
        int mid = (lo + hi) / 2; // середній елемент
        int c = CompareToKey(s[idx[mid]], course, sum, surname);
        if (c == 0) return mid;  // знайшли
        if (c < 0) lo = mid + 1; // шукаємо в правій половині
        else hi = mid - 1;       // шукаємо в лівій половині
    }
    return -1;
}

// Вивід меню
void ShowMenu()
{
    cout << endl;
    cout << "========== MENU ==========" << endl;
    cout << " 1 - Create and input array" << endl;
    cout << " 2 - Print table" << endl;
    cout << " 3 - Physical sort" << endl;
    cout << " 4 - Build index array" << endl;
    cout << " 5 - Print table by index" << endl;
    cout << " 6 - Binary search" << endl;
    cout << " 0 - Exit" << endl;
}