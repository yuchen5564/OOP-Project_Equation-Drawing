#pragma once
#pragma once

#include <iostream>
#include <string>
#include <cmath>
#include <stack>
#include <vector>
#include <sstream>
#include <Windows.h>
#include <map>

#define POINTSNUM 400

using namespace std;


typedef struct Point
{
    double x;
    double y;
    bool isInf = 0;
} point;



//2022.04.25 [�s�W]
//string splitString(string _in);


class Parser
{
private:
    // �禡�x�s
    vector<string> functions;

    // �ܼ��x�s  a , 15  -> a = 15
    map<string, double> variety;

    // �s���G
    vector<Point> points;
    //vector<double> resultY;
    // �p��Ȧs
    string s1, s2;
    // �ϧκ��
    double resolution = 0.05;

    // �B��l�u���v
    // ��J: �ƭȦr��ιB��l�r��
    // ��^: �����u���v�Ʀr�A�Y�D�B��l��^999
    int priority(string _in);

    //2022.04.25 [�s�W]
    int signC(char _in);

    // ���Ǧ����Ǧ�
    // ��J: ���Ǧ��r��A�C�ӼƩM�Ÿ������n���@�ӪŮ�(e.g. 1+2+3: 1 + 2 + 3)
    // ��^: ��Ǧ��r��A�C�ӼƩM�Ÿ������|���@�ӪŮ�(e.g. 12+3+: 1 2 + 3 +)
    string infix2posfix(string _infix, int* f);

    //                    �B��覡
    // ====================================================
    //sud�Badd�����P�ɦs�b(�|���۩I�s)
    string add(string s1, string s2);
    string sub(string s1, string s2);

    string multi(string s1, string s2);
    string divide(string s1, string s2); //By ming. 

    string power(string s1, string s2);

    //                      �禡�����B��
    string functCalculate(string _function);




    //                      �T�����
    string sine(string str);
    string cosine(string str);
    string tange(string str);


    //            string double ����(�L�j�Ƥ覡)
    // ====================================================
    double toDouble(string s);
    string toString(double num);

    // personal
    void checkStack(stack<string>);


public:

    // �p��
    void calculate(string function, vector<Point>* points);

    Parser(/* args */);
    ~Parser();

    // ========check
    void check();

};



