#define _CRT_SECURE_NO_WARNINGS true

#define color_white    0
#define color_red      1
#define color_green    2
#define color_blue     3
#define color_yellow   4
#define color_pink     5
#define color_cyan     6
#define color_gray     7
#define color_fastRd   8
#define color_slowRd   9

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <queue>
#include <cstring>
#include <fstream>
#include <thread>
#include <mmstream.h>
#include <mmsyscom.h>
#include <sstream>
#include <cmath>

#include <Windows.h>

#include "musicCfg.h"
#include "key.h"

using namespace std;

int score = 0;

int play(string);
bool readConfigBool(string, string);
int readConfigInt(string, string);
string readConfigStr(string, string);
configMusicFile open(string, char*);

string logInit(void);
void error(string, string, int);
void log(string, string, bool);
string getTime(void);

void playbgm(string);
void stopbgm();
void pausebgm();
void resumebgm();

string mainmenu(string);

inline void gotoxy(int, int);
void mav(HWND);

inline double milsec()
{
	return (double)clock()/(CLOCKS_PER_SEC/1000);
}

void color(int c)
{
	switch (c)
	{
	case 0: // white
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	case 1: // red
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		break;
	case 2: // green
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		break;
	case 3: // blue
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		break;
	case 4: // yellow
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		break;
	case 5: // pink
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
		break;
	case 6: // cyan
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	case 7: // gray
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
		break;
	case 8: // fastRandom
		srand(milsec());
		color(rand() % 8);
		break;
	case 9: // SlowRandom
		srand(time(NULL));
		color(rand() % 8);
		break;
	default:
		mav(NULL);
	}
}

bool printFile(string path, int x, int y) // ��ӡ�ļ������˵����ܽᣩ
{
	color(0);
	int x_cpy = x;
	setlocale(LC_ALL, "");
	char temp[1001];
	int colour;
	ifstream fin;
	fin.open(path);
	if (!fin.is_open())
	{
		return false;
	}
	while (!fin.eof())
	{
		fin.getline(temp, sizeof(temp));
		if (temp[0] != ';')
		{
			gotoxy(x, y);
			for (int i = 0; i < strlen(temp); i++)
			{
				if (temp[i] != '`')
				{
					if (temp[i + 1] == '\\')
					{
						continue;
					}
					if (temp[i] == '\\' && i > 0)
					{
						colour = temp[i-1] - 48;
						color(colour);
						continue;
					}
					if (i>1&&temp[i - 1] == '`')
					{
						gotoxy(x_cpy, y);
					}
					printf("%c", temp[i]);
					++x_cpy;
				}
				else
				{
					++x_cpy;
				}
			}
			x_cpy = x;
			++y;
		}
	}
	return true;
}

struct note
{
	double time;
	int pos;
};

struct renderSequence
{
	int color = 0;
	int x = 0;
	int y = 0;
	string value;
};
queue<renderSequence> q;

bool A, S, D, F, Sp, H, J, K, L, Ed;
bool pause = false;
bool back = false;
bool stop = false;
void input(string path)
{
	while (!stop)
	{
		if (KEY_DOWN(65))
		{
			A = true;
		}
		else if (!KEY_DOWN(65) && A)
		{
			A = false;
		}
		if (KEY_DOWN(83))
		{
			S = true;
		}
		else if (!KEY_DOWN(83) && S)
		{
			S = false;
		}
		if (KEY_DOWN(68))
		{
			D = true;
		}
		else if (!KEY_DOWN(68) && D)
		{
			D = false;
		}
		if (KEY_DOWN(70))
		{
			F = true;
		}
		else if (!KEY_DOWN(70) && F)
		{
			F = false;
		}
		if (KEY_DOWN(32))
		{
			Sp = true;
		}
		else if (!KEY_DOWN(32) && Sp)
		{
			Sp = false;
		}
		if (KEY_DOWN(74))
		{
			J = true;
		}
		else if (!KEY_DOWN(74) && J)
		{
			J = false;
		}
		if (KEY_DOWN(75))
		{
			K = true;
		}
		else if (!KEY_DOWN(75) && K)
		{
			K = false;
		}
		if (KEY_DOWN(76))
		{
			L = true;
		}
		else if (!KEY_DOWN(76) && L)
		{
			L = false;
		}
		if (KEY_DOWN(186))
		{
			Ed = true;
		}
		else if (!KEY_DOWN(186) && Ed)
		{
			Ed = false;
		}
		if (KEY_DOWN(27))
		{
			pause = true;

			while (true)
			{
				if (KEY_DOWN(13)) // back
				{
					back = true;
					Sleep(500);
					return;
				}
				if (KEY_DOWN(8)) // continue
				{
					pause = false;
					break;
				}
			}
		}
	}
}

void renderer(void) // ��Ⱦ��
{
	renderSequence t;
	while (!stop && !back)
	{
		while (q.empty())
		{
			if (stop || back) // ��ͣ
			{
				return;
			}
		}

		// ��Ⱦһ֡
		t = q.front();
		q.pop();
		gotoxy(t.x, t.y);
		if (t.color != -1)
		{
			color(t.color);
		}
		printf("%s", t.value.c_str());
		if (t.color != -1)
		{
			color(0);
		}
	}
}

const int maxn = 1001;
note drag[maxn] = { 0 };
note flick[maxn] = { 0 };
note hold[maxn] = { 0 };
note tap[maxn] = { 0 };
int dt[31][120] = { 0 };
string bg[31] = { "\0" };

inline void j1(int x) // KeyDown
{
	q.push({ color_yellow,x,27,"==============" });
}

inline void j0(int x) // KeyUp
{
	q.push({ color_yellow,x,27,"--------------" });
}

int play(string music, configMusicFile cfg, string bgmpath) // ��������
{
	pause = false;
	stop = false;
	back = false;

	printFile("presets/musicIndex.resS", 0, 0);

	int score = 0;
	string rootPath = "musics/" + music;

	string tapImg;
	string holdHeadImg;
	string holdEmptyImg;
	string holdFullImg;
	string flickImg;
	string dragImg;

	char tmpStr[201] = "\0";
	ifstream readImg;
	{
		// ���Ե�O(1)��м����(bushi
		readImg.open("presets/tap.resS");
		readImg.getline(tmpStr, sizeof(tmpStr));
		tapImg = tmpStr;
		readImg.close();
		memset(tmpStr, '\0', sizeof(tmpStr));
		readImg.open("presets/flick.resS");
		readImg.getline(tmpStr, sizeof(tmpStr));
		flickImg = tmpStr;
		readImg.close();
		memset(tmpStr, '\0', sizeof(tmpStr));
		readImg.open("presets/drag.resS");
		readImg.getline(tmpStr, sizeof(tmpStr));
		dragImg = tmpStr;
		readImg.close();
		memset(tmpStr, '\0', sizeof(tmpStr));
		readImg.open("presets/hold_head.resS");
		readImg.getline(tmpStr, sizeof(tmpStr));
		holdHeadImg = tmpStr;
		readImg.close();
		memset(tmpStr, '\0', sizeof(tmpStr));
		readImg.open("presets/hold_empty.resS");
		readImg.getline(tmpStr, sizeof(tmpStr));
		holdEmptyImg = tmpStr;
		readImg.close();
		memset(tmpStr, '\0', sizeof(tmpStr));
		readImg.open("presets/hold_full.resS");
		readImg.getline(tmpStr, sizeof(tmpStr));
		holdFullImg = tmpStr;
		readImg.close();
	}
	system(("title " + readConfigStr(rootPath + "/music.properties", "customname")).c_str());

	// ��ʱ�����������
	register int nowDrag = 0;
	register int nowFlick = 0;
	register int nowHold = 0;
	register int nowTap = 0;

	// ��ʱ��ťֵ����
	register bool La=false, Ls = false, Ld = false, Lf = false, LSp = false, Lj = false, Lk = false, Ll = false, LEd = false;

	// ���ļ�
	if (cfg.hasDrags)
	{
		memset(drag, -1, sizeof(drag));
		string drags = rootPath + "/drags.f";
		ifstream fdrags;
		fdrags.open(drags);
		int i = 0;
		while (!fdrags.eof())
		{
			fdrags >> drag[i].time >> drag[i].pos;
			++i;
		}
		fdrags.close();
	}
	if (cfg.hasFlicks)
	{
		memset(flick, -1, sizeof(flick));
		string flicks = rootPath + "/flicks.f";
		ifstream fflicks;
		fflicks.open(flicks);
		int i = 0;
		while (!fflicks.eof())
		{
			fflicks >> flick[i].time >> flick[i].pos;
			++i;
		}
		fflicks.close();
	}
	if (cfg.hasHolds)
	{
		memset(hold, -1, sizeof(hold));
		string holds = rootPath + "/holds.f";
		ifstream fholds;
		fholds.open(holds);
		int i = 0;
		while (!fholds.eof())
		{
			fholds >> hold[i].time >> hold[i].pos;
			++i;
		}
		fholds.close();
	}
	if (cfg.hasTaps)
	{
		memset(tap, -1, sizeof(tap));
		string taps = rootPath + "/taps.f";
		ifstream ftaps;
		ftaps.open(taps);
		int i = 0;
		while (!ftaps.eof())
		{
			ftaps >> tap[i].time >> tap[i].pos;
			++i;
		}
		ftaps.close();
	}
	if (cfg.hasBackgroundImg) // TODO AddBackground
	{
		ifstream img;
		img.open(rootPath + "/background.dt");
		char tmp[151];
		for (int i = 0; i < 30; i++)
		{
			memset(tmp, '\0', sizeof(tmp));
			img.getline(tmp, sizeof(tmp));
			bg[i] = tmp;
		}
		printFile(rootPath + "/background.dt", 0, 0);
	}
	else
	{
		for (int i = 0; i < 30; i++)
		{
			bg[i] += "                                                                                                                        ";
		}
	}


	//todo Move&Rotate Judgerln
	/*if (cfg.changeJudgerDig)
	{
		string jDig = rootPath + "/judgerdig.f";
		ifstream fjDig;
		fjDig.open(jDig);
	}
	if (cfg.changeJudgerPos)
	{
		string jPos = rootPath + "/judgerpos.f";
		ifstream fjPos;
		fjPos.open(jPos);
	}*/

	thread ipt(input, rootPath); // �̣߳����������¼
	thread rd(renderer); // �̣߳�����Ⱦ��

	long tick = 0; // Ŀǰtick��
	long lasttick = -1; // ��һ��tick
	double startTime = milsec(); // ����ʱ�䣨���ڼ�¼tick��
	double nowTime; // ����ʱ��
	note tmpNote; // ��ʱ�������ͱ���
	readImg.open("presets/judgerln.resS"); // ���ж��ߵ�img
	readImg.getline(tmpStr, sizeof(tmpStr));
	string judgerln = tmpStr;
	readImg.close();
	q.push({ color_yellow, 0,27,judgerln }); // �����ж���
	playbgm(rootPath + "/" + bgmpath); // ����bgm
	while (true) // ��ѭ��
	{
		if (pause) // ��ͣ�¼�����
		{
			pausebgm(); // ��ͣbgm
			while (!q.empty()) { ; }; // �ȴ�����Ⱦ��ֹͣ
			if (cfg.hasBackgroundImg) // TODO:ADDBACKGROUNDIMG
			{
				printFile(rootPath + "/background.dt", 0, 0);
			}
			else
			{
				printFile("presets/musicIndex.resS", 0, 0);
			}
			printFile("presets/judgerln.resS", 0, 25);
			printFile("presets/paused.resS", 38, 13); // ������ͣ����img
			while (pause^back) { ; }; // �ȵ�����ͣ
			if (back) // �뿪��?
			{
				ipt.join();
				rd.join();
				stopbgm();
				return -1;
			}
			resumebgm(); // ��������bgm
			if (cfg.hasBackgroundImg) // ���ñ���
			{
				printFile(rootPath + "/background.dt", 0, 0);
			}
			else
			{
				printFile("presets/musicIndex.resS", 0, 0);
			}
			printFile("presets/judgerln.resS", 0, 25); // �����ж���
		}
		nowTime = milsec(); // ��������ʱ��
		if ((long)(nowTime - startTime) /cfg.speed * cfg.tps - tick >= 1) // ����1tick
		{
			q.push({ color_green,0,0, to_string(cfg.tps) });
			q.push({ color_green,0,1,to_string(tick) });
			q.push({ color_white, 0, 29, bg[29] });
			tick++;
		}
		for (int i = 0; i < 30; i++) // ���Ͷ��е���Ⱦ��
		{
			for (int j = 0; j < 120; j++)
			{
				if (dt[i][j] < 4 && dt[i][j]>0)
				{
					if (i - 1 >= 0)
					{
						q.push({ color_white, 0, i - 1, bg[i - 1] });
						if (i == 28)
						{
							q.push({ color_white, 0, 29, bg[29] });
						}
						if (i - 1 == 27)
						{
							q.push({ color_yellow, 0,27,judgerln });
						}
					}
				}
				switch (dt[i][j])
				{
					case 1: // tap
						q.push({ color_cyan, j - 5, i, tapImg });
						break;
					case 2: // drag
						q.push({ color_yellow, j - 5, i, dragImg });
						break;
					case 3: // flick
						q.push({ color_red, j - 5, i, flickImg });
						break;
					case 4: // hold
						break;
					default:
						break;
				}
			}
		}
		if (tick > lasttick) // ����һtick?
		{
			lasttick = tick;
			for (int i = 30; i > 0; i--)
			{
				for (int j = 0; j < 120; j++)
				{
					dt[i][j] = dt[i-1][j];
					dt[i - 1][j] = 0;
				}
			}
			while (drag[nowDrag].time <= tick)
			{
				tmpNote = drag[nowDrag];
				dt[0][tmpNote.pos] = 2;
				nowDrag++;
			}
			while (flick[nowFlick].time <= tick)
			{
				tmpNote = flick[nowFlick];
				dt[0][tmpNote.pos] = 3;
				nowFlick++;
			}
			while (tap[nowTap].time <= tick)
			{
				tmpNote = tap[nowTap];
				dt[0][tmpNote.pos] = 1;
				nowTap++;
			}
			while (tap[nowHold].time <= tick)
			{
				tmpNote = hold[nowHold];
				dt[0][tmpNote.pos] = 4;
				nowHold++;
			}
		}

		// �����ж�
		{
			if (A&&La)
			{
				j1(0);
				La = !La;
			}
			else if(!A&&!La)
			{
				j0(0);
				q.push({ color_yellow, 0,27,"+" });
				La = !La;
			}
			if (S&&Ls)
			{
				j1(14);
				Ls = !Ls;
			}
			else if(!S&&!Ls)
			{
				j0(14);
				Ls = !Ls;
			}
			if (D&&Ld)
			{
				j1(28);
				Ld = !Ld;
			}
			else if(!D&&!Ld)
			{
				j0(28);
				Ld = !Ld;
			}
			if (F&&Lf)
			{
				j1(42);
				Lf = !Lf;
			}
			else if(!F&&!Lf)
			{
				j0(42);
				Lf = !Lf;
			}
			if (Sp&&LSp)
			{
				q.push({ color_yellow,56,27,"========" });
				LSp = !LSp;
			}
			else if(!Sp&&!LSp)
			{
				q.push({ color_yellow,56,27,"--------" });
				LSp = !LSp;
			}
			if (J&&Lj)
			{
				j1(64);
				Lj = !Lj;
			}
			else if(!J&&!Lj)
			{
				j0(64);
				Lj = !Lj;
			}
			if (K&&Lk)
			{
				j1(78);
				Lk = !Lk;
			}
			else if(!K&&!Lk)
			{
				j0(78);
				Lk = !Lk;
			}
			if (L&&Ll)
			{
				j1(92);
				Ll = !Ll;
			}
			else if(!L&&!Ll)
			{
				j0(92);
				Ll = !Ll;
			}
			if (Ed&&LEd)
			{
				j1(106);
				LEd = !LEd;
			}
			else if(!Ed&&!LEd)
			{
				j0(106);
				q.push({ color_yellow, 119,27,"+" });
				LEd = !LEd;
			}
		}
		while (!q.empty()) { ; }; // �ȴ���Ⱦ���,����ѭ��
	}
	return score;
}
