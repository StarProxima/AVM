#include "iostream"
#include "fstream"
#include "vector"
#include "string"
#include <Windows.h> 
#include <stdlib.h>

using namespace std;
#define PI 3.14159265358979323846
double a = -1, b = 1;
double Lagrange(double x, double i, vector<pair<double, double>>* p)
{
	double k = 1;
	for (double j = 0; j < p->size(); j++)
	{
		if (j != i)
			k *= (x - (*p)[j].first) / ((*p)[i].first - (*p)[j].first);
	}
	return k;
}

double L(double x, vector<pair<double, double>>* p)
{
	double el = 0;
	int n = p->size();
	for (double i = 0; i < n; i++)
	{
		el += (*p)[i].second * Lagrange(x, i, p);
	}
	return el;
}

double f(double x)
{
	return pow(2, x);
	//return pow(2.71, -2 * x * x);
	//return abs(x);
	//return 1/(1+25*x*x);
	//return x*x;
}

void PrintGraph(double x, vector<pair<double, double>>* p)
{
	HDC hDC = GetDC(GetConsoleWindow());
	HPEN Pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	HPEN Sen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	SelectObject(hDC, Pen);
	MoveToEx(hDC, 0, 850, NULL);
	LineTo(hDC, 2000, 850);
	MoveToEx(hDC, 1000, 0, NULL);
	LineTo(hDC, 1000, 1700);
	MoveToEx(hDC, 100 * a + 1000, -100 * L(a, p) + 850, NULL);
	for (x = a; x <= b; x += 0.1)
	{
		double k = -100 * L(x,p) + 850;
		LineTo(hDC, 100 * x + 1000, k);
	}
	SelectObject(hDC, Sen);
	MoveToEx(hDC, 100 * a + 1000, -100 * f(a) + 850, NULL);
	for (x = a; x <= b; x += 0.01)
	{
		double k = -100 * f(x) + 850;
		LineTo(hDC, 100 * x + 1000, k);
	}
}

int main()
{

	setlocale(LC_ALL, "Russian");
	cout << "Введите 0 для равнораспределённых узлов, 1 для чебышевских узлов." << endl;
	int k;
	double n, h, x;

	cin >> k;
	vector<pair<double, double>> points;
	cout << "Введите левую и правую границу." << endl;
	cin >> a >> b;

	while (true)
	{
		cout << "Введите количество точек разбиения и Х для расчёта приблизительного значения." << endl;
		cin >> n >> x;
		points.clear();
		system("cls");
		for (double i = 0; i < n; i++)
		{
			if (k == 0)
			{
				double h = (b - a) / n;
				points.push_back(make_pair(a + i * h, f(a + i * h)));
			}
			else if (k == 1)
			{
				double x = (b + a) / 2 + (b - a) / 2 * cos((2 * i + 1) * PI / (2 * n + 2));  // узлы Чебышева
				points.push_back(make_pair(x, f(x)));
			}
		}
		cout << L(x, &points) << endl;
		PrintGraph(a, &points);
	}
	
}

