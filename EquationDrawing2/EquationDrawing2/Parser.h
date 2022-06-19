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

#define POINTSNUM 800

#define SIN 1
#define COS 2
#define TAN 3

using namespace std;


typedef struct Point
{
    double x;
    double y;
    bool isInf = 0;
} point;



//2022.04.25 [新增]
//string splitString(string _in);


typedef struct {
    //string dataType; //變數型態
    string name; //變數名稱
    string value; //數值		//2022.4.20 [更改] 型別
}Variable;


class Parser
{
private:
    // ==================變數相關====================
     // 函式儲存
    vector<string> functions;

    // 變數儲存  a , 15  -> a = 15
    vector<Variable> variety;
    // 變數設定
    
    //                          setVariable專用
    //===============================================================

    vector<string> left;
    vector<string> right;
    vector<int> checkCirculate;
    void showVariale(); //列印儲存的變數
    //===============================================================
    // 初始化變數 x 的位置
    // 引數 : iterator 的指標(指向x變數的iterator)
    void xReset(vector<Variable>::iterator* x,string rightSign);
    // 存結果
    vector<Point> points;

    // ===================計算分析相關===================
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
    bool calculate(string function, vector<Point>* points);
    bool setVariable(string _in); // 設定變數
    void clearVariety();

    Parser(/* args */);
    ~Parser();

    // ========check
    void check();

};



