#include <iostream>
#include <math.h>
#include<fstream>
#include <vector>
#include <string>
using namespace std;



bool IsSymmetryMatrix(int size, vector<vector<double>> A)
{
	int i, j;
	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
			if ((i != j) && (A[i][j] != A[j][i]))
				return 0;

	return 1;
}



void PrintMatrix(int n, vector<vector<double>> A)
{
	cout << "Матрица: " << endl;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			cout << A[i][j] << " ";
		cout << endl;
	}
	cout << endl;
}


int RotationMethod(int size, vector<vector<double>>& A, double eps)
{
	vector<vector<double>> v;
	v.assign(size, vector<double>(size));
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			v[i][j] = 0;
		}
		v[i][i] = 1;
	}

	int k = 0;
	int maxI, maxJ;
	double max, fi;
	vector<vector<double>> matrRotation;
	matrRotation.assign(size, vector<double>(size));
	vector<vector<double>> temp;
	temp.assign(size, vector<double>(size));

	double fault = 0.0;
	for (int i = 0; i < size; i++)
		for (int j = i + 1; j < size; j++)
			fault = fault + A[i][j] * A[i][j];
	fault = sqrt(2 * fault);

	while (fault > eps)
	{
		max = 0.0;
		for (int i = 0; i < size; i++)
			for (int j = i + 1; j < size; j++)
				if (A[i][j] > 0 && A[i][j] > max)
				{
					max = A[i][j];
					maxI = i;
					maxJ = j;
				}
				else if (A[i][j] < 0 && -A[i][j] > max)
				{
					max = -A[i][j];
					maxI = i;
					maxJ = j;
				}

		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
				matrRotation[i][j] = 0;
			matrRotation[i][i] = 1;
		}

		if (A[maxI][maxI] == A[maxJ][maxJ])
		{
			matrRotation[maxI][maxI] = matrRotation[maxJ][maxJ] =
				matrRotation[maxJ][maxI] = sqrt(2.0) / 2.0;
			matrRotation[maxI][maxJ] = -sqrt(2.0) / 2.0;
		}
		else
		{
			fi = 0.5 * atan((2.0 * A[maxI][maxJ]) /
				(A[maxI][maxI] - A[maxJ][maxJ]));
			matrRotation[maxI][maxI] = matrRotation[maxJ][maxJ] = cos(fi);
			matrRotation[maxI][maxJ] = -sin(fi);
			matrRotation[maxJ][maxI] = sin(fi);
		}

		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				temp[i][j] = 0.0;
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				for (int k = 0; k < size; k++)
					temp[i][j] += matrRotation[k][i] * A[k][j];

		//вычисляем собственные значения
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				A[i][j] = 0.0;
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				for (int k = 0; k < size; k++)
					A[i][j] += temp[i][k] * matrRotation[k][j];

		//снова выделяем максимальный элемент
		fault = 0.0;
		for (int i = 0; i < size; i++)
			for (int j = i + 1; j < size; j++)
				fault = fault + A[i][j] * A[i][j];
		fault = sqrt(2 * fault);

		
		k++;
	}

	return k;
}

int IterMethod(vector<vector<double>>& A, vector<double>& a, double& L, int n, double eps)
{
	float w0[100], w[100], summ = 0, h[100], e, d, d0;
	int i, j, k = 0;
	w0[0] = 1;
	for (i = 1; i < n; i++)
		w0[i] = 0;


	do
	{
		for (i = 0; i < n; i++)
			summ = summ + w0[i] * w0[i];
		d0 = sqrt(summ);
		for (i = 0; i < n; i++)
			h[i] = w0[i] / d0;
		for (i = 0; i < n; i++)
		{
			w[i] = 0;
			for (j = 0; j < n; j++)
				w[i] = w[i] + A[i][j] * h[j];
		}
		summ = 0;
		for (i = 0; i < n; i++)
			summ = summ + w[i] * w[i];
		d = sqrt(summ);
		e = fabs(d - d0);
		for (i = 0; i < n; i++)
			w0[i] = w[i];
		summ = 0;
		k++;
	} while (e > eps);

	
	for (i = 0; i < n; i++)
	{	
		a.push_back(h[i]);
	}
	L = d;
	return k;
}

void main()
{
	setlocale(LC_ALL, "RUS");
	string tmp;
	vector<vector<double>> matrixOriginal;
	vector<vector<double>> matrix;
	vector<double> elem;
	double eps = 0.0001;
	
	ifstream matr_f("Matr.txt");
	if (matr_f.is_open())
	{
		string line;
		while (getline(matr_f, line))
		{
			line += " ";
			tmp = "";
			for (int i = 0; i < line.size(); i++)
			{
				if (line[i] == ' ')
				{
					elem.push_back(stod(tmp));
					tmp = "";
				}
				else tmp += line[i];
			}
			matrixOriginal.push_back(elem);
			elem.clear();
		}
	}
	matr_f.close();
	matrix = matrixOriginal;
	int size = matrix.size();
	PrintMatrix(size, matrix);




	

	
	

	cout << "\nМетод вращений";
	if (IsSymmetryMatrix(size, matrix))
	{
		int iters_count = RotationMethod(size, matrix, eps);
		cout << endl << endl;
		
		cout << "Собственные значения: " << endl;
		for (int i = 0; i < size; i++)
		{
			cout << matrix[i][i] << endl;
		}
		cout << endl << "Число итераций: " << iters_count << endl;
		
	}
	else
	{
		cout << endl << "Несимметричная матрица." << endl;
	}


	

	cout << "\n\n\nМетод прямой итерации:\n";
	matrix = matrixOriginal;
	if (IsSymmetryMatrix(size, matrix))
	{
		vector<double> answer;
		double l = 0;
		int iters_count = IterMethod(matrix, answer, l, size, eps);

		cout << "\nСпектральный радиус: " << l << "\n";
		cout << "Собственный вектор:\n";
		for (int i = 0; i < size; i++)
			cout << answer[i] << endl;
		cout << endl << "Число итераций: " << iters_count << endl;
	}
	else
	{
		cout << endl << "Несимметричная матрица." << endl;
	}
}