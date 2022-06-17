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



//2022.04.25 [新增]
//string splitString(string _in);


class Parser
{
private:
    // 函式儲存
    vector<string> functions;

    // 變數儲存  a , 15  -> a = 15
    map<string, double> variety;

    // 存結果
    vector<Point> points;
    //vector<double> resultY;
    // 計算暫存
    string s1, s2;
    // 圖形精度
    double resolution = 0.05;

    // 運算子優先權
    // 輸入: 數值字串或運算子字串
    // 返回: 對應優先權數字，若非運算子返回999
    int priority(string _in);

    //2022.04.25 [新增]
    int signC(char _in);

    // 中序式轉後序式
    // 輸入: 中序式字串，每個數和符號之間要有一個空格(e.g. 1+2+3: 1 + 2 + 3)
    // 返回: 後序式字串，每個數和符號之間會有一個空格(e.g. 12+3+: 1 2 + 3 +)
    string infix2posfix(string _infix, int* f);

    //                    運算方式
    // ====================================================
    //sud、add必須同時存在(會互相呼叫)
    string add(string s1, string s2);
    string sub(string s1, string s2);

    string multi(string s1, string s2);
    string divide(string s1, string s2); //By ming. 

    string power(string s1, string s2);

    //                      函式內部運算
    string functCalculate(string _function);




    //                      三角函數
    string sine(string str);
    string cosine(string str);
    string tange(string str);


    //            string double 互轉(無大數方式)
    // ====================================================
    double toDouble(string s);
    string toString(double num);

    // personal
    void checkStack(stack<string>);


public:

    // 計算
    void calculate(string function, vector<Point>* points);

    Parser(/* args */);
    ~Parser();

    // ========check
    void check();

};



