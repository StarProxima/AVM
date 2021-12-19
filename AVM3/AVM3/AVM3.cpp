#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
int main()
{
	ifstream f("D://matrix.txt");
	string temp;
	vector<vector<int>> v;
	vector<int> v2;
	if (f.is_open())
	{
		string str;
		while (getline(f, str))
		{
			str += " ";
			temp = "";
			for (int i = 0; i < str.size(); i++)
			{
				if (str[i] == ' ')
				{

					v2.push_back(stoi(temp));
					temp = "";
				}
				else temp += str[i];
			}
			v.push_back(v2);
			v2.clear();
		}
	}
	for (int i = 0; i < v.size(); i++)
	{
		for (int j = 0; j < v.size(); j++)
			cout << v[i][j] << " ";
		cout << "\n";
	}

}