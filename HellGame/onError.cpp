#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <cstring>

#include <fstream>

#include <Windows.h>

using namespace std;

void log(string,string,bool);

void mav(HWND hwnd) // ����ɶ�£���������������
{
	// ϣ����Զ�����ܵ�������...
	MessageBox(hwnd, L"Memory Access Violate", L"ERROR",MB_OK | MB_SYSTEMMODAL);
	exit(-999);
}

string getTime(void) // ��ȡ���ʱ�䣨�ַ�����
{
	time_t tt = time(NULL);
	struct tm* t = localtime(&tt);
	return to_string(t->tm_hour) + ":" + to_string(t->tm_min) + " " + to_string(t->tm_sec) + "\'";
}

void error(string logFilePath, string whatthehell, int stopCode) // ���������Ϣ
{
	log(logFilePath, whatthehell, 1);
	Sleep(200);
	exit(stopCode);
}

string logInit(void) // ������־�ļ�������������־��¼"log()"�������κζ���
{
	time_t tt = time(NULL);
	struct tm* t = localtime(&tt);
	fstream create;
	string logFilePath = ".\\logs\\log_" + to_string(t->tm_year + 1900) + "-" + to_string(t->tm_mon + 1) + "-" + to_string(t->tm_mday) + "_" + to_string(t->tm_hour) + "_" + to_string(t->tm_min) + "_" + to_string(t->tm_sec) + ".log";
	create.open(logFilePath,ios::app);
	create << flush;
	create.close();
	log(logFilePath, "Created log file successfully.", 0);
	return logFilePath;
}

void log(string logFilePath, string whatsup, bool isErrorInfo)
{
	fstream opstream;
	opstream.open(logFilePath, ios::app);
	if (!opstream.is_open()) // ѹ��û����־�ļ�(��������־��û��Ȩ��)
	{
		return;
	}
	if (isErrorInfo) // ��־���ͣ�����/��־
	{
		opstream << "[STDERR]";
	}
	else
	{
		opstream << "[STDINFO]";
	}
	opstream << "["+getTime()+"]"; // ʱ��
	opstream << whatsup <<endl; // ����

	opstream << flush; // �ر��ļ�
	opstream.close();
	return;
}
