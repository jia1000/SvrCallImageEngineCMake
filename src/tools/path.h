/*=========================================================================

  Program:   ImagingEngine
  Module:    path.h
  author: 	 zhangjian
  Brief:	 �ṩ�ļ�Ŀ¼��������

=========================================================================*/
#pragma once
#include <iostream>
#include <cstdio>

using namespace std;

#ifdef WIN32
#include <io.h>
#include <atlstr.h>
#include <windows.h>
#include <vector>

inline bool CreateMultipleDirectory(const CString& szPath)
{
	CString strDir(szPath);//���Ҫ������Ŀ¼�ַ���
	//ȷ����'\'��β�Դ������һ��Ŀ¼
	if (strDir.GetAt(strDir.GetLength()-1)!=_T('\\'))
	{
		strDir.AppendChar(_T('\\'));
	}
	std::vector<CString> vPath;//���ÿһ��Ŀ¼�ַ���
	CString strTemp;//һ����ʱ����,���Ŀ¼�ַ���
	bool bSuccess = false;//�ɹ���־
	//����Ҫ�������ַ���
	for (int i=0;i<strDir.GetLength();++i)
	{
		if (strDir.GetAt(i) != _T('\\')) 
		{//�����ǰ�ַ�����'\\'
			strTemp.AppendChar(strDir.GetAt(i));
		}
		else 
		{//�����ǰ�ַ���'\\'
			vPath.push_back(strTemp);//����ǰ����ַ�����ӵ�������
			strTemp.AppendChar(_T('\\'));
		}
	}

	//�������Ŀ¼������,����ÿ��Ŀ¼
	std::vector<CString>::const_iterator vIter;
	for (vIter = vPath.begin(); vIter != vPath.end(); vIter++) 
	{
		//���CreateDirectoryִ�гɹ�,����true,���򷵻�false
		bSuccess = CreateDirectory(*vIter, NULL) ? true : false;    
	}

	return true;
}

inline bool DeleteFile(const CString& szPath)
{
	return DeleteFile(szPath);
}
#else
#include <unistd.h>
#include <sys/stat.h>


int str2char(string s, char c[])
{
	size_t l = s.length();
	int i;
	for(i = 0; i < l; i++)
		c[i] = s[i];

	c[i] = '\0';

	return i;
}


inline int CreateDirectoryEx(const std::string &str)  
{
	if (str.empty()){
		return 0;
	}
	std::vector<std::string> v;
	std::string pattern = "/";
	/// �����ȡ���һ������
	std::string strs = str + pattern;

	size_t pos = strs.find(pattern);
	size_t size = strs.size();

	while (pos != std::string::npos)
	{
		std::string x = strs.substr(0, pos);
		v.push_back(x);
		strs = strs.substr(pos + 1, size);
		pos = strs.find(pattern);
	}
	//std::vector<std::string> v = Split(sPathName, "/");
	std::string dst_dir_path("" );
	for(auto iter = v.begin(); iter != v.end(); ++iter){
		// �����ļ���
		dst_dir_path += *iter;
		dst_dir_path += "/";
		if( 0 != access(dst_dir_path.c_str(), 0)){
			printf("create folder: %s\n", dst_dir_path.c_str());
			// ����ļ��в����ڣ�����
			mkdir(dst_dir_path.c_str(), 0755);
		}
	}
	return 1;
}

bool PathFileExists(string dir, bool mkdir_flag)
{
	char des_dir[255];
	str2char(dir, des_dir); // ��string д�뵽�ַ�������
	int state = access(des_dir, R_OK|W_OK); // ͷ�ļ� #include <unistd.h>
	if (state==0) {
		//cout<<"file exist"<<endl;
		return true;
	}
	else if (mkdir_flag){
		dir = "mkdir " + dir;
		str2char(dir, des_dir);
		//cout<<des_dir<<endl;
		system(des_dir); // ����linuxϵͳ������ļ�

		return true;
	}
	else{
		return false;
	}
}

#endif
