



#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#pragma once
using namespace std;
using namespace rapidjson;

class JSONUtil
{
private:
	map<string, vector<float>> zoneAndServerCostMap;
	map<string, int> serverCpuCountMap;
public:
	Document jsonDoc;
	JSONUtil(string jsonStr);
	bool ValidateJSONDocument(const char* json);
	string GetStringFromFile(string filePath);
	bool ParseJSON();
	map<string, vector<float>> GetZoneAndCpuCostMap();
	string ConstructJSON(vector<pair<string, pair< vector<int>, float > >> serverCostForEachZone);
	~JSONUtil();
};

