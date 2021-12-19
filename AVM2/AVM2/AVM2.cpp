#include <iostream>
#include <iomanip>
#include<cmath> 
#include <vector>
#include <functional>
using namespace std;

// Подынтегральная функция

double f(long double x) {
    //return x*x*x - 12*x + 13;
    //return sin(x);
    return x*x;
}

double Rectangle1(double a, double b, int n) {
    double result = 0;
    double width = (b - a) / n;

    for (int i = 0; i < n; i++) {
        result += width *f(a + width*i);
    }


    return result;
}
double Rectangle2(double a, double b, int n) {
    double result = 0;
    double width = (b - a) / n;

    for (int i = 0; i < n; i++) {
        result += width * f(a + width * i + width);
    }

    return result;
}
double Rectangle3(double a, double b, int n) {
    double result = 0;
    double width = (b - a) / n;

    for (int i = 0; i < n; i++) {
        result += width * f(a + width * i + width /2);
    }

    return result;
}


double Trap(double a, double b, int n) {
    double result = 0;
    double width = (b - a) / n;
    for (int step = 0; step < n; step++) {
        double x1 = a + step * width;
        double x2 = a + (step + 1) * width;

        result += (x2 - x1) * (f(x1) + f(x2))/2.0;
    }

    return result;
}

double Simpson(double a, double b, int n) {
    double width = (b - a) / n;
    double result = f(a) + f(b), k;

    for (int i = 1; i < n; i++) {
        k = a + i * width;

        
        result+= i%2 == 0 ? 2 * (f(k)) : 4 * (f(k));
        
    }

    return result * width / 3;
}
double Simpson3_8(double a, double b, int n) {
    double width = (b - a) / n, k;
    double result = f(a) + f(b);

    for (int i = 1; i < n; i++)
    {
        k = a + i* width;
        result += i % 3 == 0 ? 2 * (f(k)) : 3 * (f(k));
    }
   
    return result * width * 3.0 / 8.0;
}

double Runge(double a, double b, int n, function<double(double, double, int)> fun) {
    double vx1 = fun(a, b, n), vx2 = fun(a, b, n * 2);
    n = n * 2;
    while (abs((vx2 - vx1)) * (1.0 / 3.0) > (1e-5)) {
        vx1 = fun(a, b, n);
        vx2 = fun(a, b, n * 2);
        n = n * 2;
    }
    return vx2;
}


int main(int argc, const char* argv[]) {
    setlocale(LC_ALL, "Russian");
    double u = 0;
    double b = 4;
    int n = 100;

    cout << "Метод левых прямоугольников:   " << Rectangle1(u, b, n) << endl;
    cout << "Метод правых прямоугольников:   " << Rectangle2(u, b, n) << endl;
    cout << "Метод центральных прямоугольников:   " << Rectangle3(u, b, n) << endl;
    cout << "Метод трапеций:   " << Trap(u, b, n) << endl;
    cout << "Метод Симпсона:   " << Simpson(u, b, n) << endl;
    cout << "Метод Симпсона 3/8:   " << Simpson3_8(u, b, n) << endl << endl;
    cout << "Метод прямоугольников с правилом Рунге:   " << Runge(u, b, n, Rectangle3) << endl;
    cout << "Метод Cимпсона с правилом Рунге:   " << Runge(u, b, n, Simpson) << endl;
    cout << "Метод Cимпсона 3/8 с правилом Рунге:   " << Runge(u, b, n, Simpson3_8) << endl;
    /*cout << "Метод прямоугольников с правилом Рунге:   " << runge_PRM(u, b, n) << endl;
    cout << "Метод Cимпсона с правилом Рунге:   " << runge_SMP(n, u, b) << endl;
    cout << "Метод Cимпсона 3/8 с правилом Рунге:   " << runge_SMP3_8(n, u, b) << endl;*/
}