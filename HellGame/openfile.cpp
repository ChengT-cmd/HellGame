#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cstring>

#include <Windows.h>

#include "musicCfg.h"

using namespace std;

bool readConfigBool(string, string);
int readConfigInt(string, string);
string readConfigStr(string, string);

configMusicFile open(string target, char *bgmpath) // ����ļ��Ƿ���ڵ�ͬʱ˳����������
{
	configMusicFile cfg = {0};

	// δ�������֣������ļ�������/������?
	cfg.enabled = true;
	if (!readConfigBool(target + "\\music.properties", "enabled"))
	{
		cfg.enabled = false;
		return cfg; // ����ȫ���cfg(����Ƿ�ɹ�������Ҫ�鿴cfg.enabled)
	}

	cfg.hasBackgroundImg = readConfigBool(target+"\\music.properties","hasbackground"); // ��Ҫ����?
	
	strcpy(bgmpath, readConfigStr(target + "\\music.properties", "bgmpath").c_str()); // bgm�ļ�����?

	// �Ƿ��и���������û�оͲ����Դ��ļ���Լʱ��ռ�
	cfg.hasDrags = readConfigBool(target + "\\music.properties", "hasdrags");
	cfg.hasHolds = readConfigBool(target + "\\music.properties", "hasholds");
	cfg.hasTaps = readConfigBool(target + "\\music.properties", "hastaps");
	cfg.hasFlicks = readConfigBool(target + "\\music.properties", "hasflicks");

	// �ж����Ƿ���Ҫ����û�оͲ����Դ��ļ���Լʱ��ռ�
	cfg.changeJudgerPos = readConfigBool(target + "\\music.properties", "changejudgerpos");
	cfg.changeJudgerDig = readConfigBool(target + "\\music.properties", "changejudgerdig");

	// �ڲ��������
	cfg.tps = readConfigInt(target + "\\music.properties", "tps");
	cfg.speed = readConfigInt(target + "\\music.properties", "speed");

	return cfg;
}
