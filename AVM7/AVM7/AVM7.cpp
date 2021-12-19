#include <stdio.h>
#include <math.h>
#include <iostream>
#include <locale.h>
#include <cstdlib>

using namespace std;
double f(double x)
{
    return  x * x + 8 * x - 65;
}

double Segment_half(double a, double b, double (*f)(double), double eps)
{
    double c; int i = 0;

    double fa = f(a), fb = f(b);

    //cout << fa << " " << fb << endl;
    if (fa * fb > 0) { cout << "Нельзя применить метод\n"; return 0; }

    for (; fabs(b - a) >= eps; i++)
    {
        c = (a + b) / 2.0;
        //cout << a << " " << b << endl;
        double fc = f(c);
        if (fc == 0)
        {
            cout << "Количество итераций: " << i << endl;
            return c;
        }

        if (fc * fa > 0) { a = c; }
        else { b = c; }
    }
    cout << "Количество итераций: " << i << endl;
    return c;
}

double Secant(double a, double b, double (*f)(double), double eps)
{
    int i = 0;
    while (fabs(b - a) > eps) {
        //cout << a << " " << b << endl;
        a = b - (b - a) * f(b) / (f(b) - f(a));   //  (i - 1)-й элемент
        
        b = a - (a - b) * f(a) / (f(a) - f(b));   //  i-й элемент
        i++;
    }

    cout << "Количество итераций: " << i << endl;
    return b;
}

double Newton(double (*f)(double), double x0, double eps, int n = 40) {
    double x = x0, x2 = -x0, df, h = eps;
   //df = ;
    int i = 0;

    while (fabs(x - x2) > eps)
    {
        x2 = x;
        //cout << x << endl;
        x = x - f(x) /( (f(x + h) - f(x - h)) / (2 * h));
        
        i++;
    }
        
    cout << "Количество итераций: " << i << endl;
    return x;
}

double Iter(double (*f)(double), double eps, double x0) {

    int i = 0;
    double x = x0;
    x = f(x);
    while (abs(x - f(x)) > eps)
    {
        //cout << x << endl;
        x = f(x);
        i++;
    }
    cout << "Количество итераций: " << i << endl;
    return x;
 
}



int main(void)
{
    setlocale(LC_ALL, "Russian");
    system("chcp 1251");
    double a = -10.0, b = 10.0;
    double x0 =1.0;

    cout << "\n";
    cout << ("Метод деления отрезка пополам. ") << endl; 
    cout << "Ответ: " << Segment_half(a, b, f, 1e-5) << endl << endl;
    cout << ("Метод секущих. ") << endl;
    cout << "Ответ: " << Secant(a, b, f, 1e-5) << endl << endl;
    cout << ("Метод Ньютона. ") << endl;
    cout << "Ответ: " << Newton(f, x0, 1e-5) << endl << endl;
    cout << ("Метод простой итерации. ") << endl;
    cout << "Ответ: " << Iter(f, 1e-10, x0) << endl << endl;
}