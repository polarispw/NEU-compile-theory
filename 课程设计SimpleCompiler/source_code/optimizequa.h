#ifndef _optimizequa_H
#define _optimizequa_H
#include <iostream>
#include <cstring>
#include <stdio.h>
#include<stdlib.h>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

const int maxn = 1e2 + 5;

struct Opt {

struct token {
    string name;
    string type;     //2Ϊ��ʱ������3Ϊ���֣�1Ϊ����ʱ����
};

struct Quaternion {      //��Ԫʽ�ṹ��
    int block;           //�������ʶ
    int type;            //����-0 1 2 3
    string op;           //����
    token num1;         //������1
    token num2;         //������2
    token ans;          //���
};

struct ACT_INF //��Ԫʽ�Ļ�Ծ��Ϣ
{
	string num1;
	string num2;
	string ans;
	ACT_INF(string a="", string b="", string c="")
	{
		num1 = a; num2 = b; ans = c;
	}
};

struct ACT_SYNBL
{
	string name;
	string act_inf;
	ACT_SYNBL(string a="", string b="")
	{
		name = a; act_inf = b;
	}
};

vector< vector<ACT_INF> >Act_inf;
vector< vector<ACT_SYNBL> >Act_synbl;
vector<string>objcode;
stack<string>SEM; //����������ַ

struct Quaternion qua[maxn];   //��Ԫʽ����
vector<Quaternion> Block[15];
vector<vector<Quaternion>> ToTarget;
struct Node {                //DAG�ڵ�
    int left;
    int right;
    int id;
    string op;
    token m_sign;                    //�����
    token sign[maxn];                  //���ӱ��
};

struct DAG{                  //DAG
    int num=0;
    Node node[maxn];
}dag;

void init();     //��ʼ����Ԫʽ�ṹ������
void addition(int n,string A,string typ);   //�ѱ��A��ӵ��ڵ�n�ϣ�˳��ɾ�����ñ��
int getnode(string B);   //��ȡ���ΪB�Ľڵ��id
void makeleaf(string B,string typ);   //����Ҷ�ӽڵ�
void delnode(int n);     //ɾ��idΪn�Ľڵ�
int findnode(string opl,string B,string C);   //��DAG�в������޷���A=B op C�Ľڵ�
int makenode(string opl,string B,string C);  //�����м�ڵ�
string coutvalue(string opl,string B,string C);   //���������������ֵ
void makequa(int block);   //����block�����Ż������Ԫʽ
void deletedag();  //��dag����
void optimizequa(int block_num);    //�Ż���Ԫʽ
int divide();             //���ֻ�����
void run();

int to_int(string str)
{
	int res = 0;
	for (int j = 0; j < (int)str.size(); j++)
	{
		res = res * 10 + (str[j] - 48);
	}
	return res;
}

bool is_inActsynbl(token token1,vector<ACT_SYNBL>act_synbl)
{
	bool flag = 0;
	for (int i = 0; i < (int)act_synbl.size(); i++)
	{
		if (token1.name == act_synbl[i].name) {flag = 1; break;}
	}
	return flag;
}

//���Ծ��Ϣ�����������Ҫ������Ԫʽvector����ʱ����set�ͷ���ʱ����set
void get_act_inf(vector< vector<Quaternion> >qua)
{
	/*int num = (int)Act_inf.size();
	for (set<string>::iterator iter = Tmp_var.begin(); iter != Tmp_var.end(); iter++)
	{
		ACT_SYNBL act_synbl(*iter, "N");
		Act_synbl.push_back(act_synbl);
	}

	for (set<string>::iterator iter = var.begin(); iter != var.end(); iter++)
	{
		ACT_SYNBL act_synbl(*iter, "Y");
		Act_synbl.push_back(act_synbl);
	}

	for (int i = (int)qua.size() - 1; i >= 0; i--)
	{
		string str1="~", str2="~", str3="~";
		for (int j = 0; j < (int)Act_synbl.size(); j++)
		{
			if (qua[i].num1.name == Act_synbl[j].name)
			{
				str1 = Act_synbl[j].act_inf; Act_synbl[j].act_inf = "Y";
			}
			if (qua[i].num2.name == Act_synbl[j].name)
			{
				str2 = Act_synbl[j].act_inf; Act_synbl[j].act_inf = "Y";
			}
			if (qua[i].ans.name == Act_synbl[j].name)
			{
				str3 = Act_synbl[j].act_inf; Act_synbl[j].act_inf = "N";
			}
		}

		Act_inf.push_back(ACT_INF(str1, str2, str3));
	}
	reverse(Act_inf.begin()+num, Act_inf.end());//����һ��

	for (int i = 0; i < (int)Act_inf.size(); i++)
	{
		cout << "(" + Act_inf[i].num1 + " , " + Act_inf[i].num2 + " , " + Act_inf[i].ans + ")" << endl;
	}*/

	//��ʼ��Act_synbl����ʱ����N������ʱ����Y
	for (int i = 0; i < (int)qua.size(); i++)
	{
		vector<ACT_SYNBL>act_synbl;
		for (int j = 0; j < (int)qua[i].size(); j++)
		{
			//num1��Ϊ���Ҳ�������ʱ
			if (qua[i][j].num1.name != "" && qua[i][j].num1.type!="3")
			{
				if (!is_inActsynbl(qua[i][j].num1, act_synbl)) //���ڱ���
				{
					//��ǰ�Ƿ���ʱ����
					if (qua[i][j].num1.type == "1")
					{
						ACT_SYNBL tmp(qua[i][j].num1.name, "Y");
						act_synbl.push_back(tmp);
					}
					else if (qua[i][j].num1.type == "2")//��ǰ����ʱ����
					{
						ACT_SYNBL tmp(qua[i][j].num1.name, "N");
						act_synbl.push_back(tmp);
					}
				}
			}

			//num2��Ϊ���Ҳ�������ʱ
			if (qua[i][j].num2.name != "" && qua[i][j].num2.type != "3")
			{
				if (!is_inActsynbl(qua[i][j].num2, act_synbl)) //���ڱ���
				{
					//��ǰ�Ƿ���ʱ����
					if (qua[i][j].num2.type == "1")
					{
						ACT_SYNBL tmp(qua[i][j].num2.name, "Y");
						act_synbl.push_back(tmp);
					}
					else if (qua[i][j].num2.type == "2")//��ǰ����ʱ����
					{
						ACT_SYNBL tmp(qua[i][j].num2.name, "N");
						act_synbl.push_back(tmp);
					}
				}
			}

			//ans��Ϊ���Ҳ�������ʱ
			if (qua[i][j].ans.name != "" && qua[i][j].ans.type != "3")
			{
				if (!is_inActsynbl(qua[i][j].ans, act_synbl)) //���ڱ���
				{
					//��ǰ�Ƿ���ʱ����
					if (qua[i][j].ans.type == "1")
					{
						ACT_SYNBL tmp(qua[i][j].ans.name, "Y");
						act_synbl.push_back(tmp);
					}
					else if (qua[i][j].ans.type == "2")//��ǰ����ʱ����
					{
						ACT_SYNBL tmp(qua[i][j].ans.name, "N");
						act_synbl.push_back(tmp);
					}
				}
			}
		}

		Act_synbl.push_back(act_synbl);
	}

	//for (int i = 0; i < (int)qua.size(); i++)
	//{
		//���act_synbl
	//	for (int k = 0; k < (int)Act_synbl[i].size(); k++)
	//	{
	//		cout << i;
	//		cout << "(" + Act_synbl[i][k].name + " , " + Act_synbl[i][k].act_inf << endl;
	//	}
	//}

	//����Ԫʽ�Ļ�Ծ��Ϣ
	for (int i = 0; i < (int)qua.size(); i++)
	{
		vector<ACT_INF>act_inf;
		for (int j = (int)qua[i].size()-1; j >=0; j--)
		{
			ACT_INF tmp;
			for (int k = 0; k < (int)Act_synbl[i].size(); k++)
			{
				if (qua[i][j].num1.name == Act_synbl[i][k].name)
				{
					tmp.num1 = Act_synbl[i][k].act_inf;
					Act_synbl[i][k].act_inf = "Y";
				}

				if (qua[i][j].num2.name == Act_synbl[i][k].name)
				{
					tmp.num2 = Act_synbl[i][k].act_inf;
					Act_synbl[i][k].act_inf = "Y";
				}

				if (qua[i][j].ans.name == Act_synbl[i][k].name)
				{
					tmp.ans = Act_synbl[i][k].act_inf;
					Act_synbl[i][k].act_inf = "N";
				}
			}
			act_inf.push_back(tmp);
		}
		//Ԫ�ط�תһ��
		reverse(act_inf.begin(), act_inf.end());
		Act_inf.push_back(act_inf);
	}

	//���
	//for (int i = 0; i < (int)qua.size(); i++)
	//{
	//	for (int j = 0; j < (int)Act_inf[i].size(); j++)
	//	{
	//		cout << i;
	//		cout <<"(" + Act_inf[i][j].num1 + " , " + Act_inf[i][j].num2 + " , " + Act_inf[i][j].ans + ")" << endl;
	//	}
	//}
}

//Ŀ���������
void to_objcode(vector< vector<Quaternion> >qua, vector< vector<ACT_INF> >Act_inf)
{

	for (int i = 0; i < (int)qua.size(); i++)
	{
		int wh_id = 0;
		ACT_SYNBL Rdl("", ""); //��ʼ��R�Ĵ���������
		for (int j = 0; j < (int)qua[i].size(); j++)
		{
			// ˫���������������������ɻ���λ��
			if (qua[i][j].op == "-" || qua[i][j].op == "/" || qua[i][j].op == "!=" ||
				qua[i][j].op == ">=" || qua[i][j].op == "<=" || qua[i][j].op == ">" ||
				qua[i][j].op == "<")
			{
				if (Rdl.name == "") //��ǰ�Ĵ�������
				{
					string tmp1 = "LD R," + qua[i][j].num1.name;
					objcode.push_back(tmp1);
				}
				else //��ǰ�Ĵ���������
				{
					if (Rdl.name != qua[i][j].num1.name) //��һ�����������ڼĴ�����
					{
						string tmp1, tmp2;
						if (Rdl.act_inf == "Y")
						{
							tmp1 = "ST R," + Rdl.name; objcode.push_back(tmp1);
							tmp2 = "LD R," + qua[i][j].num1.name;
							objcode.push_back(tmp2);
						}
						else
						{
							string tmp1 = "LD R," + qua[i][j].num1.name;
							objcode.push_back(tmp1);
						}
					}
				}

				string tmp2;
				//���¼Ĵ���״̬�Ļ�Ծ��Ϣ
				Rdl.act_inf = Act_inf[i][j].num1;

				if (qua[i][j].op == "-")tmp2 = "SUB R," + qua[i][j].num2.name;
				else if (qua[i][j].op == "/")tmp2 = "DIV R," + qua[i][j].num2.name;
				else if (qua[i][j].op == "!=")tmp2 = "NE R," + qua[i][j].num2.name;
				else if (qua[i][j].op == ">=")tmp2 = "GE R," + qua[i][j].num2.name;
				else if (qua[i][j].op == "<=")tmp2 = "LE R," + qua[i][j].num2.name;
				else if (qua[i][j].op == ">")tmp2 = "GT R," + qua[i][j].num2.name;
				else if (qua[i][j].op == "<")tmp2 = "LT R," + qua[i][j].num2.name;

				objcode.push_back(tmp2);
				Rdl.name = qua[i][j].ans.name; Rdl.act_inf = Act_inf[i][j].ans;//�޸ļĴ���������
			}
			else if (qua[i][j].op == "+" || qua[i][j].op == "*") // +��*�������������������Ի���λ��
			{
				//��ǰ�Ĵ�����ò������޹�
				if (Rdl.name != "" &&Rdl.name != qua[i][j].num1.name && Rdl.name != qua[i][j].num2.name)
				{
					if (Rdl.act_inf == "Y")
					{
						string tmp1 = "ST R," + Rdl.name; objcode.push_back(tmp1);
					}
				}
				else
				{
					string tmp1, tmp2;
					if (Rdl.name == qua[i][j].num1.name)
					{
						//���¼Ĵ���״̬�Ļ�Ծ��Ϣ
						Rdl.act_inf = Act_inf[i][j].num1;

						if (qua[i][j].op == "*") { tmp1 = "MUL R,"; }
						else if (qua[i][j].op == "+") { tmp1 = "ADD R,"; }

						objcode.push_back(tmp1 + qua[i][j].num2.name);
					}
					else if (Rdl.name == qua[i][j].num2.name)
					{
						//���¼Ĵ���״̬�Ļ�Ծ��Ϣ
						Rdl.act_inf = Act_inf[i][j].num1;

						if (qua[i][j].op == "*") { tmp1 = "MUL R,"; }
						else if (qua[i][j].op == "+") { tmp1 = "ADD R,"; }

						objcode.push_back(tmp1 + qua[i][j].num1.name);
					}
					else
					{
						tmp1 = "LD R," + qua[i][j].num1.name;
						objcode.push_back(tmp1);
						if (qua[i][j].op == "*") { tmp2 = "MUL R,"; }
						else if (qua[i][j].op == "+") { tmp2 = "ADD R,"; }

						objcode.push_back(tmp2 + qua[i][j].num2.name);
					}
				}

				Rdl.name = qua[i][j].ans.name; Rdl.act_inf = Act_inf[i][j].ans;//�޸ļĴ���������
			}
			else if (qua[i][j].op == "if" || qua[i][j].op == "el" || qua[i][j].op == "ie")
			{
				//���¼Ĵ���״̬�Ļ�Ծ��Ϣ
				if(qua[i][j].op=="if")Rdl.act_inf = Act_inf[i][j].num1;
				if (Rdl.act_inf == "Y") { objcode.push_back("ST R," + Rdl.name); Rdl.act_inf = "N"; }

				string tmp;
				if (qua[i][j].op == "if") { tmp = "FJ R,"; objcode.push_back(tmp); }
				else if (qua[i][j].op == "el") { tmp = "JMP "; objcode.push_back(tmp); }
				//��ǰSEMջ��Ϊ��ʱ������ջ����
				if (!SEM.empty())
				{
					string tmp = SEM.top();
					SEM.pop();

					objcode[to_int(tmp)] = objcode[to_int(tmp)] + to_string(objcode.size());
				}
				if(qua[i][j].op != "ie")SEM.push(to_string(objcode.size()-1));

			}
			else if (qua[i][j].op == "wh" || qua[i][j].op == "do" || qua[i][j].op == "we")
			{
				//���¼Ĵ���״̬�Ļ�Ծ��Ϣ
				if (qua[i][j].op == "do")Rdl.act_inf = Act_inf[i][j].num1;
				if (Rdl.act_inf == "Y") { objcode.push_back("ST R," + Rdl.name); Rdl.act_inf = "N"; }

				string tmp;
				if (qua[i][j].op == "wh") { wh_id = (int)objcode.size(); }
				else if (qua[i][j].op == "do") { tmp = "FJ R,"; objcode.push_back(tmp); }
				else if (qua[i][j].op == "we") { tmp = "JMP "+to_string(wh_id); objcode.push_back(tmp); }
				//��ǰSEMջ��Ϊ��ʱ������ջ����
				if (!SEM.empty())
				{
					string tmp = SEM.top();
					SEM.pop();

					objcode[to_int(tmp)] = objcode[to_int(tmp)] + to_string(objcode.size());
				}
				if (qua[i][j].op == "do")SEM.push(to_string(objcode.size() - 1));

			}
			else if (qua[i][j].op == "lb" || qua[i][j].op == "gt")
			{
				string tmp;
				if (qua[i][j].op == "lb") { tmp = qua[i][j].ans.name + ":"; }
				else if (qua[i][j].op == "gt") { tmp = "JMP " + qua[i][j].ans.name; }

				objcode.push_back(tmp);
			}
			else if (qua[i][j].op == "=")
			{
				string tmp1,tmp2;
				if (Rdl.name != qua[i][j].num1.name)//�Ĵ����в��ǵ�ǰ������
				{
					if (Rdl.act_inf == "Y") { tmp1 = "ST R," + Rdl.name; objcode.push_back(tmp1); }
					tmp2 = "LD R," + qua[i][j].num1.name;
					objcode.push_back(tmp2);
				}

				Rdl.act_inf = Act_inf[i][j].num1; //���¼Ĵ�����Ծ��Ϣ

				Rdl.name = qua[i][j].ans.name; Rdl.act_inf = Act_inf[i][j].ans;
			}
			else //�������
			{

			}
		}
		if (Rdl.act_inf == "Y") { objcode.push_back("ST R," + Rdl.name); }
	}

	//���Ŀ�����
	for (int i = 0; i < (int)objcode.size(); i++)
	{
		cout <<std::left<<setw(10)<< i << std::left << setw(10) << objcode[i] << endl;
	}
}

void get_objcode()
{
	get_act_inf(ToTarget);
	to_objcode(ToTarget, Act_inf);
}

};

#endif // _optimizequa_H
