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

configMusicFile open(string target, char *bgmpath) // 检查文件是否存在的同时顺便输入配置
{
	configMusicFile cfg = {0};

	// 未启用音乐；音乐文件不存在/不完整?
	cfg.enabled = true;
	if (!readConfigBool(target + "\\music.properties", "enabled"))
	{
		cfg.enabled = false;
		return cfg; // 返回全零的cfg(检查是否成功部分主要查看cfg.enabled)
	}

	cfg.hasBackgroundImg = readConfigBool(target+"\\music.properties","hasbackground"); // 需要背景?
	
	strcpy(bgmpath, readConfigStr(target + "\\music.properties", "bgmpath").c_str()); // bgm文件在哪?

	// 是否有各类音符？没有就不尝试打开文件节约时间空间
	cfg.hasDrags = readConfigBool(target + "\\music.properties", "hasdrags");
	cfg.hasHolds = readConfigBool(target + "\\music.properties", "hasholds");
	cfg.hasTaps = readConfigBool(target + "\\music.properties", "hastaps");
	cfg.hasFlicks = readConfigBool(target + "\\music.properties", "hasflicks");

	// 判定线是否需要鬼畜？没有就不尝试打开文件节约时间空间
	cfg.changeJudgerPos = readConfigBool(target + "\\music.properties", "changejudgerpos");
	cfg.changeJudgerDig = readConfigBool(target + "\\music.properties", "changejudgerdig");

	// 内部运算基础
	cfg.tps = readConfigInt(target + "\\music.properties", "tps");
	cfg.speed = readConfigInt(target + "\\music.properties", "speed");

	return cfg;
}
