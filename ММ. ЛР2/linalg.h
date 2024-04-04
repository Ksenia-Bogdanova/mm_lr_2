#pragma once
#include <fstream>
#include <fstream>
#include <fstream>
#include <cstring>
// Опережающие объявления
class TMatrix;
class TQuaternion;
// Объявление класса векторов
class TVector {
protected:
	// Размерность вектора
	int n;
	// Элементы вектора
	double* data;
public:
	// Конструктор по умолчанию
	TVector();
	// Конструктор с заданным кол-вом элементов
	TVector(int n);
	// Конструктор копий
	TVector(const TVector& rvalue);
	// Оператор присваивания
	TVector& operator = (const TVector& rvalue);
	// Деструктор
	virtual ~TVector();
	// Функция получение кол-ва элементов вектора
	inline int size() const { return n; }
	// Функция получения индекса последнего элемента
	inline int high() const { return n - 1; }
	// Функция задания кол-ва элементов вектора
	void resize(int n);
	// Оператор доступа к элементам вектора
	inline double& operator[](int i) { return data[i]; }
	// Оператор константного доступа к элементам вектора
	inline const double& operator[](int i) const { return data[i]; }
	// Оператор - унарный минус
	TVector operator - () const;
	// Оператор вычитания векторов
	TVector operator - (const TVector& arg) const;
	// Оператор сложения векторов
	TVector operator + (const TVector& arg) const;
	// Оператор умножения вектора на число
	TVector operator * (double arg) const;
	// Оператор скалярного умножения векторов
	double operator * (const TVector& arg) const;
	// Оператор умножения вектора на матрицу
	TVector operator * (const TMatrix& arg) const;
	// Оператор умножения вектора на кватернион
	//TQuaternion operator * (const TQuaternion& arg) const;
	// Оператор векторного умножения векторов
	TVector operator ^ (const TVector& arg) const;
	// Дружественная функция - оператор умножения числа на вектор
	friend TVector operator * (double lvalue, const TVector& rvalue);
	// Функция получения модуля вектора
	double length() const;
	// Функция нормирования вектора
	TVector norm();
	void print();
	// Поворот вектора вокруг заданной оси на заданный угол при помощи формулы Родрига
		//TVector rotateByRodrigFormula(double phi, const TVector& axis) const;
	// Поворот вектора вокруг заданной оси на заданный угол при помощи кватерниона
		//TVector rotate(double phi, const TVector& axis) const;
	// Поворот векора при помощи заданного кватерниона
	//TVector rotateByQuaternion(const TQuaternion& L) const;
};
// Объявление класса матриц
class TMatrix {
protected:
	// Размерность матрицы (число строк и столбцов)
	int n, m;
	// Элементы матрицы
	double** data;
public:
	// Конструктор по умолчанию
	TMatrix();
	// Конструктор с заданной размерностью
	TMatrix(int n, int m);
	// Конструктор копий
	TMatrix(const TMatrix& rvalue);
	// Оператор присваивания
	TMatrix& operator = (const TMatrix& rvalue);
	// Деструктор
	virtual ~TMatrix();
	// Функция получения количества строк
	inline int rowCount() const { return n; }
	// Функция получения кол-ва столбцов
	inline int colCount() const { return m; }
	// Функция получения индекса последней строки
	inline int rowHigh() const { return n - 1; }
	// Функция получения индекса последнего столбца
	inline int colHigh() const { return m - 1; }
	// Функция задания размерности
	void resize(int n, int m);
	// Оператор доступа к элементам матрицы
	inline double& operator()(int i, int j) { return data[i][j]; }
	// Оператор константного доступа к элементам вектора
	inline const double& operator()(int i, int j) const { return data[i][j]; }
	// Оператор - унарный минус
	TMatrix operator - () const;
	// Оператор вычитания матриц
	TMatrix operator - (const TMatrix& arg) const;
	// Оператор сложения матриц
	TMatrix operator + (const TMatrix& arg) const;
	// Оператор умножения матрицы на число
	TMatrix operator * (double arg) const;
	// Оператор умножения матриц
	TMatrix operator * (const TMatrix& arg) const;
	// Оператор умножения матрицы на вектор
	TVector operator * (const TVector& arg) const;
	// Дружественная функция - оператор умножения числа на матрицу
	friend TMatrix operator * (double lvalue, const TMatrix& rvalue);
	// Оператор обращения матриц (метод Гаусса)
	TMatrix appeal() const;
	// Функция вычисления детерминанта
	double det() const;
	// Функция транспонирования
	TMatrix t() const;
	// Функция формирования единичной матрицы
	static TMatrix E(int n);
	// Функция перестановки строк
	TMatrix swapRows(int i, int j, const TMatrix& arg);
	// Функция вывод матрицы
	void print();
	// Функция добавления строк в матрицу
	void push(const TVector& arg);
	void print_col(int number);
	void go_to_file(std::string name);
};
