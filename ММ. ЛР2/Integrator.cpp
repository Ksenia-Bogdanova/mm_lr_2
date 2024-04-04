//---------------------------------------------------------------------------


#pragma hdrstop
#include <iostream>
#include <math.h>
#include <algorithm>
#include "integrator.h"
#include "model.h"
#include "linalg.h"
using namespace std;
//===========================================================================
// class TDormandPrinceIntegrator

const long double TDormandPrinceIntegrator::c[7] = { 0, 1. / 5, 3. / 10, 4. / 5, 8. / 9, 1., 1. };
const long double TDormandPrinceIntegrator::a[7][6] = {
    { 0. },
    { 1. / 5 },
    { 3. / 40, 9. / 40 },
    { 44. / 45, -56. / 15, 32. / 9 },
    { 19372. / 6561, -25360. / 2187, 64448. / 6561, -212. / 729 },
    { 9017. / 3168, -355. / 33, 46732. / 5247, 49. / 176, -5103. / 18656 },
    { 35. / 384, 0., 500. / 1113, 125. / 192, -2187. / 6784, 11. / 84 }
};
const long double TDormandPrinceIntegrator::b1[7] = { 35. / 384, 0., 500. / 1113, 125. / 192, -2187. / 6784, 11. / 84, 0 };
const long double TDormandPrinceIntegrator::b2[7] = { 5179. / 57600, 0., 7571. / 16695, 393. / 640, -92097. / 339200, 187. / 2100, 1. / 40 };
//---------------------------------------------------------------------------

TDormandPrinceIntegrator::TDormandPrinceIntegrator()
    : TIntegrator()
{
    long double V = 1.0l;
    while (1.0l + V > 1.0l) {
        u = V;
        V = V / 2.0l;
    }
}

//---------------------------------------------------------------------------

long double TDormandPrinceIntegrator::Run(TModel* Model)
{
    long double // Это время для интегрирования (увеличивается на величину шага интегрирования)
        t = Model->getT0(),
        // Это время для выдачи (увеличивается дискретно на величину плотности)
        t_out = t,
        // Это конечное время
        t1 = Model->getT1(),
        // Это шаг интегрирования
        h,
        // Это шаг после коррекции (инициализируется плотностью выдачи результата)
        h_new = Model->getSamplingIncrement(),
        // Это ошибка на шаге интегрирования
        e = 0;

    TVector // Это вектор состояния на начало шага
        X = Model->getInitialConditions(),
        // Это вектор состояния на конец шага (решение 4-го порядка)
        X1(X.size()),
        // Это вектор состояния на конец шага для коррекции величины шага (решение 5-го порядка)
        X2(X.size()),
        // Это вектор для выдачи результата
        Xout(X.size()),
        // Это буфер для вычисления коэффициентов К
        Y(X.length());

    // Подготовка хранилища результатов в модели для повышения эффективности выделения памяти
    Model->TModelprepareResult();

    /*fstream f("f.txt");
    if (!f.is_open())
    {
        exit(1);
    }*/
    // Инициализируем размерности коэффициентов K[j]
    for (int i = 6; i > 0; --i)
        K[i].resize(X.size());

    // Счётчик количества сделанных шагов
    int N = 0;
    int n = 0;
    // Главный цикл
    while (t < t1)
    {
        // Устанавливаем шаг на итерацию
        h = h_new;

        // Вычисляем коэффициенты К

        K[0] = Model->getRight(X, t);
        K[1] = Model->getRight(X + h * a[1][0] * K[0], t + c[1] * h);
        K[2] = Model->getRight(X + h * (a[2][0] * K[0] + a[2][1] * K[1]), t + c[2] * h);
        K[3] = Model->getRight(X + h * (a[3][0] * K[0] + a[3][1] * K[1] + a[3][2] * K[2]), t + c[3] * h);
        K[4] = Model->getRight(X + h * (a[4][0] * K[0] + a[4][1] * K[1] + a[4][2] * K[2] + a[4][3] * K[3]), t + c[4] * h);
        K[5] = Model->getRight(X + h * (a[5][0] * K[0] + a[5][1] * K[1] + a[5][2] * K[2] + a[5][3] * K[3] + a[5][4] * K[4]), t + c[5] * h);
        K[6] = Model->getRight(X + h * (a[6][0] * K[0] + a[6][1] * K[1] + a[6][2] * K[2] + a[6][3] * K[3] + a[6][4] * K[4] + a[6][5] * K[5]), t + c[6] * h);

        // Вычисляем решения 4-го и 5-го порядков
        X1 = X + h * (b1[0] * K[0] + b1[1] * K[1] + b1[2] * K[2] + b1[3] * K[3] + b1[4] * K[4] + b1[5] * K[5]);
        X2 = X + h * (b2[0] * K[0] + b2[1] * K[1] + b2[2] * K[2] + b2[3] * K[3] + b2[4] * K[4] + b2[5] * K[5] + b2[6] * K[6]);

        // Вычисляем значение локальной ошибки
        long double new_eps{};

        for (int count = 0; count < X.size(); ++count) {
            long double max_ = std::max((long double)(std::max(0.00001, abs(X[count]))), std::max((long double)(abs(X1[count])), 2.0l * u / Eps));
            new_eps += pow(h * (X1[count] - X2[count]) / max_, 2);
        }
        new_eps /= (X.size());
        e = sqrt(new_eps);

        // Вычисляем новое значение шага
        h_new = h / std::max((long double)0.1, std::min((long double)5.0, pow(e / Eps, 0.2) / (long double)0.9));

        // Если локальная ошибка превышает установленную величину, пытаемся сделать шаг заново
        if (e > Eps)
            continue;

        // Формирование результатов при помощи механизма плотной выдачи
        while ((t_out < t + h) && (t_out <= t1))
        {
            long double theta = (t_out - t) / h;
            long double b[6]{};
            b[0] = theta * (1 + theta * (-1337.0l / 480.0l + theta * (1039.0l / 360.0l + theta * (-1163.0l / 1152.0l))));
            b[1] = 0.0l;
            b[2] = 100.0l * pow(theta, 2) * (1054.0l / 9275.0l + theta * (-4682.0l / 27825.0l + theta * (379.0l / 5565.0l))) / 3.0l;
            b[3] = -5.0l * pow(theta, 2) * (27.0l / 40.0l + theta * (-9.0l / 5.0l + theta * (83.0l / 96.0l))) / 2.0l;
            b[4] = 18225.0l * pow(theta, 2) * (-3.0l / 250.0l + theta * (22.0l / 375.0l + theta * (-37.0l / 600.0l))) / 848.0l;
            b[5] = -22.0l * pow(theta, 2) * (-3.0l / 10.0l + theta * (29.0l / 30.0l + theta * (-17.0l / 24.0l))) / 7.0l;

            // Получаем результат для выдачи
            Xout = X + h * (b[0] * K[0] + b[1] * K[1] + b[2] * K[2] + b[3] * K[3] + b[4] * K[4] + b[5] * K[5]);
            //Xout[2] = t_out;
            // Передача результата в модель
            /*for (int i = 0; i < Xout.size(); i++) {
                f << Xout[i] << " ";
            }*/
            /*
            n++;
            std::cout << " " << n;*/
           /* f << std::endl;
            n++;*/

            Model->addResult(Xout, t_out);
            // Наращиваем время выдачи
            t_out += Model->getSamplingIncrement();
        }

        // Обновляем X решением 5-го порядка и наращиваем время на величину сделанного шага
        X = X2;
        t += h;

        // Считаем количество итераций для вычисления глобальной погрешности
        N++;
    }
    //f.close();
    return 0;
}


