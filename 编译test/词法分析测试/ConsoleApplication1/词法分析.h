#ifndef HEADER1_H_INCLUDED
#define HEADER1_H_INCLUDED
#include <iostream>
#include <fstream>
#include <string>
#include <ctype.h>
#include <vector>
#include <algorithm>
#include <iomanip>
#include "符号表.h"
using namespace std;

#define MAXRESERVED 33   //定义关键字的数目 考虑了33个关键字
#define MAXSPECIALSYMBOLS 22 //定义界符的数目 考虑了22个

vector<string> iT;  //标识符表
vector<string> cT;  //字符表
vector<string> sT;  //字符串表
vector<string> CT;  //常数表

//定义了七种状态 开始 完成 注释 数字 标识符 字符 字符串
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
{")",RXKH},{"[",LZKH},{"]",RZKH},{"{",LDKH},{"}",RDKH},{"/*",LZS},{"*/",RZS} };   //界符表

struct keyWords {
    string str;
    TokenType tt;
}reservedWords[MAXRESERVED] = { {"auto",AUTO},{"short",SHORT},{"int",INT},{"long",LONG},{"float",FLOAT},{"double",DOUBLE},
{"char",CHAR},{"struct",STRUCT},{"string",STRING},{"enum",ENUM},{"typedef",TYPEDEF},{"const",CONST},{"unsigned",UNSIGNED},
{"signed",SIGNED},{"extern",EXTERN},{"register",ELSE},{"static",STATIC},{"volatile",VOLATILE},{"void",VOID},{"if",IF},
{"else",ELSE},{"switch",SWITCH},{"case",CASE},{"for",FOR},{"do",DO},{"while",WHILE},{"goto",GOTO},{"continue",CONTINUE},
{"break",BREAK},{"default",DEFAULT},{"sizeof",SIZEOF},{"return",RETURN},{"main",MAIN} };  //关键字表

SYNBL i("initial");
pointer  init(&i,0);

void fillname(string str, SYNBL &ss);

void outputtoken(TokenType tt, string str, SYNBL &ss);

void GetToken(ifstream& inf, SYNBL &ss, int& lineno);

void showconsttable()
{
    cout << "字符常量如下" << endl;
    for (int i = 0; i < size(cT); i++) 
    {
        cout << i + 1 << " " << cT[i] << endl;
    }
    cout << "字符串常量如下"<<endl;
    for (int i = 0; i < size(sT); i++)
    {
        cout << i + 1 << " " << sT[i] << endl;
    }
    cout << "数字常量如下" << endl;
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

void outputtoken(TokenType tt, string str, SYNBL &ss)  //向符号表中写Token序列
{
    
    /*for (int i = 0; i < MAXRESERVED; i++)   //判断Token类型是否为关键字
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
    for (int j = 0; j < MAXSPECIALSYMBOLS; j++)   //判断Token类型是否为界符
    {
        if (specialSymbols[j].tt == tt)
            cout << str << "  :   " << str << endl;

    }
    if (tt == ID)            //是标识符
    {
        fillname(str, ss);
        cout << str << "  :   " << "i" << endl;
    }
    if (tt == NUM)           //是常数
    {
        fillname(str, ss);
        cout << str << "  :   " << "c" << endl;
    }
    if (tt == ICHAR)           //是字符
    {
        fillname(str, ss);
        cout << str << "    :   " << "z" << endl;
    }
    if (tt == ISTRING)           //是字符串
    {
        string str1 = str.substr(1, str.length()- 1);
        fillname(str1, ss);
        cout << str1<< "    :   " << "s" << endl;
    }
    */
    if (tt == ERROR)//错误
        cout << "ERROR: " << str << endl;
}

void GetToken(ifstream& inf, SYNBL &ss, int& lineno)//确定有穷自动机（DFA）的实现
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
            if (isalpha(c) || c == '_')   //以字母或者下划线开头 为关键字或标识符
            {
                state = ISID;
                str += c;
            }
            else if (isdigit(c))
            {
                state = ISNUM;      //以数字开头 是数字 可以为整数或小数
                str += c;
            }
            else if (c == '\t' || c == ' ' || c == '\n') {}  //略过空格,回车和Tab
            else if (c == '/' && inf.peek() == '*')  //跳过注释
            {//peek只是向后观测一个字符 指针并不后移
                str = str + c;
                c = inf.get();
                str = str + c;
                state = ISCOMMENT;   //说明是注释
            }
            else if (c == '/' && inf.peek() == '/')       //说明是注释
            {
                str = str + c;
                c = inf.get();
                str = str + c;
                state = ISCOMMENT;
                isdbline = true;
            }
            else if (c == '\'')       //说明是字符
            {
                str = str + c;
                state = ISCHAR;
                tt = ICHAR;
            }
            else if (c == '\"')       //说明是字符串
            {
                str = str + c;
                state = ISSTRING;
                tt = ISTRING;
            }
            else
            {
                state = DONE;   //判断界符  先将状态进行确定化
                str += c;
                //在判断是否是界符，如果不是 则ERROR
                switch (c) {
                case EOF: tt = ENDFILE; break; //文件结束
                case '+': tt = PLUS; break;
                case '-': tt = MINUS; break;
                case '*': tt = MTPLUS; break;
                case '/': tt = DEVIDE; break;
                case ';': tt = FH; break;
                case '.': tt = DOT; break;
                case '(': tt = LXKH; break;
                case ')': tt = RXKH; break;
                case '=':
                    if (inf.peek() == '=') //需要向下判断是否为==
                    {
                        c = inf.get();
                        tt = EQ;
                        str += c;
                    }
                    else
                        tt = ASSIGN;
                    break;
                case '<': //判断是否为<=
                    if (inf.peek() == '=')
                    {
                        c = inf.get();
                        tt = LE; str += c;
                    }
                    else
                        tt = LT;
                    break;
                case '>':  //判断是否为>=
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
                    if (c == '=')  //判断!=
                    {
                        tt = NOTEQ;
                        str += c;
                    }
                    else
                    {              //如果不是不等号 则指针回退一位，并置为error
                        inf.putback(c);
                        tt = ERROR;
                    }
                    break;
                default: break;
                }
            }
            break;
        case ISCOMMENT:   //如果是注释 需要分析
            if (c == '\n')   //如果有换行符 说明注释结束了
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
            if (c == '*' && inf.peek() == '/')   //判断是否为*/
            {
                str = str + c;
                c = inf.get();
                state = DONE;
                tt = ZS;
            }
            else if (inf.eof()) //如果注释没结束到了文件尾 则出错
            {
                tt = ERROR;
                state = DONE;
            }
            str = str + c;   //如果都不是 则继续向下读注释 直至能够达到判断条件
            break;
        case ISNUM:
            if (!isdigit(c))  //如果来了一个不是数字
            {
                if (c == '.' && isdq == true)  //判断是否为 .  并且判断在这之前是否已经出现过 .
                {
                    if (isdigit(inf.peek())) //再往下读 如果是数字
                    {
                        str += c;
                        isdq = false; //说明不是纯数字  有. 并将标志置为false 说明已经有两个. 避免重复出错
                        break;
                    }
                    else            //.后不是数字  再往后判断
                    {
                        str = str + c;
                        if (inf.peek() == ' ' || inf.peek() == '\t' || inf.peek() == '\n')  //并且后面存在分隔符
                        {
                            tt = ERROR;
                            state = DONE;
                            break;
                        }
                        break;
                    }
                } //如果不是.
                if (isalpha(c) || c == '.' || c == '_')   //来了一个是字母或者.或者下划线
                {
                    str = str + c;
                    if (inf.peek() == ' ' || inf.peek() == '\t' || inf.peek() == '\n')  //判断有无分隔符
                    {
                        tt = ERROR;
                        state = DONE;
                        break;
                    }
                    tt = ERROR;
                }
                else   //有界符作为分隔符
                {
                    if (tt != ERROR)  //说明是数字
                        tt = NUM;
                    inf.putback(c); //回退
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
                    for (int i = 0; i < MAXRESERVED; i++)   //判断Token类型是否为关键字
                    {
                        if (reservedWords[i].str == str)
                        {
                            flag = 1;
                            break;
                        }
                    }
                    if (flag == 0)     //不是关键字 则为标识符
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
            else//说明字符遇到了结尾
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
            else//说明字符遇到了结尾
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