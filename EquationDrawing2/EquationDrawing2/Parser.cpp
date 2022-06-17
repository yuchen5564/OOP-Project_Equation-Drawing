#include<iostream>
#include "Parser.h"
using namespace std;


Parser::Parser()
{
	//functions.resize(1);
	//this->functions[0] = "y = tan(x) * sin(x)";
	
	//points.assign(200, {0,0,false});
	
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

// 可能有精度問題
string Parser::toString(double num)
{
	
	return to_string(num);
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
	_function.erase(0, 4);
	_function.erase(_function.size()-1, 1);
	
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
				s1 = tmp.top(); //接收第二個數
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
				//如果分母是0，輸出Error
				if (s2 == "0" || s2 == "0.0") { //2022.04.21 [新增] Error Code
					errorCode = 1;
					ERROR("Divisor cannot be zero!");
					//cout << "[Error] Divisor cannot be zero!\n";
				}
				//分母不是0
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

			//判斷是否有非數字字元
			for (int i = 0; i < len; i++) {
				if (s[0] == '-') break;
				if (s.find(".") != string::npos) break;
				if (!isdigit(s[i])) {
					flag = 0;
					break;
				}
			}

			//都是數字，放入堆疊
			if (flag) tmp.push(s);
			//非全數字，檢查變數
			else {

				// 找定義過的變數或 x
				auto mapIt = variety.find(s); // variety 找到變數的跌代器
				if (mapIt != variety.end())
				{
					double mapValue = mapIt->second;
					//	cout << "\nx = " << mapValue << endl;
					s = toString(mapValue);
					/*cout << "check:|" << s << "|\n";*/
					tmp.push(s);

				}
				// 沒找到
				else
				{
					ERROR("\"" + s + "\" variable not found!");
					//cout << "[Error] \"" << s << "\" variable not found!\n";
					errorCode = 1;


				}


			}


			break;
		}


	}
	// 回傳結果
	
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

//						作分割字串(無區隔符號的實踐)
// ==============================================================================
// //分割字元與符號 --2022.04.25
// string splitString(string _in) {
// 	int len = _in.length();
// 	bool f = 0;
// 	for (int i = 0; i < len; i++) {

// 		//2022.04.23 [修正] 遇到小數點不應分隔
// 		if (_in[i] == '.') continue;

// 		//2022.04.23 [修正] 遇到負號不會分隔
// 		//cout << sign(to_string(_in[i])) << endl;
// 		if (signC(_in[i]) != 999) {

// 			//左括號後負號不分隔
// 			if (f && _in[i] == '-') { //2022.04.24 [修正] 判別式
// 				f = 0;
// 				continue;
// 			}
// 			else {
// 				f = 0;
// 			}

// 			//遇到左括號
// 			if (_in[i] == '(') f = 1;
// 			else f = 0;

// 			//遇到未加括號的負數，不分隔 --2022.04.25	
// 			//cout << _in[i] << endl;
// 			if (_in[i] == '-' && !isdigit(_in[i-1]) && !isalpha(_in[i-1])) {
// 				//cout << ".\n";
// 				continue;
// 			}


// 			_in.insert(i, " ");
// 			i++;
// 			len++;
// 			_in.insert(i + 1, " ");
// 			i++;
// 			len++;
// 		}
// 	}
// 	//cout << _in << endl;
// 	return _in;
// }

//							中序轉後序
// ======================================================================

//2022.04.21 [修復] 正負號error問題
string Parser::infix2posfix(string _infix, int* f) {
	//2022.04.22 [新增] 輸入無空格，先分隔運算元與元素

	if (_infix[0] == '-') {
		_infix = "0" + _infix;	//2022.04.24 [修正] 負號在首階乘問題
		*f = 1;
	}
	else {
		*f = 0;
	}
	// NO 分割字串
	//_infix = splitString(_infix);

	istringstream in(_infix);
	stack<string> tmp; //暫存運算元
	stack<string>postfix;
	string s;
	string result = {};
	vector<string>element;
	vector<string>::iterator i;
	//先收入所有元素
	while (in >> s) {
		element.push_back(s);
	}
	int ssign = 0;//0:+ ; 1:- //紀錄正負號標記
	string snext; //儲存下一個元素
	int count = 0; //區分正負號和加減

	for (i = element.begin(); i != element.end(); i++) {
		s = *i;

		if (i != element.end() - 1) {
			snext = *(i + 1);
		}
		bool flag = 0;
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
		case 100:	// 三角函數
			postfix.push(s);
			break;
		case 999: //非運算符號
			//處理正負號
			if (ssign == 1 && s[0] != '-')	s = '-' + s;
			else if (ssign == 1 && s[0] == '-') s.erase(0, 1);
			ssign = 0; //記錄重置
			count = 0; //記錄重置
			postfix.push(s);
			break;
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


void Parser::calculate(string function, vector<Point>* points)
{
	variety.clear();
	variety.insert(pair<string, double>("x", -10));
	// 等於位置
	int findequal = function.find("=");
	// 等於之後的式
	function = function.substr(findequal + 2, function.size() - findequal - 2);
	//暫存stack
	stack<string> tmp;
	//紀錄是否有error出現
	bool errorCode = 0; 

	// for test
	//cout << functions[0] << endl;

	int temp = 0;
	string postfix = infix2posfix(function, &temp);
	//cout << postfix;

	// 目前運算子
	string s;


	// 跑x
	for (int i = 0; i < POINTSNUM; i++)
	{
		istringstream in(postfix);
		// 隱函式運算結果
		string preResult = "";
		while ( !errorCode && in >> s)
		{
			//cout << s << " ";
			

			switch (priority(s))
			{
			case 2:case 3: case 4:

				//算式不合法
				//2022.04.27 [新增] error code: 算式不合法(ex. 123+++)
				if (tmp.empty()) {
					errorCode = 1;
					ERROR("Please confirm the formula!");
					break;
				}
				else {
					s2 = tmp.top(); //接收第一個數
					tmp.pop();
				}

				//算式不合法
				//2022.04.25 [新增] error code: 算式不合法(ex. 1*/3)
				if (tmp.empty()) {
					errorCode = 1;
					ERROR("Please confirm the formula!");
					break;
				}
				else {
					s1 = tmp.top(); //接收第二個數
					tmp.pop();
				}

				// 算式運算 
				if (s == "+")
				{
					
					tmp.push(add(s1, s2));
				}
				else if(s == "-")
				{
					
					tmp.push(sub(s1, s2));
				}
				else if (s == "*")
				{
				
					tmp.push(multi(s1, s2));
				}
				else if (s == "/")
				{
					
					//如果分母是0，輸出Error
					if (s2 == "0" || s2 == "0.0") { //2022.04.21 [新增] Error Code
						errorCode = 1;
						ERROR("Divisor cannot be zero!");
						//cout << "[Error] Divisor cannot be zero!\n";
					}
					//分母不是0
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
				preResult = functCalculate(s);
				
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


				break;
			default: // 輸入不是符號
				
				// 清除開頭+
				if (s[0] == '+') s.erase(0, 1);

				int len = s.length();
				bool flag = 1;

				//判斷是否有非數字字元
				for (int i = 0; i < len; i++) {
					if (s[0] == '-') break;
					if (s.find(".") != string::npos) break;
					if (!isdigit(s[i])) {
						flag = 0;
						break;
					}
				}

				//都是數字，放入堆疊
				if (flag) tmp.push(s);
				//非全數字，檢查變數
				else {
					
					// 找定義過的變數或 x
					auto mapIt = variety.find(s); // variety 找到變數的跌代器
					if (mapIt != variety.end())
					{
						double mapValue = mapIt->second;
					//	cout << "\nx = " << mapValue << endl;
						s = toString(mapValue);
						tmp.push(s);

					}
					// 沒找到
					else
					{
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
			points->push_back({ variety["x"] ,toDouble(tmp.top()) });
			tmp.pop();
			
		}


		variety["x"] = variety["x"] + resolution;
	}



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
