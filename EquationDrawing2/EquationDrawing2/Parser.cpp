#include<iostream>
#include "Parser.h"
using namespace std;


parser::parser()
{
	functions.resize(1);
	this->functions[0] = "y = 1 /** x";
	variety.insert(pair<string, double>("x", -10));
	resultY.assign(200, 0);
	
}

parser::~parser()
{}

int parser::priority(string _in) {
	if (_in == "(") return 0;
	if (_in == ")") return 1;

	if (_in == "+") return 2;
	if (_in == "-") return 2;
	if (_in == "*") return 3;
	if (_in == "/") return 3;
	if (_in == "^") return 4;
	if (_in == "!") return 5;

	return 999;
}

int parser::signC(char _in) {
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


double parser::toDouble(string s)
{
	double result = 0;
	sscanf_s(s.c_str(), "%lf", &result);
	return result;
}

// �i�঳��װ��D
string parser::toString(double num)
{
	
	return to_string(num);
}

//										�B��
// ==============================================================================
string parser::add(string s1, string s2)
{
	double result = toDouble(s1) + toDouble(s2);
	return toString(result);
}
string parser::sub(string s1, string s2)
{
	double result = toDouble(s1) - toDouble(s2);
	return toString(result);
}
string parser::multi(string s1, string s2)
{
	double result = toDouble(s1) * toDouble(s2);
	return toString(result);
}
string parser::divide(string s1, string s2)
{
	double result = toDouble(s1) / toDouble(s2);
	return toString(result);
}
string parser::power(string s1, string s2)
{
	double result = pow(toDouble(s1), toDouble(s2));
	return toString(result);
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
string parser::infix2posfix(string _infix, int* f) {
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
			if (!tmp.empty() && s == "*" && tmp.top() == "/") {
				string v = postfix.top();
				postfix.pop();
				postfix.push(snext);
				postfix.push(s);
				postfix.push(v);
				i++;
				break;
			}

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


void parser::calculate()
{
	
	// �����m
	int findequal = functions[0].find("=");
	// ���󤧫᪺��
	functions[0] = functions[0].substr(findequal + 2, functions.size() - findequal - 2);
	//�Ȧsstack
	stack<string> tmp;
	//�����O�_��error�X�{
	bool errorCode = 0; 

	// for test
	cout << functions[0] << endl;

	int temp = 0;
	string postfix = infix2posfix(functions[0], &temp);
	cout << postfix;

	// �ثe�B��l
	string s;


	// �]x
	for (int i = 0; i < 200; i++)
	{
		istringstream in(postfix);

		while ( !errorCode && in >> s)
		{
			//cout << s << " ";
			//// �ѨMS �ŭȰ��D
			//if (s.empty())
			//{
			//	break;
			//}

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


			default: // ��J���O�Ÿ�
				
				//�M���}�Y+
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
			resultY[i] = toDouble(tmp.top());
			tmp.pop();
			
		}


		variety["x"] = variety["x"] + resolution;
	}



}

void parser::check()
{
	for (auto It = resultY.begin(); It != resultY.end(); It++)
	{
		cout << *It << " ";
	}


}

void parser::checkStack(stack <string> t) 
{
	cout << "now stack have " << t.size() << endl;
}
