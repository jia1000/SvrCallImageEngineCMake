///

#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

inline vector<string> ReadTxt(const char *path)
{
	ifstream in(path);
	string filename;
	string line;
	vector<string> result;

	if (in) // �и��ļ�
	{
		while (getline(in, line)) // line�в�����ÿ�еĻ��з�
		{
			//cout << line << endl;
			result.push_back(line);
		}
	}
	else // û�и��ļ�
	{
		cout << "no such file" << endl;
	}
	return result;
}