#pragma once

#include <string>
#include <io.h>
#include <vector>
#include <iostream>
#include <direct.h>  

#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")
using namespace std;

class iFolder
{
public:
	void getFiles(string path, string exd, std::vector<string>& files);
	int checkDir(char* Dir);     // ����ļ����Ƿ���ڣ��������򴴽�
	int checkDir(const char* Dir);   // ����ļ����Ƿ���ڣ��������򴴽�
	bool checkFolder(char* str); // ֻ����ļ����Ƿ����
	void get_filelist(string path, std::vector<string>& files);//��ȡ�����ļ���


	void get_filelist(string path, string exd, std::vector<string>& files);
	void reFileName(string folderPath, string exd, char * name, int pos);
};
