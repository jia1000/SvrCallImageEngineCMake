/*=========================================================================

  Program:   ImagingEngine
  Module:    logger.h
  author: 	 zhangjian
  Brief:	 ��װ��־�ӿ�

=========================================================================*/
#pragma once

//#include "StdAfx.h"
#include <iostream>
#include <stdarg.h>
#include <stdint.h>
#include <string> 
#ifdef WIN32
#include "tools/path.h"
#endif


#define GOOGLE_GLOG_DLL_DECL
#define GLOG_NO_ABBREVIATED_SEVERITIES

#define LOG_LEVEL_DEBUG			9
#define LOG_LEVEL_INFO			3
#define LOG_LEVEL_ERROR			1

#define LOG_IF_INFO 			1

#include <glog/logging.h>
#include <glog/log_severity.h> 
//#include "tools/configurations.h"

using namespace std;

class CGLogger
{
public:
	CGLogger(void)
	{
	}

	~CGLogger(void)
	{
		
	}

	static void Info(string msg)
	{
		LOG(INFO) << msg;
	}

	static void Error(string msg, string ex = "")
	{
		if (ex.empty()){
			LOG(ERROR) << msg;
		}
		else{
			LOG(ERROR) << msg << " \\r\\nException:" + ex;
		}
	}

	static void Debug(string msg)
	{
#ifdef _DEBUG
		LOG(WARNING) << msg;	
#else
		int log_level =	9;			//ConfigurationManager::Get()->GetLogLevel();	
		if (log_level >= 9) {
			LOG(WARNING) << msg;
		}
#endif // _DEBUG
	}
	
	static void Info_If(string msg)
	{
		LOG_IF(INFO, LOG_IF_INFO) << msg;
	}

	static void InitGLog(string proc_name, string directory)
	{
		google::InitGoogleLogging(proc_name.c_str());

#ifdef WIN32
		SYSTEMTIME st;
		::GetLocalTime(&st);
		char chYearMonth[1000] = { 0 };
		printf_s(chYearMonth, "%04d%02d", st.wYear, st.wMonth);
		char chDay[1000] = { 0 };
		printf_s(chDay, "%02d", st.wDay);

		// get log directory according to current date
		string log_dir = "log\\" + string(chYearMonth) + "\\" + string(chDay) + "\\";
#else
		// ���ڵ�ǰϵͳ�ĵ�ǰ����/ʱ��
		time_t now = time(0);
		tm *ltm = localtime(&now);

		char iyear[50], imonth[50], iday[50], ihour[50], imin[50], isec[50];

		sprintf(iyear, "%d",1900 + ltm->tm_year );
		sprintf(imonth, "%02d", 1 + ltm->tm_mon );
		sprintf(iday, "%02d", ltm->tm_mday );
		sprintf(ihour, "%02d", ltm->tm_hour );
		sprintf(imin, "%02d",  ltm->tm_min);
		sprintf(isec, "%02d",  ltm->tm_sec);

		// get log directory according to current date
		string log_dir = "log\\" + string(iyear) + "\\" + string(imonth) + "\\" + string(iday) + "\\";
#endif

		#ifdef DEBUG_MODE
		google::SetStderrLogging(google::GLOG_INFO);								//���ü������ google::INFO ����־ͬʱ�������Ļ
#else
		google::SetStderrLogging(google::GLOG_FATAL);								//���ü������ google::FATAL ����־ͬʱ�������Ļ
#endif
		
		//// create log directory
		string log_path = directory + log_dir;
#ifdef WIN32
		if (!PathFileExistsA(log_path.c_str())) {
			CreateMultipleDirectory(log_path.c_str());
		}
#else
		//if (PathFileExists(log_path.c_str(), false)){
		//	CreateDirectoryEx(log_path.c_str());
		//}
#endif

		FLAGS_logtostderr = false;													//TRUE:��׼���,FALSE:�ļ����	    
		FLAGS_alsologtostderr = true;												//������־�ļ�֮���Ƿ���Ҫ��׼���	
		FLAGS_colorlogtostderr = true;												//�����������Ļ����־��ʾ��Ӧ��ɫ
		google::SetLogDestination(google::GLOG_INFO, log_path.c_str());				//���� google::INFO �������־�洢·�����ļ���ǰ׺
		google::SetLogDestination(google::GLOG_WARNING, log_path.c_str());			//���� google::WARNING �������־�洢·�����ļ���ǰ׺
		google::SetLogDestination(google::GLOG_ERROR, log_path.c_str());			//���� google::ERROR �������־�洢·�����ļ���ǰ׺
		google::SetLogDestination(google::GLOG_FATAL, log_path.c_str());			//���� google::ERROR �������־�洢·�����ļ���ǰ׺
		FLAGS_logbufsecs =0;														//������־�����Ĭ��Ϊ30�룬�˴���Ϊ�������
		FLAGS_max_log_size =1;														//�����־��СΪ 1MB
		FLAGS_stop_logging_if_full_disk = true;										//�����̱�д��ʱ��ֹͣ��־���
		google::SetLogFilenameExtension(".log");									//�����ļ�����չ����ƽ̨����������Ҫ���ֵ���Ϣ
  		google::InstallFailureSignalHandler();										//��׽ ���� dumped
  		google::InstallFailureWriter(&SignalHandle);								//Ĭ�ϲ�׽ SIGSEGV �ź���Ϣ���������� stderr������ͨ������ķ����Զ������>��ʽ��
	}

private:
	//����Ϣ�����LOG(FATAL)
	static void SignalHandle(const char* data, int size)
	{
		std::string str = std::string(data,size);
		/*
		std::ofstream fs("glog_dump.log",std::ios::app);
		fs<<str;
		fs.close();
		*/
		cout << "Fatal error occured. \r\n" << str <<endl;

		//Ҳ����ֱ�������﷢���ʼ������֪ͨ��������������Ǳ��ص���εģ�ÿ�λص�ֻ���һ�д�����Ϣ������������ļ�¼���ļ���Ҳ��Ҫ>��׷��ģʽ���ɣ���
		//�������﷢�ʼ�����Ų��Ǻ��ʺϣ��������ǿ��Ե���һ�� SHELL �� PYTHON �ű������˽ű����� sleep 3�����ң�Ȼ�󽫴�
		//����Ϣͨ���ʼ�����ŷ��ͳ�ȥ�������Ͳ���Ҫ��ؽű���ʱ��Ƶ��ִ�У��˷�Ч���ˡ�
	}

};