#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
using namespace std;
const int N = 10;        //初始k值

//Module类为<d,k,K>三元存储
class Module {
public:
	int distance;
	int modulenums;
	vector<vector<int>> part;
	Module():distance(0), modulenums(0){};
	Module(int distance, int modulenums, vector<vector<int>> part) {
		this->distance = distance;
		this->modulenums = modulenums;
		this->part = part;
	}
};

//整个解决方案
class Solution {
public:
	const int D[10] = { 1,2,3,4,5,6,7,8,9,10 };			//各个任务编号从1到10
	const vector<vector<int>> COST= {					//初始任务代价
		{0,1,0,0,0,0,4,0,2,0},
		{0,0,3,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,2,0,1},
		{0,0,0,0,0,2,0,5,6,0},
		{0,0,0,0,0,0,0,3,7,8},
		{0,0,0,0,0,0,1,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0}
	};
	const int PN = 4;				//模块数
	const int M = 3;				//每个模块最多任务数
	vector<int> finalCost;			//最终总代价

	//找出初始代价的最大代价
	int maxdis(vector<vector<int>> arr) {
		int maxdis = 0;
		for(int i = 0;i<arr.size();i++)
			for (int j = 0; j < arr.size(); j++) {
				if (arr[i][j] > maxdis)
					maxdis = arr[i][j];
			}
		return maxdis;
	}

	//计算通信距离矩阵
	vector<vector<int>> adjacency(vector<vector<int>> arr) {
		int maxd = maxdis(arr);
		for (int i = 0; i < arr.size(); i++)
			for (int j = i+1; j < arr.size(); j++) {
				arr[i][j] = maxd-arr[i][j]+1;
			}
		return arr;
	}

	//计算两个簇之间的最小通信距离
	int calculatedSmall(int i,int j,vector<vector<int>> vec,vector<vector<int>>a) {
		int small = INT_MAX;
		int left, right;
		if (vec.empty()) {
			return 0;
		}
		for (int m = 0; m < vec[i].size(); m++) {
			for (int n = 0; n < vec[j].size(); n++) {
				left = (vec[i][m] <= vec[j][n] ? vec[i][m] : vec[j][n]);
				right = (vec[i][m] > vec[j][n] ? vec[i][m] : vec[j][n]);
				int dis = a[left-1][right-1];
				if (dis <= small)
					small = dis;
			}
		}
		return small;
	}

	//计算两个簇之间的最大通信距离
	int calculatedLarge(int i, int j, vector<vector<int>> vec, vector<vector<int>>a) {
		int large = 0;
		int left, right;
		if (vec.empty()) {
			return 0;
		}
		for (int m = 0; m < vec[i].size(); m++) {
			for (int n = 0; n < vec[j].size(); n++) {
				left = (vec[i][m] <= vec[j][n] ? vec[i][m] : vec[j][n]);
				right = (vec[i][m] > vec[j][n] ? vec[i][m] : vec[j][n]);
				int dis = a[left - 1][right - 1];
				if (dis >= large)
					large = dis;
			}
		}
		return large;
	}

	//计算两个簇之间的平均通信距离
	double calculatedAvg(int i, int j, vector<vector<int>> vec, vector<vector<int>>a) {
		int avg = 0;
		int left, right;
		if (vec.empty()) {
			return 0;
		}
		for (int m = 0; m < vec[i].size(); m++) {
			for (int n = 0; n < vec[j].size(); n++) {
				left = (vec[i][m] <= vec[j][n] ? vec[i][m] : vec[j][n]);
				right = (vec[i][m] > vec[j][n] ? vec[i][m] : vec[j][n]);
				int dis = a[left - 1][right - 1];
				avg += dis;
			}
		}
		return (double)avg/(vec[i].size()*vec[j].size());
	}

	//将两个簇合并成一个簇
	vector<int> margin(vector<int>a, vector<int>b) {
		vector<int> rst(a);
		rst.insert(rst.end(), b.begin(), b.end());
		return rst;
	}

	//计算最终划分好的簇的总通信代价
	int finalcost(int i,int j,vector<vector<int>> vec,vector<vector<int>> a) {
		if (a.empty()||vec.empty())
			return INT_MAX;
		int rst = 0,left,right;
		for (int m = 0; m < vec[i].size(); m++) {
			for (int n = 0; n < vec[j].size(); n++) {
				left = min(vec[i][m],vec[j][n]);
				right = max(vec[i][m], vec[j][n]);
				rst += a[left - 1][right - 1];
			}
		}
			return rst;
	}

	//划分总体方法，返回最终状态<d,k,K>module
	Module partition() {
		int d = 0, k = N;
		int oldk;
		vector<vector<int>> K = { {1},{2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}, {10} };
		vector<vector<int>> A = adjacency(this->COST);

		Module module(d, k, K);
		do         //循环进行直到划分结束
		{
			oldk = k; d += 1;
			for (int i = 0; i < K.size()-1; i++) {
				for (int j = i+1; j < K.size(); j++) {
					//以下为单链接方法
					/*int smad = calculatedSmall(i,j,K,A);
					if (smad <= d && (K[i].size() + K[j].size() <= this->M)) {
						K.insert(K.begin() + i, margin(K[i],K[j]));
						K.erase(K.begin() + i+1);
						K.erase(K.begin() + j);
						k = oldk - 1;
						module.distance = d;
						module.modulenums = k;
						module.part = K;
					}*/

					//以下为全链接方法
					/*int lard = calculatedLarge(i,j,K,A);
					if (lard <= d && (K[i].size() + K[j].size() <= this->M)) {
						K.insert(K.begin() + i, margin(K[i],K[j]));
						K.erase(K.begin() + i+1);
						K.erase(K.begin() + j);
						k = oldk - 1;
						module.distance = d;
						module.modulenums = k;
						module.part = K;
					}*/

					//以下为均链接方法
					int avgd = calculatedAvg(i, j, K, A);
					if (avgd <= d && (K[i].size() + K[j].size() <= this->M)) {
						K.insert(K.begin() + i, margin(K[i], K[j]));
						K.erase(K.begin() + i + 1);
						K.erase(K.begin() + j);
						k = oldk - 1;
						module.distance = d;
						module.modulenums = k;
						module.part = K;
					}
					oldk = k;
				}
			}
		} while (k!=this->PN);

		//以下计算最终总代价
		for(int i=0;i<K.size()-1;i++)
			for(int j=i+1;j<K.size();j++)
				this->finalCost.push_back(finalcost(i,j,K,this->COST));

		return module;
	};

	
};


int main() {
	Solution solution;
	Module module = solution.partition();
	cout << "d: "<<module.distance << "   PN: " << module.modulenums << endl<<endl;
	
	//输出划分的簇
	for (int i = 0; i < module.part.size(); i++) {
		for (int j = 0; j < module.part[i].size(); j++) {
			cout << module.part[i][j]<<" ";
		}
		cout << endl;
	}
	cout << endl;

	//计算最终通信代价
	int totalCost = 0;
	for (int i = 0; i < solution.finalCost.size(); i++) {
		cout <<"cost"<<i+1<<" : "<< solution.finalCost[i] << " " << endl;
		totalCost += solution.finalCost[i];
	}
	cout << "total cost is : " << totalCost << endl;

	return 0;
}