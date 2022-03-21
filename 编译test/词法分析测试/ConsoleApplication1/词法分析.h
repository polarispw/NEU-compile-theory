#ifndef HEADER1_H_INCLUDED
#define HEADER1_H_INCLUDED
#include <iostream>
#include <fstream>
#include <string>
#include <ctype.h>
#include <vector>
#include <algorithm>
#include <iomanip>
#include "���ű�.h"
using namespace std;

#define MAXRESERVED 33   //����ؼ��ֵ���Ŀ ������33���ؼ���
#define MAXSPECIALSYMBOLS 22 //����������Ŀ ������22��

vector<string> iT;  //��ʶ����
vector<string> cT;  //�ַ���
vector<string> sT;  //�ַ�����
vector<string> CT;  //������

//����������״̬ ��ʼ ��� ע�� ���� ��ʶ�� �ַ� �ַ���
typedef enum
{
    START,DONE,ISCOMMENT,ISNUM,ISID,ISCHAR,ISSTRING
}StateType;

typedef enum
{
    AUTO, SHORT, INT, LONG, FLOAT, DOUBLE, CHAR, STRUCT, STRING,
    ENUM, TYPEDEF, CONST, UNSIGNED, SIGNED, EXTERN, REGISTER, STATIC, VOLATILE,
    VOID, IF, ELSE, SWITCH, CASE, FOR, DO, WHILE, GOTO, CONTINUE, BREAK, DEFAULT,
    SIZEOF, RETURN, MAIN,
    ID, NUM, ICHAR, ISTRING, /* multicharacter tokens */
    /* special symbols */
    PLUS, MINUS, MTPLUS, DEVIDE, EQ, ASSIGN, LT, MT, TIMES, DOT, SQ, DQ, LXKH, RXKH, LZKH, RZKH, LDKH, RDKH, SEMI, LE, ME, NOTEQ, FH, LZS, RZS, ZS,
    ENDFILE, UNKNOWN, ERROR,/*stateToken */
    I
}TokenType;

struct Symbols {
    string str;
    TokenType tt;
}specialSymbols[MAXSPECIALSYMBOLS] = { {"+",PLUS},{"-",MINUS},{"*",MTPLUS},{"/",DEVIDE},{"<",LT},{"<=",LE},{">",MT},
{">=",ME},{"==",EQ},{"!=",NOTEQ},{"=",ASSIGN},{",",SEMI},{";",FH},{".",DOT},{"(",LXKH},
{")",RXKH},{"[",LZKH},{"]",RZKH},{"{",LDKH},{"}",RDKH},{"/*",LZS},{"*/",RZS} };   //�����

struct keyWords {
    string str;
    TokenType tt;
}reservedWords[MAXRESERVED] = { {"auto",AUTO},{"short",SHORT},{"int",INT},{"long",LONG},{"float",FLOAT},{"double",DOUBLE},
{"char",CHAR},{"struct",STRUCT},{"string",STRING},{"enum",ENUM},{"typedef",TYPEDEF},{"const",CONST},{"unsigned",UNSIGNED},
{"signed",SIGNED},{"extern",EXTERN},{"register",ELSE},{"static",STATIC},{"volatile",VOLATILE},{"void",VOID},{"if",IF},
{"else",ELSE},{"switch",SWITCH},{"case",CASE},{"for",FOR},{"do",DO},{"while",WHILE},{"goto",GOTO},{"continue",CONTINUE},
{"break",BREAK},{"default",DEFAULT},{"sizeof",SIZEOF},{"return",RETURN},{"main",MAIN} };  //�ؼ��ֱ�

SYNBL i("initial");
pointer  init(&i,0);

void fillname(string str, SYNBL &ss);

void outputtoken(TokenType tt, string str, SYNBL &ss);

void GetToken(ifstream& inf, SYNBL &ss, int& lineno);

void showconsttable()
{
    cout << "�ַ���������" << endl;
    for (int i = 0; i < size(cT); i++) 
    {
        cout << i + 1 << " " << cT[i] << endl;
    }
    cout << "�ַ�����������"<<endl;
    for (int i = 0; i < size(sT); i++)
    {
        cout << i + 1 << " " << sT[i] << endl;
    }
    cout << "���ֳ�������" << endl;
    for (int i = 0; i < size(CT); i++)
    {
        cout << i + 1 << " " << CT[i] << endl;
    }
}

void fillname(string str, SYNBL &ss)
{
    if (ss.exist(str))
    {
        ss.push(str, init, "#", init);
    }
}

void outputtoken(TokenType tt, string str, SYNBL &ss)  //����ű���дToken����
{
    
    /*for (int i = 0; i < MAXRESERVED; i++)   //�ж�Token�����Ƿ�Ϊ�ؼ���
    {
        if (reservedWords[i].str == str)
        {
            tt = reservedWords[i].tt;
            if (i < 9)
                cout << str << "  :   " << "t" << endl;
            else
                cout << str << "  :   " << "k" << endl;
        }
    }
    for (int j = 0; j < MAXSPECIALSYMBOLS; j++)   //�ж�Token�����Ƿ�Ϊ���
    {
        if (specialSymbols[j].tt == tt)
            cout << str << "  :   " << str << endl;

    }
    if (tt == ID)            //�Ǳ�ʶ��
    {
        fillname(str, ss);
        cout << str << "  :   " << "i" << endl;
    }
    if (tt == NUM)           //�ǳ���
    {
        fillname(str, ss);
        cout << str << "  :   " << "c" << endl;
    }
    if (tt == ICHAR)           //���ַ�
    {
        fillname(str, ss);
        cout << str << "    :   " << "z" << endl;
    }
    if (tt == ISTRING)           //���ַ���
    {
        string str1 = str.substr(1, str.length()- 1);
        fillname(str1, ss);
        cout << str1<< "    :   " << "s" << endl;
    }
    */
    if (tt == ERROR)//����
        cout << "ERROR: " << str << endl;
}

void GetToken(ifstream& inf, SYNBL &ss, int& lineno)//ȷ�������Զ�����DFA����ʵ��
{
    TokenType tt = I;
    StateType state = START;
    string str = "";
    bool isdq = true;
    bool isdbline = false;
    while (state != DONE)
    {
        char c = inf.get();
        switch (state)
        {
        case START:
            if (isalpha(c) || c == '_')   //����ĸ�����»��߿�ͷ Ϊ�ؼ��ֻ��ʶ��
            {
                state = ISID;
                str += c;
            }
            else if (isdigit(c))
            {
                state = ISNUM;      //�����ֿ�ͷ ������ ����Ϊ������С��
                str += c;
            }
            else if (c == '\t' || c == ' ' || c == '\n') {}  //�Թ��ո�,�س���Tab
            else if (c == '/' && inf.peek() == '*')  //����ע��
            {//peekֻ�����۲�һ���ַ� ָ�벢������
                str = str + c;
                c = inf.get();
                str = str + c;
                state = ISCOMMENT;   //˵����ע��
            }
            else if (c == '/' && inf.peek() == '/')       //˵����ע��
            {
                str = str + c;
                c = inf.get();
                str = str + c;
                state = ISCOMMENT;
                isdbline = true;
            }
            else if (c == '\'')       //˵�����ַ�
            {
                str = str + c;
                state = ISCHAR;
                tt = ICHAR;
            }
            else if (c == '\"')       //˵�����ַ���
            {
                str = str + c;
                state = ISSTRING;
                tt = ISTRING;
            }
            else
            {
                state = DONE;   //�жϽ��  �Ƚ�״̬����ȷ����
                str += c;
                //���ж��Ƿ��ǽ����������� ��ERROR
                switch (c) {
                case EOF: tt = ENDFILE; break; //�ļ�����
                case '+': tt = PLUS; break;
                case '-': tt = MINUS; break;
                case '*': tt = MTPLUS; break;
                case '/': tt = DEVIDE; break;
                case ';': tt = FH; break;
                case '.': tt = DOT; break;
                case '(': tt = LXKH; break;
                case ')': tt = RXKH; break;
                case '=':
                    if (inf.peek() == '=') //��Ҫ�����ж��Ƿ�Ϊ==
                    {
                        c = inf.get();
                        tt = EQ;
                        str += c;
                    }
                    else
                        tt = ASSIGN;
                    break;
                case '<': //�ж��Ƿ�Ϊ<=
                    if (inf.peek() == '=')
                    {
                        c = inf.get();
                        tt = LE; str += c;
                    }
                    else
                        tt = LT;
                    break;
                case '>':  //�ж��Ƿ�Ϊ>=
                    if (inf.peek() == '=')
                    {
                        c = inf.get();
                        tt = ME;
                        str += c;
                    }
                    else
                        tt = MT;
                    break;
                case ',': tt = SEMI; break;
                case '[': tt = LZKH; break;
                case ']': tt = RXKH; break;
                case '{': tt = LDKH; break;
                case '}': tt = RDKH; break;
                case '!':
                    c = inf.get();
                    if (c == '=')  //�ж�!=
                    {
                        tt = NOTEQ;
                        str += c;
                    }
                    else
                    {              //������ǲ��Ⱥ� ��ָ�����һλ������Ϊerror
                        inf.putback(c);
                        tt = ERROR;
                    }
                    break;
                default: break;
                }
            }
            break;
        case ISCOMMENT:   //�����ע�� ��Ҫ����
            if (c == '\n')   //����л��з� ˵��ע�ͽ�����
            {
                if (isdbline == true)
                {
                    state = DONE;
                    tt = ZS;
                    inf.putback(c);
                }
                else
                    lineno = lineno + 1;
            }
            if (c == '*' && inf.peek() == '/')   //�ж��Ƿ�Ϊ*/
            {
                str = str + c;
                c = inf.get();
                state = DONE;
                tt = ZS;
            }
            else if (inf.eof()) //���ע��û���������ļ�β �����
            {
                tt = ERROR;
                state = DONE;
            }
            str = str + c;   //��������� ��������¶�ע�� ֱ���ܹ��ﵽ�ж�����
            break;
        case ISNUM:
            if (!isdigit(c))  //�������һ����������
            {
                if (c == '.' && isdq == true)  //�ж��Ƿ�Ϊ .  �����ж�����֮ǰ�Ƿ��Ѿ����ֹ� .
                {
                    if (isdigit(inf.peek())) //�����¶� ���������
                    {
                        str += c;
                        isdq = false; //˵�����Ǵ�����  ��. ������־��Ϊfalse ˵���Ѿ�������. �����ظ�����
                        break;
                    }
                    else            //.��������  �������ж�
                    {
                        str = str + c;
                        if (inf.peek() == ' ' || inf.peek() == '\t' || inf.peek() == '\n')  //���Һ�����ڷָ���
                        {
                            tt = ERROR;
                            state = DONE;
                            break;
                        }
                        break;
                    }
                } //�������.
                if (isalpha(c) || c == '.' || c == '_')   //����һ������ĸ����.�����»���
                {
                    str = str + c;
                    if (inf.peek() == ' ' || inf.peek() == '\t' || inf.peek() == '\n')  //�ж����޷ָ���
                    {
                        tt = ERROR;
                        state = DONE;
                        break;
                    }
                    tt = ERROR;
                }
                else   //�н����Ϊ�ָ���
                {
                    if (tt != ERROR)  //˵��������
                        tt = NUM;
                    inf.putback(c); //����
                    state = DONE;
                    vector<string>::iterator result = find(CT.begin(), CT.end(), str);
                    if (result == CT.end())
                        CT.push_back(str);
                }
                break;
            }
            str += c;
            break;
        case ISID:
            if (!isalpha(c))
            {
                if (c == '.')
                {
                    str = str + c;
                    if (inf.peek() == ' ' || inf.peek() == '\t' || inf.peek() == '\n')
                    {
                        tt = ERROR; state = DONE; break;
                    }
                    tt = ERROR; break;
                }
                if (isdigit(c) || c == '_')
                {
                    str = str + c;
                    if (inf.peek() == ' ' || inf.peek() == '\t' || inf.peek() == '\n')
                    {
                        if (tt != ERROR)
                        {
                            tt = ID;
                            state = DONE;
                            break;
                        }
                        state = DONE;
                        break;
                    }
                    break;
                }
                else
                {
                    if (tt != ERROR)
                        tt = ID;
                    inf.putback(c);
                    state = DONE;
                    int flag = 0;
                    for (int i = 0; i < MAXRESERVED; i++)   //�ж�Token�����Ƿ�Ϊ�ؼ���
                    {
                        if (reservedWords[i].str == str)
                        {
                            flag = 1;
                            break;
                        }
                    }
                    if (flag == 0)     //���ǹؼ��� ��Ϊ��ʶ��
                    {
                        vector<string>::iterator result = find(iT.begin(), iT.end(), str);
                        if (result == iT.end())
                            iT.push_back(str);
                    }
                }
                break;
            }
            str += c;
            break;
        case ISCHAR:
            if (c != '\'')
                str = str + c;
            else//˵���ַ������˽�β
            {
                str = str + c;
                state = DONE;
                str = str[1];
                vector<string>::iterator result = find(cT.begin(), cT.end(), str);
                if (result == cT.end())
                    cT.push_back(str);
            }
            tt = ICHAR;
            break;
        case ISSTRING:
            if (c != '\"')
                str = str + c;
            else//˵���ַ������˽�β
            {
                //str = str + c;
                state = DONE;
                vector<string>::iterator result = find(sT.begin(), sT.end(), str);
                if (result == sT.end())
                    sT.push_back(str);
            }
            tt = ISTRING;
            break;
        case DONE:
            break;
        default:
            state = DONE;
            tt = ERROR;
            break;
        }
    }
    if (state == DONE)
    {
        outputtoken(tt, str, ss);
    }
}
#endif // HEADER1_H_INCLUDED