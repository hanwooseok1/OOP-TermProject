#include <iostream>
#include <string>
#include <stack>

using namespace std;

class OperationBase {
public:
	virtual int doOperation(int num1, int num2) {
		return 0;
	};
};

class OperationAdd : public OperationBase {
public:
	int doOperation(int num1, int num2) {
		return num1 + num2;
	}
};

class OperationSubstract : public OperationBase {
public:
	int doOperation(int num1, int num2) {
		return num1 - num2;
	}
};

class OperationMultiply : public OperationBase {
public:
	int doOperation(int num1, int num2) {
		return num1 * num2;
	}
};

class OperationDevide : public OperationBase {
public:
	int doOperation(int num1, int num2) {
		return num1 / num2;
	}
};

class Operation {
public:
	void changeOperation(OperationBase* ob) {
		this->base = ob;
	}
	int executeOperation(int num1, int num2) {
		return this->base->doOperation(num1, num2);
	}
private:
	OperationBase* base;
};

class Error {
public:
	virtual void print() = 0;
};

class BracketError : public Error {
	void print() {
		cout << "괄호 오류" << endl;
		exit(0);
	}
};

class InputError : public Error {
	void print() {
		cout << "입력 오류" << endl;
		exit(0);
	}
};

class ExpressionError : public Error {
	void print() {
		cout << "수식 오류" << endl;
		exit(0);
	}
};

class AbstractFactory {
public:
	virtual Error* getError(string errorType) = 0;
};

class ErrorFactory : public AbstractFactory {
public:
	Error* getError(string errorType) {
		if (errorType == "괄호") {
			return new BracketError();
		}
		else if (errorType == "입력") {
			return new InputError();
		}
		else if (errorType == "수식") {
			return new ExpressionError();
		}
		return NULL;
	}
};

ErrorFactory* ef = new ErrorFactory();
Error* err;

int priority(char ch) {
	switch (ch) {
	case '(':
	case ')':
		return 0;
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
		return 2;
	}
}

void infixToPostfix(char infix[], char postfix[]) {
	std::stack<char> stack;
	int i = 0;
	int j = 0;
	int bracket = 0;

	while (infix[i] != NULL) {
		if (infix[i] == '(') {
			stack.push(infix[i]);
			i++;
			bracket++;
		}
		else if (infix[i] == ')') {
			if (bracket == 0) {
				err = ef->getError("괄호");
				err->print();
			}
			while (stack.top() != '(') {
				postfix[j] = stack.top();
				stack.pop();
				j++;
				postfix[j] = ' ';
				j++;
			}
			stack.pop();
			i++;
			bracket--;
		}
		else if (infix[i] == '+' || infix[i] == '-' || infix[i] == '*' || infix[i] == '/') {
			while (!stack.empty() && priority(infix[i]) <= priority(stack.top())) {
				postfix[j] = stack.top();
				stack.pop();
				j++;
				postfix[j] = ' ';
				j++;
			}
			stack.push(infix[i]);
			i++;
		}
		else if ('0' <= infix[i] && infix[i] <= '9') {
			if (infix[i] == '0') {
				i++;
				if (infix[i] == 'x') {
					i++;
					char num[100] = "";
					int k = 0;
					while (isdigit(infix[i])) {
						num[k] = infix[i];
						i++;
						k++;
					}
					if (sizeof(num) > 100) {
						err = ef->getError("입력");
						err->print();
					}
					int n = stoi(num, nullptr, 16);
					char ch[100] = "";
					_itoa_s(n, ch, 10);
					if (sizeof(ch) > 100) {
						err = ef->getError("입력");
						err->print();
					}
					k = 0;
					do {
						postfix[j] = ch[k];
						j++;
						k++;
					} while (ch[k] != NULL);
					postfix[j] = ' ';
					j++;
				}
				else if (infix[i] == 'b') {
					i++;
					char num[100] = "";
					int k = 0;
					while (isdigit(infix[i])) {
						num[k] = infix[i];
						i++;
						k++;
					}
					if (sizeof(num) > 100) {
						err = ef->getError("입력");
						err->print();
					}
					int n = stoi(num, nullptr, 2);
					char ch[100] = "";
					_itoa_s(n, ch, 10);
					if (sizeof(ch) > 100) {
						err = ef->getError("");
						err->print();
					}
					k = 0;
					do {
						postfix[j] = ch[k];
						j++;
						k++;
					} while (ch[k] != NULL);
					postfix[j] = ' ';
					j++;
				}
			}
			else {
				do {
					postfix[j] = infix[i];
					i++;
					j++;
				} while ('0' <= infix[i] && infix[i] <= '9');
				postfix[j] = ' ';
				j++;
			}
		}
		else {
			err = ef->getError("입력");
			err->print();
		}
	}

	if (bracket != 0) {
		err = ef->getError("괄호");
		err->print();
	}

	while (!stack.empty()) {
		postfix[j] = stack.top();
		stack.pop();
		j++;
		postfix[j] = ' ';
		j++;
	}
}

int calculate(char postfix[]) {
	std::stack<int> stack;
	int i = 0;
	int n;
	int answer;

	while (postfix[i] != NULL) {
		if (isdigit(postfix[i])) {
			char num[100] = "";
			int j = 0;
			while (isdigit(postfix[i])) {
				num[j] = postfix[i];
				i++;
				j++;
			}
			if (sizeof(num) > 100) {
				err = ef->getError("입력");
				err->print();
			}
			n = atoi(num);
			stack.push(n);
		}
		else if (postfix[i] == ' ') {
			i++;
		}
		else {
			int n2 = stack.top();
			stack.pop();
			if (stack.empty()) {
				err = ef->getError("수식");
				err->print();
			}
			int n1 = stack.top();
			stack.pop();
			Operation* op = new Operation();
			switch (postfix[i]) {
			case '+':
				op->changeOperation(new OperationAdd());
				stack.push(op->executeOperation(n1, n2));
				i++;
				break;
			case '-':
				op->changeOperation(new OperationSubstract());
				stack.push(op->executeOperation(n1, n2));
				i++;
				break;
			case '*':
				op->changeOperation(new OperationMultiply());
				stack.push(op->executeOperation(n1, n2));
				i++;
				break;
			case '/':
				op->changeOperation(new OperationDevide());
				stack.push(op->executeOperation(n1, n2));
				i++;
				break;
			}
		}
	}

	if (stack.empty()) {
		err = ef->getError("입력");
		err->print();
	}
	answer = stack.top();
	stack.pop();

	return answer;
}

int main() {
	string input;
	char infix[100] = "";
	char postfix[100] = "";
	cout << "수식 입력 : ";
	cin >> input;
	if (input.size() > 100) {
		err = ef->getError("입력");
		err->print();
	}
	strcpy(infix, input.c_str());
	infixToPostfix(infix, postfix);
	int result = calculate(postfix);
	cout << "답 : " << result << endl;

	return 0;
}