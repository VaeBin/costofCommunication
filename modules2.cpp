#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
using namespace std;
const int N = 8;			//��ʼ����Ŀ����������������
const int M = 4;			//Ҫ���ÿһ���ص�������

//<d,k,K>��Ԫ��״̬
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

//����Ӧ��int��moduleת����char��module1
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

//�������
class Solution {
public:
	const int D[8] = { 1,2,3,4,5,6,7,8};			//��{'a','b','c','d','e','f','g','h'}����ӦΪ{1��2��3��4��5��6��7��8}
	const vector<vector<double>> COST = {			//��ʼͨ�Ŵ���
		{0,0,0.5,0,0.5,0,0,0},
		{0,0,0.5,0.5,0,0,0,0},
		{0.5,0.5,0,0.5,1,0.5,0,0},
		{0,0.5,0.5,0,0,1,0,0},
		{0.5,0,1,0,0,0.5,1,0},
		{0,0,0.5,1,0.5,0,0.5,0.5},
		{0,0,0,0,1,0.5,0,0.5},
		{0,0,0,0,0,0.5,0.5,0}
	};
	const int PN = 2;			//����Ҫ��Ĵ���
	double maxdis = 1;			//���ͨ�Ŵ���
	vector<double> finalCost;			//���մ�֮���ͨ�Ŵ�������

	//����ͨ�ž������
	vector<vector<double>> adjacency(vector<vector<double>> arr) {
		for (int i = 0; i < arr.size(); i++)
			for (int j = 0; j < arr.size(); j++) {
				arr[i][j] = this->maxdis - arr[i][j] + 1;
			}
		return arr;
	}

	//����������֮�����С����
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

	//����������֮������ͨ�ž���
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

	//����������֮���ƽ��ͨ�ž���
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

	//�ϲ�������Ϊһ����
	vector<int> margin(vector<int>a, vector<int>b) {
		vector<int> rst(a);
		rst.insert(rst.end(), b.begin(), b.end());
		return rst;
	}

	//�������ջ��ֺõĴ�֮���ͨ�Ŵ���
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

	//���廮�ַ������
	Module partition() {
		double d = 0;
		int k = N;
		int oldk;
		vector<vector<int>> K = { {1},{2}, {3}, {4}, {5}, {6}, {7}, {8} };			//��ʼ�Ĵأ�123...��Ӧ��abc....
		vector<vector<double>> A = adjacency(this->COST);			//�������

		Module module(d, k, K);			//��ʼ����Ԫ״̬<d,k,K>
		do				//���ϻ���ֱ������2����ÿ����4������
		{
			oldk = k; d += 0.5;
			for (int i = 0; i < K.size() - 1; i++) {
				for (int j = i + 1; j < K.size(); j++) {
					//����Ϊ������
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

					//����Ϊȫ����
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

					//����Ϊ������
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

		//�������ջ��ֺõĴ�֮����ܴ���
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
	
	//���ֺõ������أ������������Ӧ�任Ϊ��ĸ����
	for (int i = 0; i < module.part.size(); i++) {
		for (int j = 0; j < module.part[i].size(); j++) {
		
			ch[i][j] = (char)(module.part[i][j]-1+97);
			cout << module.part[i][j]<<" "<<ch[i][j]<<endl;
		}
		cout << endl;
	}
	
	//����ͨ�Ŵ���
	double totalCost = 0;
	for (int i = 0; i < solution.finalCost.size(); i++) {
		totalCost += solution.finalCost[i];
	}
	cout << "total cost is : " << totalCost << endl;

	return 0;
}