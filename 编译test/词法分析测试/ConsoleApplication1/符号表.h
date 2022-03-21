#pragma once
#include<cstdio>
#include<cstring>
#include<iostream>
#include<utility>
#include<vector>
using namespace std;

class Table {
public:
	string Type;

	Table(string s)
	{
		Type = s;
	}

	Table()
	{
	}
};

typedef pair<Table*, int> pointer;   //�����Ӧ�ĸ�Table��������Table�еĵڼ�������; 

class SYNBL :public Table {
	struct Snode {
		string ID;
		pointer T;   //����; 
		string c;    //����; 
		pointer P;
	};

	vector<Snode> S;
public:
	SYNBL(string s)
	{
		Type = s;
	}

	void push(string ID, pointer T, string c, pointer P)  //��װ��push����; 
	{
		S.push_back({ ID,T,c,P });
	}

	int getLen()  //�õ���ĳ���; 
	{
		return S.size();
	}

	void print()  //����������; 
	{
		cout << Type << ':' << endl;

		for (auto it : S)
			cout << it.ID << " " << '(' << it.T.first->Type << ',' << it.T.second << ')' << ' ' << it.c << ' ' << '(' << it.P.first->Type << ',' << it.P.second << ')' << endl;
	}
	int exist(string str)
	{
		for (auto it : S)
		{
			if (str == it.ID)
				return 0;
		}
		return 1;
	}
};

//���ͱ�; 
class TAPEL :public Table {
	struct Tnode {   //���ͽ��; 
		string type;   //��ʾ�������;
		pointer p;    //�����������߽ṹ�����ͣ���ָ���±�; 
	};

	vector<Tnode> T;   //�������;
public:
	TAPEL(string s)
	{
		Type = s;
		pointer p;  //��pair 
		T.push_back({ "i",p });
		T.push_back({ "r",p });
		T.push_back({ "c",p });
		T.push_back({ "b",p });
	}

	void push(string type, pointer p)
	{
		T.push_back({ type,p });
	}

	int getLen()
	{
		return T.size();
	}

	void print()
	{
		cout << Type << ':' << endl;

		for (auto it : T)
		{
			if (it.p.first != NULL)
				cout << it.type << ' ' << '(' << it.p.first->Type << ' ' << it.p.second << ')' << endl;
			else
				cout << it.type << endl;
		}
	}
};

//�����; 
class AINFL :public Table {
	struct Anode {   //������;
		int low;  //�½�; 
		int up;   //�Ͻ�;
		pointer CTP;  //����(ָ��) ע:ָ�����ͱ�; 
		int CLEN;  //ÿ����Ԫ����; 
	};
	vector<Anode> A;
public:

	AINFL(string s)
	{
		Type = s;
	}

	void push(int low, int up, pointer CTP, int CLEN)
	{
		A.push_back({ low,up,CTP,CLEN });
	}

	int getLen()
	{
		return A.size();
	}

	void print()
	{
		cout << Type << ':' << endl;

		for (auto it : A)
		{
			cout << it.low << ' ' << it.up << ' ' << '(' << it.CTP.first->Type << ',' << it.CTP.second << ')' << it.CLEN << endl;
		}
	}
};

//�ṹ���; 
class RINFL :public Table {
	struct Rnode {
		string ID;
		int OFF;
		pointer TP;
	};

	vector<Rnode> R;
public:
	RINFL(string s)
	{
		Type = s;
	}

	void push(string ID, int OFF, pointer TP)
	{
		R.push_back({ ID,OFF,TP });
	}

	int getLen()
	{
		return R.size();
	}

	void print()
	{
		cout << Type << ':' << endl;

		for (auto it : R)
		{
			cout << it.ID << ' ' << it.OFF << ' ' << '(' << it.TP.first->Type << ',' << it.TP.second << ')' << endl;
		}
	}
};

//���ȱ� 
class LENTH :public Table {
	vector<int> L;
public:
	LENTH(string s)
	{
		Type = s;
	}

	void push(int t)
	{
		L.push_back(t);
	}

	int getLen()
	{
		return L.size();
	}

	void print()
	{
		for (auto it : L)
			cout << it << endl;
	}
};


