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

typedef pair<Table*, int> pointer;   //具体对应哪个Table，并且是Table中的第几个索引; 

class SYNBL :public Table {
	struct Snode {
		string ID;
		pointer T;   //类型; 
		string c;    //种类; 
		pointer P;
	};

	vector<Snode> S;
public:
	SYNBL(string s)
	{
		Type = s;
	}

	void push(string ID, pointer T, string c, pointer P)  //封装的push函数; 
	{
		S.push_back({ ID,T,c,P });
	}

	int getLen()  //得到表的长度; 
	{
		return S.size();
	}

	void print()  //输出表的内容; 
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

//类型表; 
class TAPEL :public Table {
	struct Tnode {   //类型结点; 
		string type;   //表示结点类型;
		pointer p;    //如果是数组或者结构体类型，会指向新表; 
	};

	vector<Tnode> T;   //存放类型;
public:
	TAPEL(string s)
	{
		Type = s;
		pointer p;  //空pair 
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

//数组表; 
class AINFL :public Table {
	struct Anode {   //数组结点;
		int low;  //下届; 
		int up;   //上届;
		pointer CTP;  //类型(指针) 注:指向类型表; 
		int CLEN;  //每个单元长度; 
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

//结构体表; 
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

//长度表 
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


