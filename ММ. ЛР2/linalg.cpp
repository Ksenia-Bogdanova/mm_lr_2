#include "linalg.h"
#include <cstring>
#include <cstdio>
#include <cstdint>
#include <math.h>
#include <iostream>
#include <fstream>

// Векторы
// Конструктор по умолчанию
TVector::TVector() : n(0), data(NULL) {}
// Конструктор по количеству элементов
TVector::TVector(int n) : n(0), data(NULL) {
	resize(n);
}
// Конструктор копирования
TVector::TVector(const TVector& rvalue) : n(0), data(NULL) {
	(*this) = rvalue;
}
// Оператор присваивания
TVector& TVector::operator = (const TVector& rvalue) {
	// Если левый операнд не совпадает с правым
	if (this != &rvalue) {
		// Если размер левого операнда не совпадает с правым, то память выделяется заново
		if (n != rvalue.n) {
			// Если память уже была выделена, удалить её
			if (data) { delete[] data; }
			// Выделение новой памяти
			data = new double[rvalue.n];
			// Сохранение нового размера
			n = rvalue.n;
		}
		// Перенос данных из правого операнда в левый
		memmove(data, rvalue.data, sizeof(double) * n);
	}
	// Возврат ссылки на левый операнд для возможности цепочки присваиваний
	return *this;
}
// Деструктор
TVector::~TVector() {
	// Если блок данных ранее был инициализирован, удаляем его
	if (data) {
		delete[] data;
		n = 0;
		data = NULL;
	}
}
// Функция задания кол-ва элементов вектора
void TVector::resize(int n) {
#ifdef _DEBUG
	if (n < 0)
		throw 1;
#endif
	// Если новый размер совпадает со старым - выходим
	if (n == this->n) return;
	// Новый блок памяти
	double* newData = new double[n];
	// Если блок данных ранее был инициализирован...
	if (data) {
		// Минимальный из старого и нового размера блока
		int min_n = (this->n < n) ? this->n : n;
		// Перенос данных из старого блока в новый
		memmove(newData, data, sizeof(double) * min_n);
		// Удаление старого блока
		delete[] data;
	}
	// Прикрепление нового блока к объекту вектора
	data = newData;
	// Сохранение нового размера
	this->n = n;
}
// Оператор сложения векторов
TVector TVector::operator + (const TVector& arg) const {
#ifdef _DEBUG
	if (n != arg.n)
		throw 1;
#endif
	TVector V(n);
	for (int i = 0; i < n; i++)
		V[i] = data[i] + arg[i];
	return V;
}
// Оператор умножения вектора на число
TVector TVector::operator * (double arg) const {
	TVector V1(n);
	for (int i = 0; i < n; i++)
		V1[i] = data[i] * arg;
	return V1;
}
// Оператор скалярного умножения векторов
double TVector::operator * (const TVector& arg) const {
#ifdef _DEBUG
	if (n != arg.n)
		throw 1;
#endif
	double res = 0;
	for (int i = 0; i < 3; i++)
	{
		res += data[i] * arg[i];
	}
	return res;
}
// Оператор вычитания векторов
TVector TVector::operator - (const TVector& arg) const {
#ifdef _DEBUG
	if (n != arg.n)
		throw 1;
#endif
	TVector V2(n);
	for (int i = 0; i < n; i++)
		V2[i] = data[i] - arg[i];
	return V2;
}
// Оператор - унарный минус
TVector TVector::operator - () const {
	TVector V3(n);
	for (int i = 0; i < n; i++)
		V3[i] = -data[i];
	return V3;
}
// Функция получения модуля вектора
double TVector::length() const {
	double len = 0;
	for (int i = 0; i < n; i++)
	{
		len += data[i] * data[i];
	}
	len = sqrt(len);
	return len;
}
// Функция нормирования вектора
TVector TVector::norm() {
	TVector V5(n);
	double module = 0;
	for (int i = 0; i < n; i++)
		module += data[i];
	module = sqrt(module);
	for (int i = 0; i < n; i++)
		V5[i] = data[i] / module;
	return V5;
}
// Оператор умножения вектора на матрицу
TVector TVector::operator * (const TMatrix& arg) const {
#ifdef _DEBUG
	if ((n != arg.rowCount()))
		throw 1;
#endif
	TVector V6(n);
	V6[0] = 0; V6[1] = 0; V6[2] = 0;
	for (int j = 0; j < n; j++) {
		for (int i = 0; i < n; i++) {
			V6[j] += data[i] * arg(i, j);
		}
	}
	return V6;
}
// Дружественная функция - оператор умножения числа на вектор
TVector operator * (double lvalue, const TVector& rvalue) {
	TVector V7(rvalue.n);
	for (int i = 0; i < rvalue.n; i++) {
		V7[i] = lvalue * rvalue.data[i];
	}
	return V7;
}
// Оператор векторного умножения векторов
TVector TVector::operator ^ (const TVector& arg) const {
#ifdef _DEBUG
	if (n != arg.n || n > 3)
		throw 1;
#endif
	TVector V8(n);
	V8[0] = data[1] * arg[2] - data[2] * arg[1];
	V8[1] = data[2] * arg[0] - data[0] * arg[2];
	V8[2] = data[0] * arg[1] - data[1] * arg[0];
	return V8;
}

void TVector::print()
{
	for (int i = 0; i < n; i++)
		std::cout << " " << data[i];
	std::cout << std::endl;
}
/*// Поворот вектора при помощи заданного кватерниона
TVector TVector::rotateByQuaternion(const TQuaternion& L) const {
	return (L * (*this) * L.conj()).vect();
}*/
// Матрицы
// Конструктор по умолчанию
TMatrix::TMatrix() : n(0), m(0), data(NULL) {}
// Конструктор с заданной размерностью
TMatrix::TMatrix(int n, int m) : n(0), m(0), data(NULL) { resize(n, m); }
// Конструктор копий
TMatrix::TMatrix(const TMatrix& rvalue) : n(0), m(0), data(NULL) {
	(*this) = rvalue;
}
// Оператор присваивания
TMatrix& TMatrix::operator = (const TMatrix& rvalue) {
	// Если левый операнд не совпадает с правым
	if (this != &rvalue) {
		// Удаление ранее выделенной памяти
		this->~TMatrix();
		// Выделение новой памяти по размерам правого операнда
		resize(rvalue.n, rvalue.m);
		// Перенос данных из правого операнда в левый построчно
		for (int i = 0; i < n; i++)
			memmove(data[i], rvalue.data[i], sizeof(double) * m);
	}
	// Возврат ссылки на левый операнд для возможности цепочки присваиваний
	return (*this);
}
// Деструктор объекта матрицы
TMatrix::~TMatrix() {
	if (data) {
		for (int i = 0; i < n; i++)
			delete[] data[i];
		delete[] data;
		data = NULL;
		n = m = 0;
	}
}
// Функция задания размерности матрицы
void TMatrix::resize(int n, int m) {
	// Кол-во строк, которые нужно перенести в новые блоки данных
	int min_n = this->n < n ? this->n : n;
	// Если кол-во столбцов не совпадает
	if (this->m != m) {
		// Кол-во столбцов, которые нужно перенести в новые блоки данных
		int min_m = this->m < m ? this->m : m;
		// Цикл построчного переноса данных в новые блоки
		for (int i = 0; i < min_n; i++) {
			// Создание нового блока-строки
			double* newDataRow = new double[m];
			// Перенос данных в новый блок-строку
			memmove(newDataRow, data[i], sizeof(double) * min_m);
			// Удаление старого блока строки на этом месте
			delete[] data[i];
			// Прикрепление нового блока-строки на старое место
			data[i] = newDataRow;
		}
		// Сохранение нового размера
		this->m = m;
	}
	// Если кол-во строк не совпадает
	if (this->n != n) {
		// Создание нового блока-контейнера
		double** newData = new double* [n];
		// Перенос содержимого старого контейнера в новый
		memmove(newData, data, sizeof(double*) * min_n);
		// Удаление лишних строк из старого контейнера
		for (int i = n; i < this->n; i++) { delete[] data[i]; }
		// Удаление старого контейнера
		if (data) { delete[] data; }
		// Создание недостающих строк в новом контейнере
		for (int i = this->n; i < n; i++) { newData[i] = new double[m]; }
		// Привязка старого контейнера к новому
		data = newData;
		this->n = n;
	}
}
// Оператор сложения матриц
TMatrix TMatrix::operator + (const TMatrix& arg) const {
#ifdef _DEBUG
	if ((n != arg.n) || (m != arg.m))
		throw 1;
#endif
	TMatrix M(n, m);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			M(i, j) = data[i][j] + arg(i, j);
	return M;
}
// Производящая функция для формирования единичной матрицы
TMatrix TMatrix::E(int n) {
	TMatrix E(n, n);
	for (int i = 0; i < n; i++) {
		E(i, i) = 1;
		for (int j = i + 1; j < n; j++) { E(i, j) = E(j, i) = 0; }
	}
	return E;
}
// Оператор - унарный минус
TMatrix TMatrix::operator - () const {
	TMatrix M1(n, m);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			M1(i, j) = -data[i][j];
	return M1;
}
// Оператор вычитания матриц
TMatrix TMatrix::operator - (const TMatrix& arg) const {
#ifdef _DEBUG
	if ((n != arg.n) || (m != arg.m))
		throw 1;
#endif
	TMatrix M2(n, m);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			M2(i, j) = data[i][j] - arg(i, j);
	return M2;
}
// Функция транспонирования матриц
TMatrix TMatrix::t() const {
	TMatrix M3(m, n);
	TMatrix M3help(n, m);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			M3help(i, j) = data[i][j];
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			M3(i, j) = M3help(j, i);
		}
	}
	return M3;
}
// Оператор умножения матрицы на число
TMatrix TMatrix::operator * (double arg) const {
	TMatrix M4(n, m);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			M4(i, j) = data[i][j] * arg;
	return M4;
}
// Оператор умножения матриц
TMatrix TMatrix::operator * (const TMatrix& arg) const {
#ifdef _DEBUG
	if ((n != arg.n) || (m != arg.m))
		throw 1;
#endif
	TMatrix M5(m, n);

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			M5(i, j) = 0;
			for (int k = 0; k < m; k++)
				M5(i, j) += data[i][k] * arg(k, j);
		}
	}
	return M5;
}
// Функция перестановки строк
TMatrix TMatrix::swapRows(int i, int j, const TMatrix& arg) {
#ifdef _DEBUG
	if ((n != arg.n) || (m != arg.m))
		throw 1;
#endif
	TMatrix M6(n, m);
	double temp;
	for (int k = 0; k < n; k++)
		for (int l = 0; l < m; l++)
			M6(k, l) = arg(k, l);
	for (int k = 0; k < n; k++)
		for (int l = 0; l < m; l++)
			if (k == i)
			{
				temp = M6(i, l);
				M6(i, l) = M6(j, l);
				M6(j, l) = temp;
			}
	return M6;
}
//Функция вычисления детерминанта
double TMatrix::det() const {
#ifdef _DEBUG
	if ((n != m))
		throw 1;
#endif
	TMatrix M7(n, m);
	double det = 1;
	double tmp;
	double chet = 0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++) {
			M7(i, j) = data[i][j];
		}
	for (int k = 0; k < n - 1; k++) {
		for (int i = k + 1; i < n; i++) {
			if (M7(k, k) == 0) {
				for (int t = k + 1; t < n; t++)
				{
					if (M7(k, t) != 0) {
						M7 = M7.swapRows(k, t, M7);
						chet++;
						break;
					}
				}
			}
			tmp = -M7(i, k) / M7(k, k);
			for (int j = 0; j < n; j++) {
				M7(i, j) += M7(k, j) * tmp;
			}
		}
	}
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
		{
			if (j == i && i == 0)
				det = M7(i, j);
			if (j == i && i != 0)
				det *= M7(i, j);
		}
	det *= pow(-1, chet);
	return det;
}
// Оператор обращения матриц (метод Гаусса)
TMatrix TMatrix::appeal() const {
	TMatrix M8(n, m);
	TMatrix M9(n, m);
	int matrix1 = 0;
	int matrix2 = 0;
	double temp;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++) {
			M8(i, j) = data[i][j];
			M9(i, j) = data[i][j];
		}
	M9 = M9.E(n);
	for (int k = 0; k < n; k++)
	{
		//temp = M8(k, k);
		if (M8(k, k) == 0) {
			for (int t = k + 1; t < n; t++)
			{
				if (M8(t, k) != 0) {
					M8 = M8.swapRows(k, t, M8);
					matrix1 = k;
					matrix2 = t;
					break;
				}
			}
		}
		temp = M8(k, k);
		for (int j = 0; j < m; j++)
		{
			M8(k, j) /= temp;
			M9(k, j) /= temp;
		}
		for (int i = k + 1; i < n; i++)
		{
			temp = M8(i, k);
			for (int j = 0; j < n; j++)
			{
				M8(i, j) -= M8(k, j) * temp;
				M9(i, j) -= M9(k, j) * temp;
			}
		}
		for (int s = n - 1; s > 0; s--)
		{
			for (int i = s - 1; i >= 0; i--)
			{
				temp = M8(i, s);
				for (int j = 0; j < n; j++)
				{
					M8(i, j) -= M8(s, j) * temp;
					M9(i, j) -= M9(s, j) * temp;
				}
			}
		}
	}
	/*for (int k = n - 1; k > 0; k--)
	{
		for (int i = k - 1; i >= 0; i--)
		{
			temp = M8(i, k);
			for (int j = 0; j < n; j++)
			{
				M8(i, j) -= M8(k, j) * temp;
				M9(i, j) -= M9(k, j) * temp;
			}
		}
	}*/
	return M9;
}
//Оператор умножения матрицы на вектор
TVector TMatrix::operator * (const TVector& arg) const {
#ifdef _DEBUG
	if ((m != arg.size()))
		throw 1;
#endif
	TVector V7(n);
	V7[0] = 0; V7[1] = 0; V7[2] = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			V7[i] += data[i][j] * arg[j];
		}
	}
	return V7;
}
//Дружественная функция - оператор умножения числа на матрицу
TMatrix operator * (double lvalue, const TMatrix& rvalue) {
	TMatrix M10(rvalue.n, rvalue.m);
	for (int i = 0; i < rvalue.n; i++) {
		for (int j = 0; j < rvalue.m; j++) {
			M10(i, j) = lvalue * rvalue.data[i][j];
		}
	}
	return M10;
}
// Функция вывода матрицы
void TMatrix::print() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			std::cout << data[i][j] << " ";
		}
		std::cout << std::endl;
	}

}
// Функция добавления сирок в матрицу
void TMatrix::push(const TVector& arg) {

	TMatrix temp(n + 1, arg.size());
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < arg.size(); j++)
		{
			temp.data[i][j] = data[i][j];
		}
	}
	for (int i = 0; i < arg.size(); i++)
	{
		temp.data[n][i] = arg[i];
	}
	(*this) = temp;
}
void TMatrix::print_col(int number) {
	for (int i = 0; i < n; i++)
		std::cout << data[i][number] << std::endl;
}

void TMatrix::go_to_file(std::string name)
{
	std::ofstream f(name);

	//if (!f.is_open())
	//{
	//	exit(1);
	//}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
			f << data[i][j] << " ";
		f << std::endl;
	}
	f.close();
}
