#define _CRT_SECURE_NO_WARNINGS true

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cstring>
#include <sstream>
#include <fstream>

#include <Windows.h>

using namespace std;

/*
 * config�ļ���ʽ:
 * ע��ÿ���Ծ���"#"��ͷ����һ�в���Ӱ�죬û�����з�
 * ����Ϊ NAME=VALUE ��ʽ:
 *	BOOL: NAME=true | NAME=false
 *	INT:  NAME=VALUE(%d)
 *  STRING: NAME="VALUE"
*/

bool readConfigBool(string filePath, string name) // ����config�ļ��� Ŀ���ֵΪ��������
{
	/// ����readConfigInt, readConfigStr�Ĺ��̲�࣬������ע��.

	// ���ļ�
	ifstream input;
	input.open(filePath);
	if (!input.is_open())
	{
		cerr << "Failed open config file :\"" << filePath<<"\" !";
		exit(-2);
	}

	// ��ʼһ��һ�ж���
	string temp;
	char tmp[1001] = { "\0" };
	string nodeName;
	bool nodeValue = 0;
	size_t i;
	while (!input.eof())
	{
		nodeName = "";
		i = 0;
		input.getline(tmp,sizeof(tmp));
		temp = tmp;
		if (temp[0] == '#') // ����ע��
		{
			continue;
		}
		for (; i < temp.length(); i++) // ��ȡ����
		{
			if (temp[i] == '=')
			{
				i++;
				break;
			}
			nodeName += temp[i];
		}
		if (nodeName != name) // ������Ŀ����ͬ?
		{
			continue;
		}

		

		if (temp[i] == 't' || temp[i] == 'T') // ��ȡ���ֶ�Ӧ��ֵ
		{
			return 1;
		}
		else
		{
			return 0;
		}
		break;
	}
	return 0;
}

int readConfigInt(string filePath, string name) // ����config�ļ��� Ŀ���ֵΪ����
{
	ifstream input;
	int result = 0;
	input.open(filePath);
	if (!input.is_open())
	{
		cerr << "Failed open config file :\"" << filePath << "\" !";
		exit(-2);
	}
	string temp;
	char tmp[1001] = { "\0" };
	string nodeName;
	bool nodeValue = 0;
	size_t i;
	while (!input.eof())
	{
		nodeName = "";
		i = 0;
		input.getline(tmp, sizeof(tmp));
		temp = tmp;
		if (temp[0] == '#')
		{
			continue;
		}
		for (; i < temp.length(); i++)
		{
			if (temp[i] == '=')
			{
				i++;
				break;
			}
			nodeName += temp[i];
		}
		if (nodeName != name)
		{
			continue;
		}

		for (; i < temp.length(); i++)
		{
			result *= 10;
			result += temp[i] - 48;
		}
		break;
	}

	return result;
}

string readConfigStr(string filePath, string name) // ����config�ļ��� Ŀ���ֵΪ�ַ���
{
	ifstream input;
	string result;
	input.open(filePath);
	if (!input.is_open())
	{
		cerr << "Failed open config file :\"" << filePath << "\" !";
		exit(-2);
	}
	string temp;
	char tmp[1001] = { "\0" };
	string nodeName;
	bool nodeValue = 0;
	size_t i;
	while (!input.eof())
	{
		nodeName = "";
		i = 0;
		input.getline(tmp, sizeof(tmp));
		temp = tmp;
		if (temp[0] == '#')
		{
			continue;
		}
		for (; i < temp.length(); i++)
		{
			if (temp[i] == '=' && temp[i + 1] == '\"')
			{
				i+=2;
				break;
			}
			nodeName += temp[i];
		}
		if (nodeName != name)
		{
			continue;
		}

		for (; i < temp.length(); i++)
		{
			if (temp[i] != '\"')
			{
				result += temp[i];
			}
			else
			{
				break;
			}
		}
		break;
	}
	return result;
}
