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
 * config文件格式:
 * 注释每行以井号"#"开头，下一行不受影响，没有续行符
 * 正文为 NAME=VALUE 格式:
 *	BOOL: NAME=true | NAME=false
 *	INT:  NAME=VALUE(%d)
 *  STRING: NAME="VALUE"
*/

bool readConfigBool(string filePath, string name) // 输入config文件， 目标的值为布尔类型
{
	/// 下面readConfigInt, readConfigStr的过程差不多，不多列注释.

	// 打开文件
	ifstream input;
	input.open(filePath);
	if (!input.is_open())
	{
		cerr << "Failed open config file :\"" << filePath<<"\" !";
		exit(-2);
	}

	// 开始一行一行读入
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
		if (temp[0] == '#') // 跳过注释
		{
			continue;
		}
		for (; i < temp.length(); i++) // 获取名字
		{
			if (temp[i] == '=')
			{
				i++;
				break;
			}
			nodeName += temp[i];
		}
		if (nodeName != name) // 名字与目标相同?
		{
			continue;
		}

		

		if (temp[i] == 't' || temp[i] == 'T') // 获取名字对应的值
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

int readConfigInt(string filePath, string name) // 输入config文件， 目标的值为整型
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

string readConfigStr(string filePath, string name) // 输入config文件， 目标的值为字符串
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
