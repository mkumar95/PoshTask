#include "stdafx.h"
#include "JSONUtil.h"


JSONUtil::JSONUtil(string jsonStr)
{
	//string jsonStr = GetStringFromFile(filePath);
	if (!jsonStr.empty())
	{
		if (this->ValidateJSONDocument(jsonStr.c_str()))
		{
			cout << "Validation of JSON Doc Succeedded" << endl;
			serverCpuCountMap.emplace(make_pair("large", 0));
			serverCpuCountMap.emplace(make_pair("xlarge", 1));
			serverCpuCountMap.emplace(make_pair("2xlarge", 2));
			serverCpuCountMap.emplace(make_pair("4xlarge", 3));
			serverCpuCountMap.emplace(make_pair("8xlarge", 4));
			serverCpuCountMap.emplace(make_pair("10xlarge", 5));
		}
		else
		{
			cout << "JSON validation failed" << endl;
		}
	}
	else
	{
		cout << "reading file failed" << endl;
	}
}

bool JSONUtil::ValidateJSONDocument(const char* json)
{
	if (jsonDoc.Parse(json).HasParseError()) {
		return true;
	}
	char buffer[sizeof(json)];
	memcpy(buffer, json, sizeof(json));
	if (jsonDoc.ParseInsitu(buffer).HasParseError()) {
		return true;
	}
	return false;
}

string JSONUtil::GetStringFromFile(string filePath)
{
	ifstream f(filePath);
	ostringstream out;
	out << f.rdbuf();
	string jsonStr = out.str();
	cout << " JSONStr :: " << jsonStr << endl;
	return jsonStr;
}

bool JSONUtil::ParseJSON()
{
	try
	{
		auto zoneItr = jsonDoc.MemberBegin();
		while (zoneItr != jsonDoc.MemberEnd())
		{
			string zoneName = zoneItr->name.GetString();
			const Value& zoneValue = zoneItr->value;
			vector<float> cpuCostVect(6,-1); // server types 6
			for (Value::ConstMemberIterator itr = zoneValue.MemberBegin(); itr != zoneValue.MemberEnd(); ++itr)
			{
				int index = serverCpuCountMap[itr->name.GetString()];
				cpuCostVect[index] = itr->value.GetFloat();
			}
			zoneAndServerCostMap.emplace(make_pair(zoneName, cpuCostVect));
			zoneItr++;
		}
	}
	catch(...)
	{
		cout<<"Parsing json failed"<<endl;
	}
	return false;
}

map<string, vector<float>> JSONUtil::GetZoneAndCpuCostMap()
{
	return zoneAndServerCostMap;
}

string JSONUtil::ConstructJSON(vector<pair<string, pair< vector<int>, float > >> serverCostForEachZone)
{
	string jsonStr = "";
	Document jsonDoc;
	jsonDoc.SetObject();
	Document::AllocatorType& allocator = jsonDoc.GetAllocator();
	auto serverItr = serverCostForEachZone.begin();
	Value serverListWithZone(kArrayType);
	while (serverItr != serverCostForEachZone.end())
	{
		Value region(kObjectType), cost(kObjectType);
		Value servers(kArrayType);
		string zoneName = serverItr->first;
		float totalCost = serverItr->second.second;

		region.SetString(zoneName.c_str(), static_cast<SizeType>(zoneName.length()), allocator);
		cost.SetFloat(totalCost);
		auto serverVectItr = serverItr->second.first.begin();
		int serverType = 0;
		for (;serverVectItr< serverItr->second.first.end(); serverVectItr++, serverType++)
		{
			if (*serverVectItr)
			{
				Value serverCount(kObjectType);
				switch (serverType)
				{
					case 0:
					{
						serverCount.AddMember("large", *serverVectItr, allocator);
						break;
					}
					case 1:
					{
						serverCount.AddMember("xlarge", *serverVectItr, allocator);
						break;
					}
					case 2:
					{
						serverCount.AddMember("2xlarge", *serverVectItr, allocator);
						break;
					}
					case 3:
					{
						serverCount.AddMember("4xlarge", *serverVectItr, allocator);
						break;
					}
					case 4:
					{
						serverCount.AddMember("8xlarge", *serverVectItr, allocator);
						break;
					}
					case 5:
					{
						serverCount.AddMember("10xlarge", *serverVectItr, allocator);
						break;
					}
					default:
					{
						cout << "Invalid server type" << endl;
						break;
					}
				}
				servers.PushBack( serverCount, allocator);
			}
		}
		Value childNode(kObjectType);
		childNode.AddMember("region", region, allocator);
		childNode.AddMember("totalcost", cost, allocator);
		childNode.AddMember("servers", servers, allocator);
		serverListWithZone.PushBack(childNode, allocator);
		serverItr++;
	}
	jsonDoc.AddMember("ServerCostDetails",serverListWithZone, allocator);
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	jsonDoc.Accept(writer);
	jsonStr = buffer.GetString();
	cout << jsonStr << std::endl;
	return jsonStr;
}

void SetServerName(Value &serverCount, int serverType, Document::AllocatorType allocator)
{
	switch (serverType)
	{
		case 0:
		{
			serverCount.AddMember("large", serverCount, allocator);
			break;
		}
		case 1:
		{
			serverCount.AddMember("xlarge", serverCount, allocator);
			break;
		}
		case 2:
		{
			serverCount.AddMember("2xlarge", serverCount, allocator);
			break;
		}
		case 3:
		{
			serverCount.AddMember("4xlarge", serverCount, allocator);
			break;
		}
		case 4:
		{
			serverCount.AddMember("8xlarge", serverCount, allocator);
			break;
		}
		case 5:
		{
			serverCount.AddMember("10xlarge", serverCount, allocator);
			break;
		}
		default:
		{
			cout << "Invalid server type" << endl;
			break;
		}
	}
}

JSONUtil::~JSONUtil()
{
}
