#pragma once
//---------------------------------------------------------------------------

#ifndef modelH
#define modelH

// Подключение заголовочного файла библиотеки векторных и матричных операций
#include <fstream>
#include "linalg.h"

//---------------------------------------------------------------------------
// Базовый класс модели для интегратора

class TModel
{
protected:
    // Начальные условия
    TVector X0;
    // Требуемый от интегратора интервал выдачи результатов
    long double SamplingIncrement;
    // Начало и окончание времени интегрирования
    long double t0, t1;
    // Хранилище результатов
    TMatrix Result;
    // Счётчик строк в матрице результатов
    int N;

public:
    // Общий конструктор моделей - инициализация параметров по умолчанию
    TModel()
        : SamplingIncrement(1e-1)
        , t0(0)
        , t1(1)
        , N(0)
    {}
    // Абстрактная перегружаемая функция правых частей ДУ (X - вектор состояния, t - независимый аргумент)
    virtual TVector getRight(const TVector& X, long double t) = 0;
    // Получение начальных условий
    inline TVector getInitialConditions() const { return X0; }
    // Порядок системы - по размерности вектора начальных условий
    inline int getOrder() const { return X0.size(); }

    // Интервал выдачи результатов
    inline long double getSamplingIncrement() const { return SamplingIncrement; }

    // Управление временным интервалом интегрирования
    inline long double getT0() const { return t0; }
    inline long double getT1() const { return t1; }

    // Получение матрицы результатов
    inline TMatrix getResult() const { return Result; }
    // Запись результатов (в этом методе в наследнике допустимо организовать запись в файл 
    // вместо или вместе с наполнением матрицы результатов)
    virtual void addResult(const TVector& X, long double t);
    // Очистка матрицы результатов (или файла с результатами)
    virtual void clearResult();
    // Подготовка матрицы результатов для более эффективного выделения памяти
    virtual void TModelprepareResult();
};

//---------------------------------------------------------------------------

#endif
