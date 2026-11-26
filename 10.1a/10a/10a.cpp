// Lab 10.1
// Sequential search in an array of structures
// Варіант 4
#pragma warning(disable : 4996)
#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
using namespace std;

// Перелік спеціальностей
enum Specialty { COMPUTER_SCIENCE, INFORMATICS, MATH_ECONOMICS, PHYSICS_INFORMATICS, LABOR_TRAINING };

const int SPECIALTY_COUNT = 5;

// Назви спеціальностей для виводу
const string specialtyNames[SPECIALTY_COUNT] = {
	"Computer Science",
	"Informatics",
	"Math and Economics",
	"Physics and Informatics",
	"Labor Training"
};

// Структура студента
struct Student
{
	string surname;     // прізвище
	int course;         // курс
	Specialty specialty; // спеціальність
	int physics;        // оцінка з фізики
	int math;           // оцінка з математики
	int informatics;    // оцінка з інформатики
};

// Прототипи функцій
int ReadInt(const string& prompt, int minValue, int maxValue);
string ReadString(const string& prompt);
void Create(Student* s, const int N);
void PrintTable(const Student* s, const int N);
bool IsExcellent(const Student& st);
int CountExcellent(const Student* s, const int N);
double PercentPhysics5(const Student* s, const int N);

int main()
{
	int N = ReadInt("Enter number of students: ", 1, 1000);

	// Динамічний масив структур
	Student* s = new Student[N];

	Create(s, N);
	PrintTable(s, N);

	cout << "Number of excellent students: " << CountExcellent(s, N) << endl;
	cout << fixed << setprecision(2)
		<< "Percent of students with physics grade 5: " << PercentPhysics5(s, N) << "%" << endl;

	delete[] s;
	return 0;
}

// Введення цілого числа з контролем помилок; повертає значення в діапазоні [minValue, maxValue]
int ReadInt(const string& prompt, int minValue, int maxValue)
{
	int value;
	while (true)
	{
		cout << prompt;
		if (cin >> value && value >= minValue && value <= maxValue)
		{
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			return value;
		}
		if (cin.eof())
		{
			cout << "Input ended unexpectedly." << endl;
			exit(1); // кінець вводу, щоб уникнути зациклення
		}
		cout << "Invalid input. Enter an integer from " << minValue << " to " << maxValue << "." << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
}

// Введення непорожнього рядка (прізвища)
string ReadString(const string& prompt)
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
			cout << "Input ended unexpectedly." << endl;
			exit(1);
		}
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid input." << endl;
	}
}

// Заповнення масиву студентів з клавіатури
void Create(Student* s, const int N)
{
	for (int i = 0; i < N; i++)
	{
		cout << "Student #" << i + 1 << ":" << endl;
		s[i].surname = ReadString("  Surname: ");
		s[i].course = ReadInt("  Course (1-6): ", 1, 6);
		cout << "  Specialties:" << endl;
		for (int k = 0; k < SPECIALTY_COUNT; k++)
			cout << "    " << k << " - " << specialtyNames[k] << endl;
		s[i].specialty = (Specialty)ReadInt("  Specialty (0-4): ", 0, SPECIALTY_COUNT - 1);
		s[i].physics = ReadInt("  Physics grade (1-5): ", 1, 5);
		s[i].math = ReadInt("  Math grade (1-5): ", 1, 5);
		s[i].informatics = ReadInt("  Informatics grade (1-5): ", 1, 5);
	}
}

// Вивід масиву у вигляді таблиці
void PrintTable(const Student* s, const int N)
{
	const string line(88, '-');
	cout << endl << "Students table" << endl;
	cout << line << endl;
	cout << "| " << setw(3) << left << "No"
		<< "| " << setw(15) << left << "Surname"
		<< "| " << setw(7) << left << "Course"
		<< "| " << setw(25) << left << "Specialty"
		<< "| " << setw(8) << left << "Physics"
		<< "| " << setw(6) << left << "Math"
		<< "| " << setw(6) << left << "Inf" << "|" << endl;
	cout << line << endl;
	for (int i = 0; i < N; i++)
	{
		cout << "| " << setw(3) << left << i + 1
			<< "| " << setw(15) << left << s[i].surname
			<< "| " << setw(7) << left << s[i].course
			<< "| " << setw(25) << left << specialtyNames[s[i].specialty]
			<< "| " << setw(8) << left << s[i].physics
			<< "| " << setw(6) << left << s[i].math
			<< "| " << setw(6) << left << s[i].informatics << "|" << endl;
	}
	cout << line << endl << endl;
}

// Перевіряє, чи студент вчиться на "відмінно" (усі оцінки рівні 5)
bool IsExcellent(const Student& st)
{
	return st.physics == 5 && st.math == 5 && st.informatics == 5;
}

// Обчислює кількість студентів, які вчаться на "відмінно"
int CountExcellent(const Student* s, const int N)
{
	int count = 0;
	for (int i = 0; i < N; i++)
		if (IsExcellent(s[i]))
			count++;
	return count;
}

// Обчислює процент студентів, які отримали з фізики оцінку "5"
double PercentPhysics5(const Student* s, const int N)
{
	if (N <= 0)
		return 0.0;
	int count = 0;
	for (int i = 0; i < N; i++)
		if (s[i].physics == 5)
			count++;
	return 100.0 * count / N;
}