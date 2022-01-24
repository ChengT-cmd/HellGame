#define _CRT_SECURE_NO_WARNINGS true

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cstring>
#include <fstream>

#include <Windows.h>

#include "main.h"
#include "musicCfg.h"
#include "key.h"

using namespace std;

int play(string, configMusicFile, string);
bool readConfigBool(string, string);
int readConfigInt(string, string);
string readConfigStr(string, string);
configMusicFile open(string, char*);

string logInit(void);
void error(string, string, int);
void log(string, string, bool);
string getTime(void);

void playbgm(string);
void stopbgm(string);

config cfg;
string choosedmusic;

string mainmenu(string, HWND);

//typedef HWND(WINAPI* PROCGETCONSOLEWINDOW)(); // 获取控制台句柄1
//PROCGETCONSOLEWINDOW GetConsoleWindow;

int main(int argc, char *argv[])
{
	//HMODULE hKernel32 = GetModuleHandle(L"kernel32"); // 获取控制台句柄2
	//GetConsoleWindow = (PROCGETCONSOLEWINDOW)GetProcAddress(hKernel32, "GetConsoleWindow");
	//HWND hwnd = GetConsoleWindow();
	HWND hwnd = FindWindow(L"ConsoleWindowClass", NULL);

	string errorinfo;
	string logger;
	char bgmpath[1001] = "\0";
	char menuTitle[1001] = "\0";
	char temp[1001] = "\0";

	system("mode con cols=120 lines=30"); // 标准化控制台大小

	// 读取设置
	cfg.enableLogs=readConfigBool("configs/settings.ini", "enablelogs");
	
	if (cfg.enableLogs) // 如果启用日志，则创建日志文件；否则下文日志记录"log()"将不做任何动作 
	{
		logger = logInit();
	}
	log(logger, "Program started.", 0); // 写入日志
	log(logger, "Gotten value of enablelogs from \"configs/settings.ini.\".", 0);

	// 检查控制台可用性
	cfg.enableConsole = readConfigBool("configs/settings.ini", "enableconsole");
	if (cfg.enableConsole)
	{
		log(logger, "The console has been enabled currently.", 0);
	}
	// 是否一定使用启动菜单
	cfg.enableMenuWhatever = readConfigBool("configs/settings.ini", "enablemenuwhatever");
	if (cfg.enableMenuWhatever)
	{
		log(logger, "The main menu has been locked currently.", 0);
	}



	if (!hwnd)
	{
		cerr << "Failed to get console handle, Ignore...\n";
		log(logger, "Failed to get console handle, Ignore.", 0);
	}

	int cmdline = false;
	if (argc > 1)
	{
		cmdline = true;
	}

	// 获取控制台窗口标题
	strcpy(menuTitle, "title ");
	strcat(menuTitle, readConfigStr("configs/settings.ini", "customtitle").c_str());

	// 播 放 器
	int score;
	while (true)
	{
		system(menuTitle);

		score = 0;
		// 有命令行参数且未一定使用启动菜单，则尝试播放指定的歌曲
		if (!cfg.enableMenuWhatever && argc > 1 && cmdline == true)
		{
			log(logger, "Detected commandline command. playing target music...", 0);
			choosedmusic = argv[1];
			cmdline = false;
		}
		else // 否则乖乖启动主菜单(bushi
		{
			choosedmusic = mainmenu(logger, hwnd);
		}
		// 尝试打开选定的音乐
		configMusicFile cmfg = open("musics/" + choosedmusic,bgmpath);
		if (!cmfg.enabled) // 未启用音乐；音乐文件不存在/不完整?
		{
			cerr << "Failed to open target music!\n";
			log(logger, "Failed to load target music file: \"" + choosedmusic + "\" !", 1);
		}
		// 启动指定的音乐
		log(logger, "Playing music file: \"" + choosedmusic + "\" !", 0);
		score = play(choosedmusic, cmfg, bgmpath); // 启动主播放器
		if (score == -1)
		{
			continue;
		}
		log(logger, "played music file: \"" + choosedmusic + "\" !", 0);
	}

	// 退出
	log(logger, "Exiting...", 0);
	return 0;
}
