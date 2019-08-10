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
	vector<pair<string, pair< vector<int>, float > >> serverCostDetails;
	auto serverItr = servers.begin();
	while (serverItr != servers.end()) {
		serverCostDetails.push_back(make_pair(serverItr->first, GetServerCostUtil(serverItr->second, hours, cpus, price)));
		serverItr++;
	}
	sort(serverCostDetails.begin(), serverCostDetails.end(), comp);
	auto serverCostDetailsItr = serverCostDetails.begin();
	while (serverCostDetailsItr != serverCostDetails.end())
	{
		cout << "region :: " << serverCostDetailsItr->first << endl;
		cout << "total cost :: " << serverCostDetailsItr->second.second << endl;
		int totalCpuCount = 0;
		for (int i = 0; i < 6; i++)
		{
			cout << "server :: " << i << " :: count :: " << serverCostDetailsItr->second.first[i] << endl;
			totalCpuCount += (pow(2, i)*serverCostDetailsItr->second.first[i]);
		}
		cout << "totalCpuCount :: " << totalCpuCount << endl;
		serverCostDetailsItr++;
	}
	return serverCostDetails;
}


int main()
{
	string severCostDetails;
	int hours, cpus, price = -1;
	cout << "Enter the region and their server details as json :: " << endl;
	cin >> severCostDetails;
	JSONUtil jsonUtil(severCostDetails);
	if(!jsonUtil.ParseJSON())
	{
		cout<<"Parsing JSON failed"<<endl;
		return 0;
	}
	vector< vector<float> > servers;
	cout << "Enter CPU's required and how many hours you need  :: ";
	cin >> cpus >> hours;
	map<string, vector<float>>  zoneAndCpuCostMap = jsonUtil.GetZoneAndCpuCostMap();
	vector<pair<string, pair< vector<int>, float > >> result = GetServerCost(zoneAndCpuCostMap, hours, cpus, price);
	jsonUtil.ConstructJSON(result);
	system("pause");
	return 0;
}

