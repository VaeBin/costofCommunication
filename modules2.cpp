#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
using namespace std;
const int N = 8;			//初始簇数目（或者任务总数）
const int M = 4;			//要求的每一个簇的任务数

//<d,k,K>三元组状态
class Module {
public:
	double distance;
	int modulenums;
	vector<vector<int>> part;
	Module() :distance(0), modulenums(0) {};
	Module(int distance, int modulenums, vector<vector<int>> part) {
		this->distance = distance;
		this->modulenums = modulenums;
		this->part = part;
	}
};

//将对应的int型module转换成char型module1
class Module1 {
public:
	double distance;
	int modulenums;
	vector<vector<char>> part;
	Module1() :distance(0), modulenums(0) {};
	Module1(int distance, int modulenums, vector<vector<char>> part) {
		this->distance = distance;
		this->modulenums = modulenums;
		this->part = part;
	}
};

//解决方案
class Solution {
public:
	const int D[8] = { 1,2,3,4,5,6,7,8};			//将{'a','b','c','d','e','f','g','h'}简便对应为{1，2，3，4，5，6，7，8}
	const vector<vector<double>> COST = {			//初始通信代价
		{0,0,0.5,0,0.5,0,0,0},
		{0,0,0.5,0.5,0,0,0,0},
		{0.5,0.5,0,0.5,1,0.5,0,0},
		{0,0.5,0.5,0,0,1,0,0},
		{0.5,0,1,0,0,0.5,1,0},
		{0,0,0.5,1,0.5,0,0.5,0.5},
		{0,0,0,0,1,0.5,0,0.5},
		{0,0,0,0,0,0.5,0.5,0}
	};
	const int PN = 2;			//最终要求的簇数
	double maxdis = 1;			//最大通信代价
	vector<double> finalCost;			//最终簇之间的通信代价容器

	//计算通信距离矩阵
	vector<vector<double>> adjacency(vector<vector<double>> arr) {
		for (int i = 0; i < arr.size(); i++)
			for (int j = 0; j < arr.size(); j++) {
				arr[i][j] = this->maxdis - arr[i][j] + 1;
			}
		return arr;
	}

	//计算两个簇之间的最小距离
	double calculatedSmall(int i, int j, vector<vector<int>> vec, vector<vector<double>>a) {
		double small = INT_MAX;
		if (vec.empty()) {
			return 0;
		}
		for (int m = 0; m < vec[i].size(); m++) {
			for (int n = 0; n < vec[j].size(); n++) {
				double dis = a[vec[i][m] - 1][vec[j][n] - 1];
				if (dis <= small)
					small = dis;
			}
		}
		return small;
	}

	//计算两个簇之间的最大通信距离
	double calculatedLarge(int i, int j, vector<vector<int>> vec, vector<vector<double>>a) {
		double large = 0;
		if (vec.empty()) {
			return 0;
		}
		for (int m = 0; m < vec[i].size(); m++) {
			for (int n = 0; n < vec[j].size(); n++) {
				double dis = a[vec[i][m] - 1][vec[j][n] - 1];
				if (dis >= large)
					large = dis;
			}
		}
		return large;
	}

	//计算两个簇之间的平均通信距离
	double calculatedAvg(int i, int j, vector<vector<int>> vec, vector<vector<double>>a) {
		double avg = 0;
		if (vec.empty()||a.empty()) {
			return 0;
		}
		for (int m = 0; m < vec[i].size(); m++) {
			for (int n = 0; n < vec[j].size(); n++) {
				double dis = a[vec[i][m]-1][vec[j][n]-1];
				avg += dis;
			}
		}//{{3,5,2}{4,5}{6,7}}
		return avg / (vec[i].size() * vec[j].size());
	}

	//合并两个簇为一个簇
	vector<int> margin(vector<int>a, vector<int>b) {
		vector<int> rst(a);
		rst.insert(rst.end(), b.begin(), b.end());
		return rst;
	}

	//计算最终划分好的簇之间的通信代价
	double finalcost(int i, int j, vector<vector<int>> vec, vector<vector<double>> a) {
		if (a.empty() || vec.empty())
			return INT_MAX;
		double rst = 0;
		for (int m = 0; m < vec[i].size(); m++) {
			for (int n = 0; n < vec[j].size(); n++) {
				rst += a[vec[i][m]-1][vec[j][n]-1];
			}
		}
		return rst;
	}

	//总体划分方法入口
	Module partition() {
		double d = 0;
		int k = N;
		int oldk;
		vector<vector<int>> K = { {1},{2}, {3}, {4}, {5}, {6}, {7}, {8} };			//初始的簇：123...对应着abc....
		vector<vector<double>> A = adjacency(this->COST);			//距离矩阵

		Module module(d, k, K);			//初始化三元状态<d,k,K>
		do				//不断划分直到满足2个簇每个簇4个任务
		{
			oldk = k; d += 0.5;
			for (int i = 0; i < K.size() - 1; i++) {
				for (int j = i + 1; j < K.size(); j++) {
					//以下为单链接
					/*double smad = calculatedSmall(i,j,K,A);
					if (smad <= d && (K[i].size() + K[j].size() <= M)) {
						K.insert(K.begin() + i, margin(K[i],K[j]));
						K.erase(K.begin() + i+1);
						K.erase(K.begin() + j);
						k = oldk - 1;
						module.distance = d;
						module.modulenums = k;
						module.part = K;
					}*/

					//以下为全链接
					double lard = calculatedLarge(i,j,K,A);
					if (lard <= d && (K[i].size() + K[j].size() <= M)) {
						K.insert(K.begin() + i, margin(K[i],K[j]));
						K.erase(K.begin() + i+1);
						K.erase(K.begin() + j);
						k = oldk - 1;
						module.distance = d;
						module.modulenums = k;
						module.part = K;
					}

					//以下为均链接
					/*double avgd = calculatedAvg(i, j, K, A);
					if (avgd <= d && (K[i].size() + K[j].size() <= M)) {
						K.insert(K.begin() + i, margin(K[i], K[j]));
						K.erase(K.begin() + i + 1);
						K.erase(K.begin() + j);
						k = oldk - 1;
						module.distance = d;
						module.modulenums = k;
						module.part = K;
					}*/
					oldk = k;
				}
			}
		} while (k != this->PN);

		//计算最终划分好的簇之间的总代价
		for (int i = 0; i < K.size() - 1; i++)
			for (int j = i + 1; j < K.size(); j++)
				this->finalCost.push_back(finalcost(i, j, K, this->COST));

		return module;
	};


};


int main() {
	Solution solution;
	Module module = solution.partition();
	const int SIZE = module.part.size();
	char ch[2][M];
	cout << "d: " << module.distance << "   PN: " << module.modulenums << endl << endl;
	
	//划分好的两个簇，将数字任务对应变换为字母任务
	for (int i = 0; i < module.part.size(); i++) {
		for (int j = 0; j < module.part[i].size(); j++) {
		
			ch[i][j] = (char)(module.part[i][j]-1+97);
			cout << module.part[i][j]<<" "<<ch[i][j]<<endl;
		}
		cout << endl;
	}
	
	//最终通信代价
	double totalCost = 0;
	for (int i = 0; i < solution.finalCost.size(); i++) {
		totalCost += solution.finalCost[i];
	}
	cout << "total cost is : " << totalCost << endl;

	return 0;
}