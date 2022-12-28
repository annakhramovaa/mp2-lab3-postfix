// ���������� ������� � ������� ��� ���������� �������������� ���������
#include "arithmetic.h"
#include "stack.h"
using namespace std;

Lexema::Lexema(string _str) {
	lexema = _str;
}

Lexema::~Lexema() {}

Operand::Operand(string _str) :Lexema(_str) {}
Operand::~Operand() {}

void Operand::show(){
	cout << lexema;
 }

int Operand::Priority() {
	throw exception("operand has not priority");
	return 0;
}

string Operand::NameOfClass() {
	return "Operand";
}

Operation::Operation(char _str) : Lexema(&_str) {
	if (_str == '~')
		priority = 1;
	if (_str == '*' || _str == '/')
		priority = 2;
	if (_str == '+' || _str == '-')
		priority = 3;
	if ((_str == '(') || (_str == ')'))
		priority = 0;
}
int Operation::Priority() {
	return priority;
}

string Operation::NameOfClass() {
	return "Operation";
}

void Operation::show() {
	cout << lexema;
}

Operation::~Operation() {}

Const::Const(string _str) : Operand(_str) {
	int pow = 10;
	value = int(_str[0]) - 48;
	int i = 1;
	for (; i < _str.size() && _str[i] != ','; i++) {
		if (int(_str[i]) >= 48 && int(_str[i]) <= 57)
		{
			value = value * pow + (int(_str[i])) - 48;
		}
	}
	i++;
	for (; i < _str.size(); i++) {
		if (int(_str[i]) >= 48 && int(_str[i]) <= 57)
		{
			value = value + (int(_str[i]) - 48) / pow;
			pow = pow * 10;
		}
	}

}

Const::~Const() {}

Variable::Variable(char _str) : Operand(&_str) {}

Variable::~Variable() {}

Arithmetic_expression::Arithmetic_expression(string expr) : infix(expr) {
	size = expr.size();
	lexems = new Lexema * [expr.size()];
	postfix = new Lexema * [expr.size()];
	Parse();
	ToPostfix();
};


bool Arithmetic_expression::IsOperation(char symb) {
	if ((symb == '+') || (symb == '-') || (symb == '*') || (symb == '/')|| (symb == ')') || (symb == '('))
		return true;
	else return false;
}

Arithmetic_expression::~Arithmetic_expression() {}

void Arithmetic_expression::Parse() {

	int j = 0;

	for (int i = 0; i < infix.size(); i++) {

		if (int(infix[i]) >= 48 && int(infix[i]) <= 57) {
			string operand;
			int points = 0;
			while ((int(infix[i]) >= 48 && int(infix[i]) <= 57) || (infix[i] == ',')) {
				operand += infix[i];
				i++;
			}
			lexems[j] = new Const(operand);
			j++;
		}

		if (infix[i] >= 97 && infix[i] <= 122) {
			lexems[j] = new Variable(infix[i]);
			j++; 
		}

		if (IsOperation(infix[i]))  {
			if ((infix[i] == '-' ) && ((i == 0) || (IsOperation(infix[i-1])&&(infix[i - 1]!=')')))) {
				lexems[j] = new Operation('~');
				j++;
			}
			else {
				lexems[j] = new Operation(infix[i]);
				j++;
			}				
		}
	}
}

void Arithmetic_expression::show() {
	for (int i = 0; i < size; i++) {
		lexems[i]->show();
		cout << " ";
	}
}

void Arithmetic_expression::ToPostfix() {
	TStack <Lexema*> Stk(size);
	int j = 0;

	for (int i = 0; i < infix.size(); i++) {
		if (lexems[i]->NameOfClass() == "Operand") {
			postfix[j] = lexems[i];
			j++;
		}

		if (lexems[i]->lexema == "(") {
			while (Stk.TopElem() != '(') {
				postfix[j] = Stk.Pop();
				j++;
			}
		}

		if (lexems[i]->lexema == ")") {
			Stk.Push(lexems[i]);
		}

		if (lexems[i]->NameOfClass() == "Operation" && lexems[i]->lexema != ")" && lexems[i]->lexema != "(" && lexems[i]->Priority()!=0) {
			while ((Stk.IsEmpty()) || (Stk.TopElem()->Priority() < lexems[i]->Priority())) {
				Stk.Push(lexems[i]);
			}
			postfix[j] = Stk.Pop();
			j++;
		}

		while (!Stk.IsEmpty()) {
			postfix[j] = Stk.Pop();
			j++;
		}
	}

}