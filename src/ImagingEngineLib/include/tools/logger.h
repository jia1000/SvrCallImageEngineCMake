/*=========================================================================

  Program:   ImagingEngine
  Module:    logger.h
  author: 	 zhangjian
  Brief:	 封装日志接口

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
		// 基于当前系统的当前日期/时间
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
		google::SetStderrLogging(google::GLOG_INFO);								//设置级别高于 google::INFO 的日志同时输出到屏幕
#else
		google::SetStderrLogging(google::GLOG_FATAL);								//设置级别高于 google::FATAL 的日志同时输出到屏幕
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

		FLAGS_logtostderr = false;													//TRUE:标准输出,FALSE:文件输出	    
		FLAGS_alsologtostderr = true;												//除了日志文件之外是否需要标准输出	
		FLAGS_colorlogtostderr = true;												//设置输出到屏幕的日志显示相应颜色
		google::SetLogDestination(google::GLOG_INFO, log_path.c_str());				//设置 google::INFO 级别的日志存储路径和文件名前缀
		google::SetLogDestination(google::GLOG_WARNING, log_path.c_str());			//设置 google::WARNING 级别的日志存储路径和文件名前缀
		google::SetLogDestination(google::GLOG_ERROR, log_path.c_str());			//设置 google::ERROR 级别的日志存储路径和文件名前缀
		google::SetLogDestination(google::GLOG_FATAL, log_path.c_str());			//设置 google::ERROR 级别的日志存储路径和文件名前缀
		FLAGS_logbufsecs =0;														//缓冲日志输出，默认为30秒，此处改为立即输出
		FLAGS_max_log_size =1;														//最大日志大小为 1MB
		FLAGS_stop_logging_if_full_disk = true;										//当磁盘被写满时，停止日志输出
		google::SetLogFilenameExtension(".log");									//设置文件名扩展，如平台？或其它需要区分的信息
  		google::InstallFailureSignalHandler();										//捕捉 程序 dumped
  		google::InstallFailureWriter(&SignalHandle);								//默认捕捉 SIGSEGV 信号信息输出会输出到 stderr，可以通过下面的方法自定义输出>方式：
	}

private:
	//将信息输出到LOG(FATAL)
	static void SignalHandle(const char* data, int size)
	{
		std::string str = std::string(data,size);
		/*
		std::ofstream fs("glog_dump.log",std::ios::app);
		fs<<str;
		fs.close();
		*/
		cout << "Fatal error occured. \r\n" << str <<endl;

		//也可以直接在这里发送邮件或短信通知，不过这个方法是被回调多次的（每次回调只输出一行错误信息，所以如上面的记录到文件，也需要>以追加模式方可），
		//所以这里发邮件或短信不是很适合，不过倒是可以调用一个 SHELL 或 PYTHON 脚本，而此脚本会先 sleep 3秒左右，然后将错
		//误信息通过邮件或短信发送出去，这样就不需要监控脚本定时高频率执行，浪费效率了。
	}

};