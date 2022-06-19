#include<iostream>
#include "Parser.h"
using namespace std;


Parser::Parser()
{
	//functions.resize(1);
	//this->functions[0] = "y = tan(x) * sin(x)";

	//points.assign(200, {0,0,false});
	/*setVariable("a = 15");
	setVariable("b = 12");
	setVariable("c = 12");
	setVariable("c = 11 + i");*/
	//setVariable("a = b");
}

Parser::~Parser()
{}

int Parser::priority(string _in) {
	if (_in == "(") return 0;
	if (_in == ")") return 1;

	if (_in == "+") return 2;
	if (_in == "-") return 2;
	if (_in == "*") return 3;
	if (_in == "/") return 3;
	if (_in == "^") return 4;
	if (_in == "!") return 5;
	// 三角函數
	if (_in.substr(0, 4) == "sin(") return 100;
	if (_in.substr(0, 4) == "cos(") return 100;
	if (_in.substr(0, 4) == "tan(") return 100;



	return 999;
}

int Parser::signC(char _in) {
	if (_in == '(') return 0;
	if (_in == ')') return 1;

	if (_in == '+') return 2;
	if (_in == '-') return 2;
	if (_in == '*') return 3;
	if (_in == '/') return 3;
	if (_in == '^') return 4;
	if (_in == '!') return 5;
	if (_in == '=') return 10;

	return 999;
}


double Parser::toDouble(string s)
{
	double result = 0;
	sscanf_s(s.c_str(), "%lf", &result);
	return result;
}

// 可能經度有問題
string Parser::toString(double num)
{

	return to_string(num);
}
// =============================變數相關====================================
// 初始化x

void Parser::xReset(vector<Variable>::iterator* x, string rightSign)
{
	// 有找到的flag 
	bool findFlag = 0;
	for (vector<Variable>::iterator i = variety.begin(); i != variety.end(); i++)
	{
		// 有之前算過的x
		if (i->name == rightSign)
		{
			findFlag = 1;
			variety.erase(i);
			break;
		}

	}
	variety.push_back({ rightSign, "-10" });
	*x = variety.end() - 1;
}

void Parser::showVariale() {
	vector<Variable>::iterator i;
	//SET_COLOR(14);
	cout << "=====Now in memory======\n";
	cout << "Type\tName\tValue\n";
	cout << "------------------------\n";
	for (i = variety.begin(); i != variety.end(); i++) {
		cout << "\t" << i->name << "\t" << i->value << "\n";
	}
	cout << "========================\n";
	//SET_COLOR(15);

}


bool Parser::setVariable(string _in)
{
	bool errorCode = 0;
	static int num = 0;
	//字元分隔
	//_in = splitString(_in);

	//接收資料
	stringstream in(_in);
	string tmp;
	Variable var;
	string value = {};


	// 有set的情況
	//輸入

	//輸入: Set A = 10
	if (1) {

		//接收其他資訊
		in >> tmp;
		var.name = tmp;
		in >> tmp;

		while (in >> tmp) {
			//cout << "_____________________" << tmp << endl;
			value += (' ' + tmp);
			//cout << value;

			if (var.name == tmp)
			{
				errorCode = 1;
				ERROR("循環");

			}
			for (size_t i = 0; i < left.size(); ++i) {
				//cout << tmp;
				//cout << "_____________________" << tmp << endl;
				if (checkCirculate[i] != 0)
				{
					//cout << "_____________________" << tmp << endl;
					//cout << value;
					//cout << i << "\n" ;
					for (int j = checkCirculate[i - 1] + 1; j <= checkCirculate[i]; ++j)
					{

						if (var.name == right[j - 1] && tmp == left[i])
						{
							errorCode = 1;
							ERROR("循環");

						}

					}
				}
				//cout << value;

			}
			for (size_t i = 0; i < left.size(); ++i) {
				if (tmp == left[i])
				{
					if (errorCode != 1)
					{
						right.push_back(tmp);
						num++;
						break;
					}

				}
			}

		}
		if (errorCode != 1)
			checkCirculate.push_back(num);
		if (errorCode != 1)
		{
			value.erase(value.begin());
			cout << "now value:" << value << endl;
			var.value = functCalculate(value); //計算數值

			if (var.value != "Error") {
				//cout << "test3";
				//尋找是否已存在
				bool find = 0;
				vector<Variable>::iterator i;

				for (i = variety.begin(); i != variety.end(); i++) {

					//有找到
					if (var.name == i->name) {
						i->value = var.value;
						find = 1;
						if (errorCode != 1)
							left.push_back(var.name);
					}

				}

				//未找到
				if (!find)
				{
					variety.push_back(var);
					if (errorCode != 1)
						left.push_back(var.name);
				}
			}
		}




	}

	//2022.04.27 [修改] 輸出列表時機
	if (!errorCode) showVariale();
	if (!errorCode) {
		return true;
	}
	else {
		return false;
	}
	

}

void Parser::clearVariety()
{
	variety.clear();
}



//										運算
// ==============================================================================
// 算三角函式括號內部
string Parser::functCalculate(string _function)
{

	// 暫存 運算元的stack
	stack <string> tmp;
	// 紀錄是否有error出現
	bool errorCode = 0;
	// for test

	int temp = 0;
	string postfix = infix2posfix(_function, &temp);
	//cout << postfix;

	// 目前運算子
	string s;

	istringstream in(postfix);

	while (!errorCode && in >> s)
	{
		switch (priority(s))
		{
		case 2: case 3: case 4:

			// 算式不合法
			// 2022.04.27 [新增] error code: 算式不合法(ex. 123+++)
			if (tmp.empty())
			{
				errorCode = 1;
				ERROR("Please confirm the formula!");
				break;
			}
			else
			{
				s2 = tmp.top();
				tmp.pop();
			}

			// 算式不合法
			// 2022.04.25 [新增] error code : 算式不合法(1*/3)
			if (tmp.empty())
			{
				errorCode = 1;
				ERROR("Please confirm the formula!");
				break;
			}
			else
			{
				s1 = tmp.top(); // 接收第二個數
				tmp.pop();
			}

			// 算式運算
			if (s == "+")
			{
				tmp.push(add(s1, s2));
			}
			else if (s == "-")
			{
				tmp.push(sub(s1, s2));
			}
			else if (s == "*")
			{
				tmp.push(multi(s1, s2));
			}
			else if (s == "/")
			{
				// 如果分母是0，輸出Error
				if (s2 == "0" || s2 == "0.0") { //2022.04.21 [新增] Error Code
					errorCode = 1;
					ERROR("Divisor cannot be zero!");
					//cout << "[Error] Divisor cannot be zero!\n";
				}
				// 分母不是0
				else {
					tmp.push(divide(s1, s2)); //2022.04.22 By ming.
				}

			}
			else if (s == "^")
			{

				tmp.push(power(s1, s2));
			}


			break;
		case 100:	// 三角函數
			if (s.substr(0, 4) == "sin(")					// sin 運算
			{

			}
			else if (s.substr(0, 4) == "cos(")				// cos 運算
			{

			}
			else											// tan 運算
			{

			}
			break;




		default:// 輸入不是符號
			/*cout << "\ntest\n";*/
			// 清除開頭+
			if (s[0] == '+') s.erase(0, 1);

			int len = s.length();
			bool flag = 1;

			// 判斷是否有非數字字元
			for (int i = 0; i < len; i++) {
				if (s[0] == '-') break;
				if (s.find(".") != string::npos) break;
				if (!isdigit(s[i])) {
					flag = 0;
					break;
				}
			}

			// 都是數字，放入堆疊
			if (flag) tmp.push(s);
			// 非全數字，檢查變數
			else {

				vector<Variable>::iterator i;
				flag = 0;

				//尋訪變數
				for (i = variety.begin(); i != variety.end(); i++) {

					//找到
					if (s == i->name) {
						s = i->value;
						flag = 1;
						break;
					}

				}

				//有找到
				if (flag) tmp.push(s);
				//沒找到
				else {
					ERROR("\"" + s + "\" variable not found!");
					//cout << "[Error] \"" << s << "\" variable not found!\n";
					errorCode = 1;

				}


			}


			break;
		}


	}


	if (!errorCode)
	{
		return tmp.top();
	}
	return "error";

}

string Parser::add(string s1, string s2)
{
	double result = toDouble(s1) + toDouble(s2);
	return toString(result);
}
string Parser::sub(string s1, string s2)
{
	double result = toDouble(s1) - toDouble(s2);
	return toString(result);
}
string Parser::multi(string s1, string s2)
{
	double result = toDouble(s1) * toDouble(s2);
	return toString(result);
}
string Parser::divide(string s1, string s2)
{
	double result = toDouble(s1) / toDouble(s2);
	return toString(result);
}
string Parser::power(string s1, string s2)
{
	double result = pow(toDouble(s1), toDouble(s2));
	return toString(result);
}


string Parser::sine(string str)
{
	double num = toDouble(str);

	return toString(sin(num));

}

string Parser::cosine(string str)
{
	double num = toDouble(str);
	return toString(cos(num));
}
string Parser::tange(string str)
{
	//cout << "check:|" << str << "|\n";
	double num = toDouble(str);
	return toString(tan(num));
}



//							中序轉後序
// ======================================================================
//2022.06.19 如果三角函數格式有錯則輸出"ERROR" 
//2022.04.21 [修復] 正負號error問題 
string Parser::infix2posfix(string _infix, int* f) {
	//2022.04.22 [新增] 輸出無空格，先分隔運算元與元素
	//cout << endl << _infix << endl;
	//if (_infix[0] == '-') {
	//	_infix = "0" + _infix;	//2022.04.24 [修正] 開頭負階層問題 
	//	*f = 1;
	//}
	//else {
	//	*f = 0;
	//}
	// NO 分割字串	
	//_infix = splitString(_infix);

	istringstream in(_infix);
	stack<string> tmp; // 暫存運算元
	stack<string>postfix;
	string s;
	string result = {};
	vector<string>element;
	vector<string>::iterator i;
	// ====== (三角函數處理) // update: 2022.06.19 by ming
	bool triFunctionEnd = 0;	// 三角函數右括號旗標
	int triFunctMode = -1;		// 三角函數去頭後判斷sin 、cos 、 tan ，-1代表函式無定義
	bool triFunctionReceive = 0; // 三角函數括號裡的運算式接收旗標
	string InFunction = "";		// 三角函數括號裡的運算式
	// ======
	// 先收入所有元素
	while (in >> s) {
		element.push_back(s);
	}
	int ssign = 0;//0:+ ; 1:- // 紀錄正負號標記
	string snext; // 儲存下一個元素
	int count = 0; // 區分正負號和加減

	for (i = element.begin(); i != element.end(); i++) {
		s = *i;

		if (i != element.end() - 1) {
			snext = *(i + 1);
		}
		bool flag = 0;

		// update: 2022.06.19 by ming
		if (triFunctionReceive) // 做函式接受處理
		{
			if (s.size() > 1 && s.back() == ')') // 遇到三角函數右括號
			{
				// 做收尾
				InFunction += (" " + s.substr(0, s.size() - 1));   // 去掉右括號
				//cout << "\nfanal Infunction :" << InFunction << endl;
				string value = functCalculate(InFunction);			// 對裡面的運算式做計算
				//cout << "function value =" << value << endl;
				// 加sin(value)、cos(value)、tan(value) 進postfix
				if (triFunctMode == SIN)
				{
					postfix.push("sin(" + value + ")");
				}
				else if (triFunctMode == COS)
				{
					postfix.push("cos(" + value + ")");
				}
				else
				{
					postfix.push("tan(" + value + ")");
				}
				triFunctionReceive = false;
			}
			else
			{
				// inFunction 接收 
				InFunction += (" " + s);
			}
		}
		else // 正常做postfix
		{


			switch (priority(s)) {

			case 2:
				//正負號處理
				if (i != element.begin() && count == 0) {
					count++;
				}
				else {
					flag = 1;
					if (!(isdigit(snext[0]) || isdigit(snext[1]))) {
						if (s == "-" && ssign == 0) ssign = 1;
						else if (s == "-" && ssign == 1) ssign = 0;
						continue;
					}
				}
			case 3: case 4: case 5:
				//2022.04.23 [修正] 1/3*3 = 0.99999...  //TO-DO: 1/3+2/3
				//2022.04.25 [修正] stack empty error
				/*if (!tmp.empty() && s == "*" && tmp.top() == "/") {
					string v = postfix.top();
					postfix.pop();
					postfix.push(snext);
					postfix.push(s);
					postfix.push(v);
					i++;
					break;
				}*/

				while (!tmp.empty() && priority(tmp.top()) >= priority(s)) {
					postfix.push(tmp.top());
					tmp.pop();
				}

				if (!flag) tmp.push(s);

				break;
			case 0:	// "("
				tmp.push(s);
				break;
			case 1:	// ")"
				while (tmp.top() != "(") {
					postfix.push(tmp.top());
					tmp.pop();
				}
				tmp.pop();
				break;
			case 100:	// 三角函數					// update: 2022.06.19 by ming
				// 處理其他格式(ex: sin(x + 10))
				// 轉成只有sin(x)這個格式
				if ((s.back() == ')' && s.size() >= 6) && !triFunctionEnd)  // sin(x)的格式
				{
					// 最終格式 sin(x)
					postfix.push(s);
					//cout << "test\n";
					break;
				}
				// 最後整理成sin(x) 格式加入postfix
				if (triFunctionEnd)
				{

				}
				else		// 這裡代表去頭，且改變旗標做 運算式的接收
				{

					if (s[3] != '(')	// 代表三角函數的運算式有錯
					{
						// 三角函數錯誤格式處理
						return "ERROR";
					}
					else
					{
						triFunctionReceive = true;
						InFunction = s.substr(4, s.size() - 4);
						//cout << "Infunction test:" << InFunction << endl;
						if (s.substr(0, 4) == "sin(")
						{
							triFunctMode = SIN;
						}
						else if (s.substr(0, 4) == "cos(")
						{
							triFunctMode = COS;
						}
						else if (s.substr(0, 4) == "tan(")
						{
							triFunctMode = TAN;
						}
						else
						{
							triFunctMode = -1;
						}
					}

				}


				break;
			case 999: // 非運算符號
				// 處理正負號
				if (ssign == 1 && s[0] != '-')	s = '-' + s;
				else if (ssign == 1 && s[0] == '-') s.erase(0, 1);
				ssign = 0; // 記錄重置
				count = 0; // 記錄重置
				postfix.push(s);
				break;
			}
		}
	}
	while (!tmp.empty()) {
		postfix.push(tmp.top());
		tmp.pop();
	}
	while (!postfix.empty()) {
		result = postfix.top() + ' ' + result;
		postfix.pop();
	}
	//cout << result << endl;
	return result;
}

//						計算點的位置
// ========================================================
// new update (for project 3)


bool Parser::calculate(string function, vector<Point>* points)
{
	vector<Variable>::iterator x;

	string rightSign = "x";
	if (function[0] == 'x') {
		rightSign = "y";
	}

	// 每做一次運算初始化x
	xReset(&x, rightSign);




	// 等於位置
	int findequal = function.find("=");
	// 等於之後的式
	function = function.substr(findequal + 2, function.size() - findequal - 2);
	// 暫存stack
	stack<string> tmp;
	// 紀錄是否有error出現
	bool errorCode = 0;
	string sTemp; // 存分割過後的值
	// for test
	//cout << functions[0] << endl;

	int temp = 0;
	string postfix;


	// 目前運算子
	string s;


	// 跑x
	for (int i = 0; i < POINTSNUM; i++)
	{
		// 效能變差的主因
		string postfix = infix2posfix(function, &temp);		// update: 2022.06.19  by ming
		//cout << postfix;
		istringstream in(postfix);
		// 隱函式運算結果
		string preResult = "";
		while (!errorCode && in >> s)
		{
			//cout << s << " ";


			switch (priority(s))
			{
			case 2:case 3: case 4:

				// 運算不合法
				//2022.04.27 [修正] error code: 算式不合法(ex. 123+++)
				if (tmp.empty()) {
					errorCode = 1;
					ERROR("Please confirm the formula!");
					return false;
					break;
				}
				else {
					s2 = tmp.top(); // 接收第一個數
					tmp.pop();
				}

				// 算式不合法
				//2022.04.25 [新增] error code: 算式不合法(ex. 1*/3)
				if (tmp.empty()) {
					errorCode = 1;
					ERROR("Please confirm the formula!");
					return false;
					break;
				}
				else {
					s1 = tmp.top(); // 接收第二個數
					tmp.pop();
				}

				// 算式運算
				if (s == "+")
				{

					tmp.push(add(s1, s2));
				}
				else if (s == "-")
				{

					tmp.push(sub(s1, s2));
				}
				else if (s == "*")
				{

					tmp.push(multi(s1, s2));
				}
				else if (s == "/")
				{

					// 如果分母是0 ，輸出error
					if (s2 == "0" || s2 == "0.0") { //2022.04.21 [新增] Error Code
						errorCode = 1;
						ERROR("Divisor cannot be zero!");
						return false;
						//cout << "[Error] Divisor cannot be zero!\n";
					}
					// 分母不是0
					else {
						tmp.push(divide(s1, s2)); //2022.04.22 By ming.
					}

				}
				else if (s == "^")
				{

					tmp.push(power(s1, s2));
				}


				break;
			case 100:	// 三角函數
				// 隱函式運算結果
				sTemp = s.substr(4, s.size() - 5);

				preResult = functCalculate(sTemp);

				if (preResult != "error")		// 內容無錯誤數學式
				{
					if (s.substr(0, 4) == "sin(")					// sin 運算
					{
						tmp.push(sine(preResult));
					}
					else if (s.substr(0, 4) == "cos(")				// cos 運算
					{
						tmp.push(cosine(preResult));
					}
					else											// tan 運算
					{
						tmp.push(tange(preResult));
					}


				}
				else
				{
					errorCode = 1;
					ERROR("Please confirm the formula!");
					return false;

				}

				break;
			default: // 輸入不是符號

				// 清除開頭+
				if (s[0] == '+') s.erase(0, 1);

				int len = s.length();
				bool flag = 1;

				// 判斷是否有非數字字元
				for (int i = 0; i < len; i++) {
					if (s[0] == '-') break;
					if (s.find(".") != string::npos) break;
					if (!isdigit(s[i])) {
						flag = 0;
						break;
					}
				}

				// 都是數字，放入堆疊
				if (flag) tmp.push(s);
				// 非全數字，檢查變數
				else {
					vector<Variable>::iterator i;
					flag = 0;

					//尋訪變數
					for (i = variety.begin(); i != variety.end(); i++) {

						//找到
						if (s == i->name) {
							s = i->value;
							flag = 1;
							break;
						}

					}

					//有找到
					if (flag) tmp.push(s);
					//沒找到
					else {
						ERROR("\"" + s + "\" variable not found!");
						//cout << "[Error] \"" << s << "\" variable not found!\n";
						errorCode = 1;
					}
				}

				break;
			}



		}
		if (!errorCode)
		{
			if (rightSign == "x") {
				points->push_back({ toDouble(x->value) ,toDouble(tmp.top()) });
			}
			else {
				points->push_back({ toDouble(tmp.top()),toDouble(x->value) });
			}
			
			tmp.pop();

		}


		x->value = toString(toDouble(x->value) + resolution);
	}


	return true;
}

void Parser::check()
{
	for (auto It = points.begin(); It != points.end(); It++)
	{
		cout << "{" << It->x << " , " << It->y << "}\n";
	}


}

void Parser::checkStack(stack <string> t)
{
	cout << "now stack have " << t.size() << endl;
}