#include "�ʷ�����.h"
#include "���ű�.h"

SYNBL s("SYNBL");
TAPEL t("TAPEL");
AINFL a("AINFL");
LENTH l("LENTH");

int main()
{

    ifstream inf("��������.txt");
    if (!inf)
    {
        cout << "cannot open input.txt file!" << endl;
        return 1;
    }
    ofstream cout("output.txt");
    if (!cout)
    {
        cout << "cannot open output.txt file!" << endl;
        return 1;
    }
    int lineno = 1;
    while (true)
    {
        GetToken(inf, s, lineno);
        if (inf.peek() == EOF)
        {
            cout << "END" << endl;
            break;
        }
    }
    showconsttable();
    //s.print();
    cout << "����ɷ�������" << endl;
    inf.close();
    cout.close();
    return 0;
}