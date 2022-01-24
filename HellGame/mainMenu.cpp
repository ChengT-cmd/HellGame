#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <mmstream.h>
#include <sstream>

#include <Windows.h>

#include "main.h"
#include "musicCfg.h"
#include "key.h"

using namespace std;

bool readConfigBool(string, string);
int readConfigInt(string, string);
string readConfigStr(string, string);

void error(string, string, int);
void log(string, string, bool);

bool printFile(string, int, int);
void gotoxy(int, int);

void mav(HWND);

void musicPlaying(void) // 播放开始
{
	printFile("presets/musicIndex.resS", 0, 0);
}

void musicEnded(void) // 播放结束
{
	printFile("presets/musicSummary.resS", 0, 0);
}

void mainMenuUpdate(int recent, int now) // 选择可视化
{
	printFile("presets/mainMenuButtonUp.resS", 47, 11 + recent * 4);
	printFile("presets/mainMenuButtonDown.resS", 47, 11 + now * 4);
	return;
}

void chooseMusicWrite(void) // 打印可用的音乐列表
{
	for (int i = 1; i <= 9; i++)
	{
		gotoxy(47, 5 + (i - 1) * 2);
		cout << readConfigStr("configs/registry.ini", to_string(i));
	}
	gotoxy(47, 25);
	return;
}

string runEvent(int eventNum, string logger, HWND hwnd) // 运行事件
{
	if (eventNum == 0)
	{
		string result;
		printFile("presets/chooseMusic.resS", 0, 0);
		chooseMusicWrite();
		while (1)
		{
			if (KEY_DOWN(8)) // BACKSPACE
			{
				return "";
			}
			for (int i = 0; i < 9; i++)
			{
				if (KEY_DOWN(49 + i)) // 字符键盘 1-9
				{
					result = readConfigStr("configs/registry.ini", to_string(i+1));
					return result;
				}
			}
		}
	}
	else if (eventNum == 1) // 设置
	{
		system("start configs/settings.ini");
		return "";
	}
	else if(eventNum == 2) // 退出
	{
		log(logger, "Exiting...", 0);
		exit(0);
	}
	else // 未知的异常
	{
		mav(hwnd);
	}
}

string mainmenu(string logger, HWND hwnd) // 主菜单界面
{
	string choosedmusic = "";
	int recent = 0, now = 0;
	if (!printFile("presets/mainMenu.resS", 0, 0))
	{
		cerr << "Failed to start main menu!";
		error(logger, "Failed to start main menu!\n", -5);
	}
	printFile("presets/mainMenuButtonDown.resS", 47, 11);
	//printFile("presets/colorMap.resS", 0, 0); // debug of colorMap
	while (true)
	{
		if (KEY_DOWN(27)) // ESC
		{
			printFile("presets/escape.resS", 38, 13);
			while (true)
			{
				if (KEY_DOWN(13)) // CONFIRM
				{
					log(logger, "Exiting...", 0);
					exit(0);
				}
				else if (KEY_DOWN(8)) // BACKSPACE
				{
					printFile("presets/mainMenu.resS", 0, 0);
					mainMenuUpdate(recent, now);
					break;
				}
			}
		}
		if (KEY_DOWN(40)) // DownArrow
		{
			if (now + 1 > 2)
			{
				continue;
			}
			recent = now;
			now = recent + 1;
			mainMenuUpdate(recent, now);
			Sleep(200);
		}
		if (KEY_DOWN(38)) // UpArrow
		{
			if (now - 1 < 0)
			{
				continue;
			}
			recent = now;
			now = recent - 1;
			mainMenuUpdate(recent, now);
			Sleep(200);
		}
		if (KEY_DOWN(13)) // Enter
		{
			choosedmusic = runEvent(now, logger, hwnd);
			if (choosedmusic == "")
			{
				printFile("presets/mainMenu.resS", 0, 0);
				mainMenuUpdate(recent, now);
			}
			else
			{
				break;
			}
		}
	}
	return choosedmusic;
}
