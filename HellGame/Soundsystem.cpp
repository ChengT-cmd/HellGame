#define _CRT_SECURE_NO_WARNINGS true

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <cassert>

#include <Windows.h>
#include <mmsystem.h>

#pragma comment(lib,"WinMM.Lib")

using namespace std;

bool readConfigBool(string, string);
bool readConfigInt(string, string);
bool readConfigStr(string, string);

string logInit(void);
void error(string, string, int);
void log(string, string, bool);
string getTime(void);

string lastbgm;

//bool MByteToWChar(LPCSTR lpcszStr, LPWSTR lpwszStr, DWORD dwSize) // string转wchar_t
//{
//    // Get the required size of the buffer that receives the Unicode 
//    // string. 
//    DWORD dwMinSize;
//    dwMinSize = MultiByteToWideChar(CP_ACP, 0, lpcszStr, -1, NULL, 0);
//    assert(dwSize >= dwMinSize);
//
//    // Convert headers from ASCII to Unicode.
//    MultiByteToWideChar(CP_ACP, 0, lpcszStr, -1, lpwszStr, dwMinSize);
//    return true;
//}

void playbgm(string targetbgm) // 播放bgm函数
{
    // 我已经捣腾**MCI和Playsound几天了
    // 不是不满足条件(如暂停)就是玄学失败(返回正常但事实不工作)
    // 所以现在我**用pssuspend和vbs代替
    // vbs可以完全满足一切我需要的功能
    // 甚至同时支持扩展
    // 而且同时支持32/64
    // 总之我不会想再换回MCI了
    // 我也劝你们别多尝试在这里使用MCI并浪费过多时间
    // 
    // 
    // 
    //string tmp;
    //wchar_t wText[1001] = { 0 };
    //if (lastbgm == targetbgm)
    //{
    //    goto play;
    //}
    //lastbgm = targetbgm;
    //tmp += "close bgm";
    //MByteToWChar(tmp.c_str(), wText, sizeof(wText) / sizeof(wText[0]));
    //mciSendString(wText, NULL, 0, NULL); // mp3 close
    ////tmp += "open ";
    ////tmp += targetbgm;
    ////MByteToWChar(tmp.c_str(), wText, sizeof(wText) / sizeof(wText[0]));
    ///*if (!PlaySound(wText, NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP)) // wav
    //{
    //    cerr << "Failed to play the bgm!\n";
    //    exit(-3);
    //}*/

    //tmp = "";
    //tmp += "open ";
    //tmp += targetbgm;
    //tmp += " alias bgm";
    //MByteToWChar(tmp.c_str(), wText, sizeof(wText) / sizeof(wText[0]));
    ///*if (!PlaySound(wText, NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP)) // wav
    //{
    //    cerr << "Failed to play the bgm!\n";
    //    exit(-3);
    //}*/
    //if (mciSendString(wText, NULL, 0, NULL)) // mp3
    //{
    //    cerr << "Failed to open the bgm!\n";
    //    exit(-3);
    //}

    //play:
    //tmp = "play bgm";
    //MByteToWChar(tmp.c_str(), wText, sizeof(wText) / sizeof(wText[0]));
    //if (mciSendString(wText, NULL, 0, NULL)) // mp3
    //{
    //    cerr << "Failed to play the bgm!\n";
    //    exit(-4);
    //}

    char tmp[1001] = "\0";
    strcpy(tmp, "start ");
    strcat(tmp, targetbgm.c_str());
    system(tmp);
    return;
}

void stopbgm() // 不杀死主程序下停止bgm
{
    //string tmp;
    //tmp += "stop bgm";
    //wchar_t wText[1001] = { 0 };
    //MByteToWChar(tmp.c_str(), wText, sizeof(wText) / sizeof(wText[0]));
    ///*if (!PlaySound(NULL, NULL, SND_FILENAME | SND_ASYNC)) // wav
    //{
    //    cerr << "Failed to stop the bgm!\n";
    //    exit(-5);
    //}*/

    //if (mciSendString(wText, NULL, 0, NULL)) // mp3 stop
    //{
    //    cerr << "Failed to stop the bgm file!\n";
    //    exit(-5);
    //}
    system("taskkill /t /f /im wscript.exe");
    return;
}

void pausebgm() // 暂停bgm
{
    /*string tmp;
    tmp += "pause bgm";
    wchar_t wText[1001] = { 0 };
    MByteToWChar(tmp.c_str(), wText, sizeof(wText) / sizeof(wText[0]));
    if (mciSendString(wText, NULL, 0, NULL))
    {
        cerr << "Failed to pause the bgm!\n";
        exit(-5);
    }*/
    system("pssuspend.exe wscript.exe");
    return;
}

void resumebgm() // 继续bgm
{
    /*
    string tmp;
    tmp += "resume bgm";
    wchar_t wText[1001] = { 0 };
    MByteToWChar(tmp.c_str(), wText, sizeof(wText) / sizeof(wText[0]));
    if (mciSendString(wText, NULL, 0, NULL))
    {
        cerr << "Failed to resume the bgm!\n";
        exit(-20);
    }
    */
    system("pssuspend.exe wscript.exe -r");
    return;
}
