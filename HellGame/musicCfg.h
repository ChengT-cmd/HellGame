#pragma once

#include <cstring>

struct configMusicFile // �����ļ����õ���
{
	bool enabled;

	bool hasBackgroundImg;
	bool hasDrags;
	bool hasHolds;
	bool hasTaps;
	bool hasFlicks;
	bool changeJudgerPos;
	bool changeJudgerDig;

	int tps;
	int speed;
};
typedef configMusicFile cfgmsc;
