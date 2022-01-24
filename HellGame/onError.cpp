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

void mav(HWND hwnd) // 发生啥事，我练功发自真心
{
	// 希望永远不会跑到到这里...
	MessageBox(hwnd, L"Memory Access Violate", L"ERROR",MB_OK | MB_SYSTEMMODAL);
	exit(-999);
}

string getTime(void) // 获取大概时间（字符串）
{
	time_t tt = time(NULL);
	struct tm* t = localtime(&tt);
	return to_string(t->tm_hour) + ":" + to_string(t->tm_min) + " " + to_string(t->tm_sec) + "\'";
}

void error(string logFilePath, string whatthehell, int stopCode) // 输出错误信息
{
	log(logFilePath, whatthehell, 1);
	Sleep(200);
	exit(stopCode);
}

string logInit(void) // 创建日志文件；否则下文日志记录"log()"将不做任何动作
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
	if (!opstream.is_open()) // 压根没有日志文件(禁用了日志或没有权限)
	{
		return;
	}
	if (isErrorInfo) // 日志类型：错误/日志
	{
		opstream << "[STDERR]";
	}
	else
	{
		opstream << "[STDINFO]";
	}
	opstream << "["+getTime()+"]"; // 时间
	opstream << whatsup <<endl; // 内容

	opstream << flush; // 关闭文件
	opstream.close();
	return;
}
