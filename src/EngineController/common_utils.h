
#pragma once

#include <stdio.h>
#include <string>
#include <vector>

#if USE_JSONCPP_LIB
#include "json/json.h"
#endif

#if USE_JSONCPP_LIB
int GetJsonDataInt(const Json::Value& root, const std::string key, int& data);
int GetJsonDataString(const Json::Value& root, const std::string key, std::string& data);
int GetJsonDataFloat(const Json::Value& root, const std::string key, float& data);
#endif

void SplitString(const std::string& src, std::vector<std::string>& v, const std::string& c);
void TryCreateDir(const std::string& dir);
std::string TryAppendPathSeparatorInTail(const std::string& path);


int ListDir(std::string path, std::vector<std::string>& files);