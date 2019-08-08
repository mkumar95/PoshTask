// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <JSONUtil.h>
#include <algorithm>
using namespace std;

bool comp(pair<string, pair<vector<int>, float >> a, pair<string, pair<vector<int>, float >> b) {
	return a.second.second <= b.second.second;
}
pair< vector<int>, float >  GetServerCostUtil(vector<float> server, int hours, int cpus, float price) {
	vector< pair<float, int> > dp(cpus + 32);
	dp[0] = { 0,-1 };
	int resi = -1;
	float resVal = INT_MAX;
	for (int i = 1; i<cpus + 32; i++) {
		dp[i].first = INT_MAX;
		for (int j = 0; j <= 5; j++) {
			int powVal = pow(2, j);
			if (i >= powVal && server[j] != -1 && dp[i].first > dp[i - powVal].first + server[j])
				dp[i] = { dp[i - powVal].first + server[j], j };
		}
		if (i >= cpus && resVal>dp[i].first) {
			resVal = dp[i].first; resi = i;
		}
	}
	vector< int > res(6, 0);
	while (resi > 0) {
		res[dp[resi].second]++;
		resi -= pow(2, dp[resi].second);
	}
	return{ res, hours*resVal };
}
vector<pair<string, pair< vector<int>, float > >>  GetServerCost(map<string, vector<float>>  servers, int hours, int cpus, float price) {
	vector<pair<string, pair< vector<int>, float > >> result;
	auto serverItr = servers.begin();
	while (serverItr != servers.end()) {
		result.push_back(make_pair(serverItr->first, GetServerCostUtil(serverItr->second, hours, cpus, price)));
		serverItr++;
	}
	sort(result.begin(), result.end(), comp);
	auto itr = result.begin();
	while (itr != result.end())
	{
		cout << "region :: " << itr->first << endl;
		cout << "total cost :: " << itr->second.second << endl;
		int resultcount = 0;
		for (int i = 0; i < 6; i++)
		{
			cout << "server :: " << i << " :: count :: " << itr->second.first[i] << endl;
			resultcount += (pow(2, i)*itr->second.first[i]);
		}
		cout << "totalCpuCount :: " << resultcount << endl;
		itr++;
	}
	return result;
}


int main()
{
	string filePath;
	int hours, cpus, price;
	cout << "Enter the filePath :: " << endl;
	cin >> filePath;
	JSONUtil jsonUtil(filePath);
	jsonUtil.ParseJSON();
	vector< vector<float> > servers;
	cout << "Enter CPUs, Price and Hours :: ";
	cin >> cpus>>price>>hours;
	map<string, vector<float>>  zoneAndCpuCostMap = jsonUtil.GetZoneAndCpuCostMap();
	vector<pair<string, pair< vector<int>, float > >> result = GetServerCost(zoneAndCpuCostMap, hours, cpus, price);
	jsonUtil.ConstructJSON(result);
	system("pause");
	return 0;
}

