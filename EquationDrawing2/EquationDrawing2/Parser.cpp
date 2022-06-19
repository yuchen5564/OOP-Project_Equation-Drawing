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

// �i��g�צ����D
string Parser::toString(double num)
{

	return to_string(num);
}
// =============================�ܼƬ���====================================
// ��l��x

void Parser::xReset(vector<Variable>::iterator* x, string rightSign)
{
	// ����쪺flag 
	bool findFlag = 0;
	for (vector<Variable>::iterator i = variety.begin(); i != variety.end(); i++)
	{
		// �����e��L��x
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
	//�r�����j
	//_in = splitString(_in);

	//�������
	stringstream in(_in);
	string tmp;
	Variable var;
	string value = {};


	// ��set�����p
	//��J

	//��J: Set A = 10
	if (1) {

		//������L��T
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
				ERROR("�`��");

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
							ERROR("�`��");

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
			var.value = functCalculate(value); //�p��ƭ�

			if (var.value != "Error") {
				//cout << "test3";
				//�M��O�_�w�s�b
				bool find = 0;
				vector<Variable>::iterator i;

				for (i = variety.begin(); i != variety.end(); i++) {

					//�����
					if (var.name == i->name) {
						i->value = var.value;
						find = 1;
						if (errorCode != 1)
							left.push_back(var.name);
					}

				}

				//�����
				if (!find)
				{
					variety.push_back(var);
					if (errorCode != 1)
						left.push_back(var.name);
				}
			}
		}




	}

	//2022.04.27 [�ק�] ��X�C��ɾ�
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
				s1 = tmp.top(); // �����ĤG�Ӽ�
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
				// �p�G�����O0�A��XError
				if (s2 == "0" || s2 == "0.0") { //2022.04.21 [�s�W] Error Code
					errorCode = 1;
					ERROR("Divisor cannot be zero!");
					//cout << "[Error] Divisor cannot be zero!\n";
				}
				// �������O0
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

			// �P�_�O�_���D�Ʀr�r��
			for (int i = 0; i < len; i++) {
				if (s[0] == '-') break;
				if (s.find(".") != string::npos) break;
				if (!isdigit(s[i])) {
					flag = 0;
					break;
				}
			}

			// ���O�Ʀr�A��J���|
			if (flag) tmp.push(s);
			// �D���Ʀr�A�ˬd�ܼ�
			else {

				vector<Variable>::iterator i;
				flag = 0;

				//�M�X�ܼ�
				for (i = variety.begin(); i != variety.end(); i++) {

					//���
					if (s == i->name) {
						s = i->value;
						flag = 1;
						break;
					}

				}

				//�����
				if (flag) tmp.push(s);
				//�S���
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



//							��������
// ======================================================================
//2022.06.19 �p�G�T����Ʈ榡�����h��X"ERROR" 
//2022.04.21 [�״_] ���t��error���D 
string Parser::infix2posfix(string _infix, int* f) {
	//2022.04.22 [�s�W] ��X�L�Ů�A�����j�B�⤸�P����
	//cout << endl << _infix << endl;
	//if (_infix[0] == '-') {
	//	_infix = "0" + _infix;	//2022.04.24 [�ץ�] �}�Y�t���h���D 
	//	*f = 1;
	//}
	//else {
	//	*f = 0;
	//}
	// NO ���Φr��	
	//_infix = splitString(_infix);

	istringstream in(_infix);
	stack<string> tmp; // �Ȧs�B�⤸
	stack<string>postfix;
	string s;
	string result = {};
	vector<string>element;
	vector<string>::iterator i;
	// ====== (�T����ƳB�z) // update: 2022.06.19 by ming
	bool triFunctionEnd = 0;	// �T����ƥk�A���X��
	int triFunctMode = -1;		// �T����ƥh�Y��P�_sin �Bcos �B tan �A-1�N��禡�L�w�q
	bool triFunctionReceive = 0; // �T����ƬA���̪��B�⦡�����X��
	string InFunction = "";		// �T����ƬA���̪��B�⦡
	// ======
	// �����J�Ҧ�����
	while (in >> s) {
		element.push_back(s);
	}
	int ssign = 0;//0:+ ; 1:- // �������t���аO
	string snext; // �x�s�U�@�Ӥ���
	int count = 0; // �Ϥ����t���M�[��

	for (i = element.begin(); i != element.end(); i++) {
		s = *i;

		if (i != element.end() - 1) {
			snext = *(i + 1);
		}
		bool flag = 0;

		// update: 2022.06.19 by ming
		if (triFunctionReceive) // ���禡�����B�z
		{
			if (s.size() > 1 && s.back() == ')') // �J��T����ƥk�A��
			{
				// ������
				InFunction += (" " + s.substr(0, s.size() - 1));   // �h���k�A��
				//cout << "\nfanal Infunction :" << InFunction << endl;
				string value = functCalculate(InFunction);			// ��̭����B�⦡���p��
				//cout << "function value =" << value << endl;
				// �[sin(value)�Bcos(value)�Btan(value) �ipostfix
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
				// inFunction ���� 
				InFunction += (" " + s);
			}
		}
		else // ���`��postfix
		{


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
			case 100:	// �T�����					// update: 2022.06.19 by ming
				// �B�z��L�榡(ex: sin(x + 10))
				// �ন�u��sin(x)�o�Ӯ榡
				if ((s.back() == ')' && s.size() >= 6) && !triFunctionEnd)  // sin(x)���榡
				{
					// �̲׮榡 sin(x)
					postfix.push(s);
					//cout << "test\n";
					break;
				}
				// �̫��z��sin(x) �榡�[�Jpostfix
				if (triFunctionEnd)
				{

				}
				else		// �o�̥N��h�Y�A�B���ܺX�а� �B�⦡������
				{

					if (s[3] != '(')	// �N��T����ƪ��B�⦡����
					{
						// �T����ƿ��~�榡�B�z
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
			case 999: // �D�B��Ÿ�
				// �B�z���t��
				if (ssign == 1 && s[0] != '-')	s = '-' + s;
				else if (ssign == 1 && s[0] == '-') s.erase(0, 1);
				ssign = 0; // �O�����m
				count = 0; // �O�����m
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

//						�p���I����m
// ========================================================
// new update (for project 3)


bool Parser::calculate(string function, vector<Point>* points)
{
	vector<Variable>::iterator x;

	string rightSign = "x";
	if (function[0] == 'x') {
		rightSign = "y";
	}

	// �C���@���B���l��x
	xReset(&x, rightSign);




	// �����m
	int findequal = function.find("=");
	// ���󤧫᪺��
	function = function.substr(findequal + 2, function.size() - findequal - 2);
	// �Ȧsstack
	stack<string> tmp;
	// �����O�_��error�X�{
	bool errorCode = 0;
	string sTemp; // �s���ιL�᪺��
	// for test
	//cout << functions[0] << endl;

	int temp = 0;
	string postfix;


	// �ثe�B��l
	string s;


	// �]x
	for (int i = 0; i < POINTSNUM; i++)
	{
		// �į��ܮt���D�]
		string postfix = infix2posfix(function, &temp);		// update: 2022.06.19  by ming
		//cout << postfix;
		istringstream in(postfix);
		// ���禡�B�⵲�G
		string preResult = "";
		while (!errorCode && in >> s)
		{
			//cout << s << " ";


			switch (priority(s))
			{
			case 2:case 3: case 4:

				// �B�⤣�X�k
				//2022.04.27 [�ץ�] error code: �⦡���X�k(ex. 123+++)
				if (tmp.empty()) {
					errorCode = 1;
					ERROR("Please confirm the formula!");
					return false;
					break;
				}
				else {
					s2 = tmp.top(); // �����Ĥ@�Ӽ�
					tmp.pop();
				}

				// �⦡���X�k
				//2022.04.25 [�s�W] error code: �⦡���X�k(ex. 1*/3)
				if (tmp.empty()) {
					errorCode = 1;
					ERROR("Please confirm the formula!");
					return false;
					break;
				}
				else {
					s1 = tmp.top(); // �����ĤG�Ӽ�
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

					// �p�G�����O0 �A��Xerror
					if (s2 == "0" || s2 == "0.0") { //2022.04.21 [�s�W] Error Code
						errorCode = 1;
						ERROR("Divisor cannot be zero!");
						return false;
						//cout << "[Error] Divisor cannot be zero!\n";
					}
					// �������O0
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
				sTemp = s.substr(4, s.size() - 5);

				preResult = functCalculate(sTemp);

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
				else
				{
					errorCode = 1;
					ERROR("Please confirm the formula!");
					return false;

				}

				break;
			default: // ��J���O�Ÿ�

				// �M���}�Y+
				if (s[0] == '+') s.erase(0, 1);

				int len = s.length();
				bool flag = 1;

				// �P�_�O�_���D�Ʀr�r��
				for (int i = 0; i < len; i++) {
					if (s[0] == '-') break;
					if (s.find(".") != string::npos) break;
					if (!isdigit(s[i])) {
						flag = 0;
						break;
					}
				}

				// ���O�Ʀr�A��J���|
				if (flag) tmp.push(s);
				// �D���Ʀr�A�ˬd�ܼ�
				else {
					vector<Variable>::iterator i;
					flag = 0;

					//�M�X�ܼ�
					for (i = variety.begin(); i != variety.end(); i++) {

						//���
						if (s == i->name) {
							s = i->value;
							flag = 1;
							break;
						}

					}

					//�����
					if (flag) tmp.push(s);
					//�S���
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