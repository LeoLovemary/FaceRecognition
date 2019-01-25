#include "iFolder.h"

void iFolder::reFileName(string folderPath, string exd, char *name, int pos){
	
	std::vector<string> files;
	getFiles(folderPath, exd, files);
	std::vector<string>::iterator it;
	char posC[100];
	int ind = 0;
	for (it = files.begin(); it < files.end(); it++){

		switch (pos)
		{
		case 2:
			sprintf(posC, "%s%02d", name,++ind);
			break;
		case 3:
			sprintf(posC, "%s%03d", name, ++ind);
			break;
		case 4:
			sprintf(posC, "%s%04d", name, ++ind);
			break;
		case 5:
			sprintf(posC, "%s%05d", name, ++ind);
			break;
		case 6:
			sprintf(posC, "%s%06d", name, ++ind);
			break;
		case 7:
			sprintf(posC, "%s%07d", name, ++ind);
			break;
		case 8:
			sprintf(posC, "%s%08d", name, ++ind);
			break;
		default:
			sprintf(posC, "%s%d", name, ++ind);
			break;
		}
		string newName = folderPath + "/";
		newName += posC;
		newName += ".";
		newName += exd;
		if (rename((*it).c_str(), newName.c_str()) < 0) return;
	}
}

/************************************************************************/
/*  ��ȡ�ļ����������ļ���
���룺
path	:	�ļ���·��
exd		:   ��Ҫ��ȡ���ļ�����׺����jpg��png�ȣ����ϣ����ȡ����
�ļ���, exd = ""
�����
files	:	��ȡ���ļ����б�

shao, 20140707
*/
/************************************************************************/
void iFolder::getFiles(string path, string exd, std::vector<string>& files)
{
	//�ļ����
	long   hFile = 0;
	//�ļ���Ϣ
	struct _finddata_t fileinfo;
	string pathName, exdName;

	if (0 != strcmp(exd.c_str(), ""))
	{
		exdName = "\\*." + exd;
	}
	else
	{
		exdName = "\\*";
	}

	if ((hFile = _findfirst(pathName.assign(path).append(exdName).c_str(), &fileinfo)) != -1)
	{
		do
		{
			//������ļ����������ļ���,����֮
			//�������,�����б�
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(pathName.assign(path).append("/").append(fileinfo.name), exd, files);
			}
			else
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					files.push_back(pathName.assign(path).append("/").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

//����ļ����Ƿ���ڣ��������򴴽�
//�ļ��д��ڷ��� 0  
//�ļ��д���ʧ�ܷ���-1  
//�ļ��д����ɹ�����1  
int iFolder::checkDir(char* Dir)
{
	FILE *fp = NULL;
	char TempDir[200];
	memset(TempDir, '\0', sizeof(TempDir));
	sprintf(TempDir, Dir);
	strcat(TempDir, "\\");
	strcat(TempDir, ".temp.fortest");
	fp = fopen(TempDir, "w");
	if (!fp)
	{
		if (_mkdir(Dir) == 0)
		{
			return 1;//�ļ��д����ɹ�  
		}
		else
		{
			return -1;//can not make a dir;  
		}
	}
	else
	{
		fclose(fp);
	}
	return 0;
}
int iFolder::checkDir(const char* Dir)
{
	FILE *fp = NULL;
	char TempDir[200];
	memset(TempDir, '\0', sizeof(TempDir));
	sprintf(TempDir, Dir);
	strcat(TempDir, "\\");
	strcat(TempDir, ".temp.fortest");
	fp = fopen(TempDir, "w");
	if (!fp)
	{
		if (_mkdir(Dir) == 0)
		{
			return 1;//�ļ��д����ɹ�  
		}
		else
		{
			return -1;//can not make a dir;  
		}
	}
	else
	{
		fclose(fp);
	}
	return 0;
}


bool iFolder::checkFolder(char* str){

	wchar_t dir[1000];
	mbstowcs(dir, str, 999);
	return 	PathIsDirectory(dir);
}

void iFolder::get_filelist(string path, std::vector<string>& files)
{
	//�ļ����
	long   hFile = 0;
	//�ļ���Ϣ
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		_findnext(hFile, &fileinfo);
		_findnext(hFile, &fileinfo);
		do
		{
			//�����Ŀ¼,����֮
			//�������,�����б�
			/*if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{*/
				//files.push_back(p.assign(path).append("/").append(fileinfo.name));
				files.push_back(fileinfo.name);
			//}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void iFolder::get_filelist(string path, string exd, std::vector<string>& files)
{
	//�ļ����
	long   hFile = 0;
	//�ļ���Ϣ
	struct _finddata_t fileinfo;
	string pathName, exdName;

	if (0 != strcmp(exd.c_str(), ""))
	{
		exdName = "\\*." + exd;
	}
	else
	{
		exdName = "\\*";
	}

	if ((hFile = _findfirst(pathName.assign(path).append(exdName).c_str(), &fileinfo)) != -1)
	{
		do
		{
			//������ļ����������ļ���,����֮
			//�������,�����б�
			/*if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(pathName.assign(path).append("/").append(fileinfo.name), exd, files);
			}
			else
			{*/
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					files.push_back(fileinfo.name);
			//}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}