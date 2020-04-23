#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
using namespace std;
const int N = 10;        //��ʼkֵ

//Module��Ϊ<d,k,K>��Ԫ�洢
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

//�����������
class Solution {
public:
	const int D[10] = { 1,2,3,4,5,6,7,8,9,10 };			//���������Ŵ�1��10
	const vector<vector<int>> COST= {					//��ʼ�������
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
	const int PN = 4;				//ģ����
	const int M = 3;				//ÿ��ģ�����������
	vector<int> finalCost;			//�����ܴ���

	//�ҳ���ʼ���۵�������
	int maxdis(vector<vector<int>> arr) {
		int maxdis = 0;
		for(int i = 0;i<arr.size();i++)
			for (int j = 0; j < arr.size(); j++) {
				if (arr[i][j] > maxdis)
					maxdis = arr[i][j];
			}
		return maxdis;
	}

	//����ͨ�ž������
	vector<vector<int>> adjacency(vector<vector<int>> arr) {
		int maxd = maxdis(arr);
		for (int i = 0; i < arr.size(); i++)
			for (int j = i+1; j < arr.size(); j++) {
				arr[i][j] = maxd-arr[i][j]+1;
			}
		return arr;
	}

	//����������֮�����Сͨ�ž���
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

	//����������֮������ͨ�ž���
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

	//����������֮���ƽ��ͨ�ž���
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

	//�������غϲ���һ����
	vector<int> margin(vector<int>a, vector<int>b) {
		vector<int> rst(a);
		rst.insert(rst.end(), b.begin(), b.end());
		return rst;
	}

	//�������ջ��ֺõĴص���ͨ�Ŵ���
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

	//�������巽������������״̬<d,k,K>module
	Module partition() {
		int d = 0, k = N;
		int oldk;
		vector<vector<int>> K = { {1},{2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}, {10} };
		vector<vector<int>> A = adjacency(this->COST);

		Module module(d, k, K);
		do         //ѭ������ֱ�����ֽ���
		{
			oldk = k; d += 1;
			for (int i = 0; i < K.size()-1; i++) {
				for (int j = i+1; j < K.size(); j++) {
					//����Ϊ�����ӷ���
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

					//����Ϊȫ���ӷ���
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

					//����Ϊ�����ӷ���
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

		//���¼��������ܴ���
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
	
	//������ֵĴ�
	for (int i = 0; i < module.part.size(); i++) {
		for (int j = 0; j < module.part[i].size(); j++) {
			cout << module.part[i][j]<<" ";
		}
		cout << endl;
	}
	cout << endl;

	//��������ͨ�Ŵ���
	int totalCost = 0;
	for (int i = 0; i < solution.finalCost.size(); i++) {
		cout <<"cost"<<i+1<<" : "<< solution.finalCost[i] << " " << endl;
		totalCost += solution.finalCost[i];
	}
	cout << "total cost is : " << totalCost << endl;

	return 0;
}