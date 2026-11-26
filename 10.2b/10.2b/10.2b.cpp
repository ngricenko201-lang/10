// Lab 10.2
// Sorting and binary search in an array of structures with unions
// Варіант 4
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <limits>
using namespace std;

// Перелік спеціальностей
enum Specialty { COMPUTER_SCIENCE, INFORMATICS, MATH_ECONOMICS, PHYSICS_INFORMATICS, LABOR_TRAINING };

// Об'єднання для третьої оцінки (залежить від спеціальності)
// Усі поля займають одну й ту саму пам'ять: осмислене лише одне з них,
// яке саме - визначає спеціальність студента
union ThirdGrade
{
	int programming;	// Комп'ютерні науки
	int numerical;		// Інформатика
	int pedagogy;		// інші спеціальності
};

// Структура студента
struct Student
{
	string surname;		// прізвище
	int course;			// курс
	Specialty spec;		// спеціальність
	int physics;		// оцінка з фізики
	int math;			// оцінка з математики
	ThirdGrade third;	// третя оцінка
};

int InputInt(const char* prompt, int minValue, int maxValue);
string InputSurname(const char* prompt);
double InputDouble(const char* prompt, double minValue, double maxValue);
const char* SpecialtyName(Specialty spec);
int GetThirdGrade(const Student& s);
void SetThirdGrade(Student& s, int value);
int GetAvgKey(const Student& s);
Student* CreateStudents(int N);
void InputStudent(Student& s);
void FillStudents(Student* s, int N);
void PrintHeader();
void PrintRow(const Student& s, int number);
void PrintTable(const Student* s, int N);
void PrintIndexed(const Student* s, const int* idx, int N);
int CompareByCourseAndAvg(const Student& a, const Student& b);
int ComparePhysical(const Student& a, const Student& b);
int CompareIndexed(const Student& a, const Student& b);
void PhysicalSort(Student* s, int N);
int* BuildIndexArray(const Student* s, int N);
int BinarySearch(const Student* s, const int* idx, int N, const string& surname, int course, int avgKey);
void PrintMenu();

int main()
{
	Student* s = nullptr;	// масив студентів
	int* idx = nullptr;		// індексний масив
	int N = 0;
	int choice;

	do
	{
		PrintMenu();
		// читаємо пункт меню
		choice = InputInt("Your choice: ", 0, 6);
		switch (choice)
		{
		case 1: // створення масиву та введення даних
			delete[] s; // звільняємо старі масиви, якщо були
			delete[] idx;
			idx = nullptr; // нові дані - індексу ще немає
			N = InputInt("Number of students: ", 1, 1000);
			s = CreateStudents(N);
			FillStudents(s, N);
			break;
		case 2: // вивід таблиці
			if (s) PrintTable(s, N);
			else cout << "Array is empty." << endl;
			break;
		case 3: // фізичне впорядкування
			if (s)
			{
				PhysicalSort(s, N);
				delete[] idx;	// індексний масив став неактуальним
				idx = nullptr;
				cout << "Array is physically sorted." << endl;
			}
			else cout << "Array is empty." << endl;
			break;
		case 4: // побудова індексного масиву
			if (s)
			{
				delete[] idx; // прибираємо старий індекс, якщо був
				idx = BuildIndexArray(s, N);
				cout << "Index array is built." << endl;
			}
			else cout << "Array is empty." << endl;
			break;
		case 5: // вивід таблиці в порядку індексного масиву
			if (s && idx) PrintIndexed(s, idx, N);
			else cout << "Build the index array first." << endl;
			break;
		case 6: // бінарний пошук (потрібен побудований індекс)
			if (s && idx)
			{
				string surname = InputSurname("Surname: ");
				int course = InputInt("Course (1-6): ", 1, 6);
				double avg = InputDouble("Average grade: ", 1, 5);
				// середній бал переводимо в ціле (*100), щоб порівнювати без похибок double
				int key = (int)round(avg * 100.0);
				int pos = BinarySearch(s, idx, N, surname, course, key);
				if (pos >= 0)
					cout << "Student found, position in the index array: " << pos + 1 << endl;
				else
					cout << "Student not found." << endl;
			}
			else cout << "Build the index array first." << endl;
			break;
		}
	} while (choice != 0);

	delete[] s; // звільняємо пам'ять перед виходом
	delete[] idx;
	return 0;
}

// Виведення меню
void PrintMenu()
{
	cout << endl << "===== MENU =====" << endl;
	cout << "1 - Enter students" << endl;
	cout << "2 - Print table" << endl;
	cout << "3 - Physical sort" << endl;
	cout << "4 - Build index array" << endl;
	cout << "5 - Print table by index array" << endl;
	cout << "6 - Binary search" << endl;
	cout << "0 - Exit" << endl;
}

// Введення цілого числа з контролем помилок; при кінці вводу повертає minValue
int InputInt(const char* prompt, int minValue, int maxValue)
{
	int x;
	while (true)
	{
		cout << prompt;
		if (cin >> x && x >= minValue && x <= maxValue)
		{
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			return x;
		}
		if (cin.eof())
			return minValue;
		cin.clear(); // скидаємо прапорці помилки
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); // відкидаємо некоректний рядок
		cout << "Invalid input, try again." << endl;
	}
}

// Введення дійсного числа з контролем помилок; при кінці вводу повертає minValue
double InputDouble(const char* prompt, double minValue, double maxValue)
{
	double x;
	while (true)
	{
		cout << prompt;
		if (cin >> x && x >= minValue && x <= maxValue)
		{
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			return x;
		}
		if (cin.eof())
			return minValue;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid input, try again." << endl;
	}
}

// Введення прізвища (одне слово); при кінці вводу повертає порожній рядок
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
			return "";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid input, try again." << endl;
	}
}

// Повертає назву спеціальності
const char* SpecialtyName(Specialty spec)
{
	switch (spec)
	{
	case COMPUTER_SCIENCE: return "Computer Science";
	case INFORMATICS: return "Informatics";
	case MATH_ECONOMICS: return "Math & Economics";
	case PHYSICS_INFORMATICS: return "Physics & Informatics";
	default: return "Labor Training";
	}
}

// Повертає третю оцінку залежно від спеціальності
// (читаємо те поле об'єднання, яке було записане для цієї спеціальності)
int GetThirdGrade(const Student& s)
{
	switch (s.spec)
	{
	case COMPUTER_SCIENCE: return s.third.programming;
	case INFORMATICS: return s.third.numerical;
	default: return s.third.pedagogy;
	}
}

// Записує третю оцінку у відповідне поле об'єднання
void SetThirdGrade(Student& s, int value)
{
	switch (s.spec)
	{
	case COMPUTER_SCIENCE: s.third.programming = value; break;
	case INFORMATICS: s.third.numerical = value; break;
	default: s.third.pedagogy = value; break;
	}
}

// Повертає ключ середнього балу: середній бал * 100, округлений до цілого
// (цілий ключ дозволяє порівнювати бали без похибок дійсних чисел)
int GetAvgKey(const Student& s)
{
	int sum = s.physics + s.math + GetThirdGrade(s);
	return (int)round(sum * 100.0 / 3.0);
}

// Створює динамічний масив з N студентів
Student* CreateStudents(int N)
{
	return new Student[N];
}

// Введення даних одного студента
void InputStudent(Student& s)
{
	s.surname = InputSurname("Surname: ");
	s.course = InputInt("Course (1-6): ", 1, 6);
	cout << "1 - Computer Science, 2 - Informatics, 3 - Math & Economics," << endl;
	cout << "4 - Physics & Informatics, 5 - Labor Training" << endl;
	// користувач вводить 1-5, а в enum номери з 0 - віднімаємо 1
	s.spec = (Specialty)(InputInt("Specialty (1-5): ", 1, 5) - 1);
	s.physics = InputInt("Physics grade (1-5): ", 1, 5);
	s.math = InputInt("Math grade (1-5): ", 1, 5);
	// третя оцінка залежить від спеціальності - запитуємо відповідну
	switch (s.spec)
	{
	case COMPUTER_SCIENCE:
		SetThirdGrade(s, InputInt("Programming grade (1-5): ", 1, 5));
		break;
	case INFORMATICS:
		SetThirdGrade(s, InputInt("Numerical methods grade (1-5): ", 1, 5));
		break;
	default:
		SetThirdGrade(s, InputInt("Pedagogy grade (1-5): ", 1, 5));
		break;
	}
}

// Введення масиву студентів
void FillStudents(Student* s, int N)
{
	for (int i = 0; i < N; i++)
	{
		cout << "--- Student " << i + 1 << " ---" << endl;
		InputStudent(s[i]);
	}
}

// Виведення заголовка і шапки таблиці
void PrintHeader()
{
	cout << endl << "                         STUDENTS" << endl;
	cout << string(110, '-') << endl;
	cout << "|" << setw(4) << "No" << "|" << setw(15) << left << "Surname" << "|"
		<< setw(7) << "Course" << "|" << setw(23) << "Specialty" << "|"
		<< setw(8) << "Physics" << "|" << setw(6) << "Math" << "|"
		<< setw(12) << "Programming" << "|" << setw(9) << "Numerical" << "|"
		<< setw(9) << "Pedagogy" << "|" << right << endl;
	cout << string(110, '-') << endl;
}

// Виведення одного рядка таблиці
void PrintRow(const Student& s, int number)
{
	cout << "|" << setw(4) << number << "|" << setw(15) << left << s.surname << "|" << right
		<< setw(7) << s.course << "|" << setw(23) << left << SpecialtyName(s.spec) << "|" << right
		<< setw(8) << s.physics << "|" << setw(6) << s.math << "|";
	// третя оцінка виводиться лише в стовпці, що відповідає спеціальності,
	// в інших стовпцях - порожні клітинки
	if (s.spec == COMPUTER_SCIENCE) cout << setw(12) << s.third.programming << "|";
	else cout << setw(12) << " " << "|";
	if (s.spec == INFORMATICS) cout << setw(9) << s.third.numerical << "|";
	else cout << setw(9) << " " << "|";
	if (s.spec != COMPUTER_SCIENCE && s.spec != INFORMATICS) cout << setw(9) << s.third.pedagogy << "|";
	else cout << setw(9) << " " << "|";
	cout << endl;
}

// Виведення масиву у вигляді таблиці
void PrintTable(const Student* s, int N)
{
	PrintHeader();
	for (int i = 0; i < N; i++)
		PrintRow(s[i], i + 1);
	cout << string(110, '-') << endl;
}

// Виведення масиву у вигляді таблиці в порядку індексного масиву
void PrintIndexed(const Student* s, const int* idx, int N)
{
	PrintHeader();
	for (int i = 0; i < N; i++)
		PrintRow(s[idx[i]], i + 1); // беремо студента за номером з індексу
	cout << string(110, '-') << endl;
}

// Порівняння за курсом і середнім балом (за зростанням): -1, 0, 1
int CompareByCourseAndAvg(const Student& a, const Student& b)
{
	if (a.course != b.course)
		return a.course < b.course ? -1 : 1;
	int ka = GetAvgKey(a), kb = GetAvgKey(b);
	if (ka != kb)
		return ka < kb ? -1 : 1;
	return 0;
}

// Порівняння для фізичного впорядкування: курс, середній бал, прізвище за спаданням
int ComparePhysical(const Student& a, const Student& b)
{
	int r = CompareByCourseAndAvg(a, b);
	if (r != 0)
		return r;
	int c = b.surname.compare(a.surname); // a і b навмисно поміняні місцями - спадання
	return c < 0 ? -1 : (c > 0 ? 1 : 0);
}

// Порівняння для індексного впорядкування: курс, середній бал, прізвище за зростанням
int CompareIndexed(const Student& a, const Student& b)
{
	int r = CompareByCourseAndAvg(a, b);
	if (r != 0)
		return r;
	int c = a.surname.compare(b.surname);
	return c < 0 ? -1 : (c > 0 ? 1 : 0);
}

// Фізичне впорядкування масиву (метод бульбашки)
void PhysicalSort(Student* s, int N)
{
	for (int i = 0; i < N - 1; i++)
		for (int j = 0; j < N - 1 - i; j++)
			if (ComparePhysical(s[j], s[j + 1]) > 0) // сусіди стоять не в тому порядку
			{
				Student tmp = s[j];
				s[j] = s[j + 1];
				s[j + 1] = tmp;
			}
}

// Будує індексний масив (метод вставок); виділена пам'ять має бути звільнена delete[]
int* BuildIndexArray(const Student* s, int N)
{
	int* idx = new int[N];
	for (int i = 0; i < N; i++)
		idx[i] = i; // початково індекси йдуть по порядку
	// сортування вставками: переставляємо номери, а самі студенти лишаються на місці
	for (int i = 1; i < N; i++)
	{
		int tmp = idx[i];
		int j = i - 1;
		while (j >= 0 && CompareIndexed(s[idx[j]], s[tmp]) > 0)
		{
			idx[j + 1] = idx[j]; // зсуваємо більші індекси вправо
			j--;
		}
		idx[j + 1] = tmp;
	}
	return idx;
}

// Бінарний пошук в індексно впорядкованому масиві.
// Повертає позицію в індексному масиві або -1, якщо студента не знайдено
int BinarySearch(const Student* s, const int* idx, int N, const string& surname, int course, int avgKey)
{
	int L = 0, R = N - 1; // межі пошуку
	while (L <= R)
	{
		int m = (L + R) / 2; // середній елемент
		const Student& cur = s[idx[m]];
		int r; // результат порівняння студента з ключем: -1, 0, 1
		// порівнюємо в тому ж порядку, що й при побудові індексу: курс, середній бал, прізвище
		if (cur.course != course)
			r = cur.course < course ? -1 : 1;
		else if (GetAvgKey(cur) != avgKey)
			r = GetAvgKey(cur) < avgKey ? -1 : 1;
		else
		{
			int c = cur.surname.compare(surname);
			r = c < 0 ? -1 : (c > 0 ? 1 : 0);
		}
		if (r == 0)
			return m;     // знайшли
		if (r < 0)
			L = m + 1;    // шукаємо в правій половині
		else
			R = m - 1;    // шукаємо в лівій половині
	}
	return -1;
}