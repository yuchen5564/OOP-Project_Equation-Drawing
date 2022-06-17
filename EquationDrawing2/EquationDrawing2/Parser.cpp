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
	// �T�����
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

// �i�঳��װ��D
string Parser::toString(double num)
{
	
	return to_string(num);
}

//										�B��
// ==============================================================================
// ��T���禡�A������
string Parser::functCalculate(string _function)
{

	// �Ȧs �B�⤸��stack
	stack <string> tmp;
	// �����O�_��error�X�{
	bool errorCode = 0;
	// for test
	_function.erase(0, 4);
	_function.erase(_function.size()-1, 1);
	
	int temp = 0;
	string postfix = infix2posfix(_function, &temp);
	//cout << postfix;

	// �ثe�B��l
	string s;

	istringstream in(postfix);

	while (!errorCode && in >> s)
	{
		switch (priority(s))
		{
		case 2: case 3: case 4:

			// �⦡���X�k
			// 2022.04.27 [�s�W] error code: �⦡���X�k(ex. 123+++)
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

			// �⦡���X�k
			// 2022.04.25 [�s�W] error code : �⦡���X�k(1*/3)
			if (tmp.empty())
			{
				errorCode = 1;
				ERROR("Please confirm the formula!");
				break;
			}
			else
			{
				s1 = tmp.top(); //�����ĤG�Ӽ�
				tmp.pop();
			}

			// �⦡�B�� 
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
				//�p�G�����O0�A��XError
				if (s2 == "0" || s2 == "0.0") { //2022.04.21 [�s�W] Error Code
					errorCode = 1;
					ERROR("Divisor cannot be zero!");
					//cout << "[Error] Divisor cannot be zero!\n";
				}
				//�������O0
				else {
					tmp.push(divide(s1, s2)); //2022.04.22 By ming.
				}

			}
			else if (s == "^")
			{

				tmp.push(power(s1, s2));
			}


			break;
		case 100:	// �T�����
			if (s.substr(0, 4) == "sin(")					// sin �B��
			{

			}
			else if (s.substr(0, 4) == "cos(")				// cos �B��
			{

			}
			else											// tan �B��
			{

			}
			break;




		default:// ��J���O�Ÿ�
			/*cout << "\ntest\n";*/
			// �M���}�Y+
			if (s[0] == '+') s.erase(0, 1);

			int len = s.length();
			bool flag = 1;

			//�P�_�O�_���D�Ʀr�r��
			for (int i = 0; i < len; i++) {
				if (s[0] == '-') break;
				if (s.find(".") != string::npos) break;
				if (!isdigit(s[i])) {
					flag = 0;
					break;
				}
			}

			//���O�Ʀr�A��J���|
			if (flag) tmp.push(s);
			//�D���Ʀr�A�ˬd�ܼ�
			else {

				// ��w�q�L���ܼƩ� x
				auto mapIt = variety.find(s); // variety ����ܼƪ��^�N��
				if (mapIt != variety.end())
				{
					double mapValue = mapIt->second;
					//	cout << "\nx = " << mapValue << endl;
					s = toString(mapValue);
					/*cout << "check:|" << s << "|\n";*/
					tmp.push(s);

				}
				// �S���
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
	// �^�ǵ��G
	
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

//						�@���Φr��(�L�Ϲj�Ÿ������)
// ==============================================================================
// //���Φr���P�Ÿ� --2022.04.25
// string splitString(string _in) {
// 	int len = _in.length();
// 	bool f = 0;
// 	for (int i = 0; i < len; i++) {

// 		//2022.04.23 [�ץ�] �J��p���I�������j
// 		if (_in[i] == '.') continue;

// 		//2022.04.23 [�ץ�] �J��t�����|���j
// 		//cout << sign(to_string(_in[i])) << endl;
// 		if (signC(_in[i]) != 999) {

// 			//���A����t�������j
// 			if (f && _in[i] == '-') { //2022.04.24 [�ץ�] �P�O��
// 				f = 0;
// 				continue;
// 			}
// 			else {
// 				f = 0;
// 			}

// 			//�J�쥪�A��
// 			if (_in[i] == '(') f = 1;
// 			else f = 0;

// 			//�J�쥼�[�A�����t�ơA�����j --2022.04.25	
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

//							��������
// ======================================================================

//2022.04.21 [�״_] ���t��error���D
string Parser::infix2posfix(string _infix, int* f) {
	//2022.04.22 [�s�W] ��J�L�Ů�A�����j�B�⤸�P����

	if (_infix[0] == '-') {
		_infix = "0" + _infix;	//2022.04.24 [�ץ�] �t���b���������D
		*f = 1;
	}
	else {
		*f = 0;
	}
	// NO ���Φr��
	//_infix = splitString(_infix);

	istringstream in(_infix);
	stack<string> tmp; //�Ȧs�B�⤸
	stack<string>postfix;
	string s;
	string result = {};
	vector<string>element;
	vector<string>::iterator i;
	//�����J�Ҧ�����
	while (in >> s) {
		element.push_back(s);
	}
	int ssign = 0;//0:+ ; 1:- //�������t���аO
	string snext; //�x�s�U�@�Ӥ���
	int count = 0; //�Ϥ����t���M�[��

	for (i = element.begin(); i != element.end(); i++) {
		s = *i;

		if (i != element.end() - 1) {
			snext = *(i + 1);
		}
		bool flag = 0;
		switch (priority(s)) {

		case 2:
			//���t���B�z
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
			//2022.04.23 [�ץ�] 1/3*3 = 0.99999...  //TO-DO: 1/3+2/3
			//2022.04.25 [�ץ�] stack empty error
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
		case 100:	// �T�����
			postfix.push(s);
			break;
		case 999: //�D�B��Ÿ�
			//�B�z���t��
			if (ssign == 1 && s[0] != '-')	s = '-' + s;
			else if (ssign == 1 && s[0] == '-') s.erase(0, 1);
			ssign = 0; //�O�����m
			count = 0; //�O�����m
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

//						�p���I����m
// ========================================================
// new update (for project 3)


void Parser::calculate(string function, vector<Point>* points)
{
	variety.clear();
	variety.insert(pair<string, double>("x", -10));
	// �����m
	int findequal = function.find("=");
	// ���󤧫᪺��
	function = function.substr(findequal + 2, function.size() - findequal - 2);
	//�Ȧsstack
	stack<string> tmp;
	//�����O�_��error�X�{
	bool errorCode = 0; 

	// for test
	//cout << functions[0] << endl;

	int temp = 0;
	string postfix = infix2posfix(function, &temp);
	//cout << postfix;

	// �ثe�B��l
	string s;


	// �]x
	for (int i = 0; i < POINTSNUM; i++)
	{
		istringstream in(postfix);
		// ���禡�B�⵲�G
		string preResult = "";
		while ( !errorCode && in >> s)
		{
			//cout << s << " ";
			

			switch (priority(s))
			{
			case 2:case 3: case 4:

				//�⦡���X�k
				//2022.04.27 [�s�W] error code: �⦡���X�k(ex. 123+++)
				if (tmp.empty()) {
					errorCode = 1;
					ERROR("Please confirm the formula!");
					break;
				}
				else {
					s2 = tmp.top(); //�����Ĥ@�Ӽ�
					tmp.pop();
				}

				//�⦡���X�k
				//2022.04.25 [�s�W] error code: �⦡���X�k(ex. 1*/3)
				if (tmp.empty()) {
					errorCode = 1;
					ERROR("Please confirm the formula!");
					break;
				}
				else {
					s1 = tmp.top(); //�����ĤG�Ӽ�
					tmp.pop();
				}

				// �⦡�B�� 
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
					
					//�p�G�����O0�A��XError
					if (s2 == "0" || s2 == "0.0") { //2022.04.21 [�s�W] Error Code
						errorCode = 1;
						ERROR("Divisor cannot be zero!");
						//cout << "[Error] Divisor cannot be zero!\n";
					}
					//�������O0
					else {
							tmp.push(divide(s1, s2)); //2022.04.22 By ming.
					}

				}
				else if (s == "^")
				{
					
					tmp.push(power(s1, s2));
				}


				break;
			case 100:	// �T�����
				// ���禡�B�⵲�G
				preResult = functCalculate(s);
				
				if (preResult != "error")		// ���e�L���~�ƾǦ�
				{
					if (s.substr(0, 4) == "sin(")					// sin �B��
					{
						tmp.push(sine(preResult));
					}
					else if (s.substr(0, 4) == "cos(")				// cos �B��
					{
						tmp.push(cosine(preResult));
					}
					else											// tan �B��
					{
						tmp.push(tange(preResult));
					}


				}


				break;
			default: // ��J���O�Ÿ�
				
				// �M���}�Y+
				if (s[0] == '+') s.erase(0, 1);

				int len = s.length();
				bool flag = 1;

				//�P�_�O�_���D�Ʀr�r��
				for (int i = 0; i < len; i++) {
					if (s[0] == '-') break;
					if (s.find(".") != string::npos) break;
					if (!isdigit(s[i])) {
						flag = 0;
						break;
					}
				}

				//���O�Ʀr�A��J���|
				if (flag) tmp.push(s);
				//�D���Ʀr�A�ˬd�ܼ�
				else {
					
					// ��w�q�L���ܼƩ� x
					auto mapIt = variety.find(s); // variety ����ܼƪ��^�N��
					if (mapIt != variety.end())
					{
						double mapValue = mapIt->second;
					//	cout << "\nx = " << mapValue << endl;
						s = toString(mapValue);
						tmp.push(s);

					}
					// �S���
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
