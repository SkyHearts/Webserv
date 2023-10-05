#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>

// Function to evaluate a simple mathematical expression
double evaluateExpression(const char* expression) {
	// Use sscanf to parse the expression and evaluate it
	double result;
	if (sscanf(expression, "%lf", &result) == 1) {
		return result; // If the expression is just a number, return it
	} else {
		// Use a simple stack to evaluate +, -, *, / operations
		int len = strlen(expression);
		char operatorStack[len];
		double valueStack[len];
		int operatorTop = -1;
		int valueTop = -1;

		for (int i = 0; i < len; i++) {
			if (expression[i] == ' ')
				continue; // Skip spaces

			if (isdigit(expression[i]) || (expression[i] == '-' && (i == 0 || expression[i - 1] == '('))) {
				// Read numbers and negative signs
				double num;
				int numRead = sscanf(expression + i, "%lf", &num);
				if (numRead == 1) {
					valueStack[++valueTop] = num;
					while (i < len && (isdigit(expression[i]) || expression[i] == '.')) {
						i++;
					}
				}
			} else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') {
				// Handle operators
				while (operatorTop >= 0 && (expression[i] == '*' || expression[i] == '/') &&
					(operatorStack[operatorTop] == '+' || operatorStack[operatorTop] == '-')) {
					char op = operatorStack[operatorTop--];
					double b = valueStack[valueTop--];
					double a = valueStack[valueTop--];
					switch (op) {
						case '+':
							valueStack[++valueTop] = a + b;
							break;
						case '-':
							valueStack[++valueTop] = a - b;
							break;
						case '*':
							valueStack[++valueTop] = a * b;
							break;
						case '/':
							valueStack[++valueTop] = a / b;
							break;
					}
				}
				operatorStack[++operatorTop] = expression[i];
			} else if (expression[i] == '(') {
				// Handle opening parentheses
				operatorStack[++operatorTop] = expression[i];
			} else if (expression[i] == ')') {
				// Handle closing parentheses
				while (operatorTop >= 0 && operatorStack[operatorTop] != '(') {
					char op = operatorStack[operatorTop--];
					double b = valueStack[valueTop--];
					double a = valueStack[valueTop--];
					switch (op) {
						case '+':
							valueStack[++valueTop] = a + b;
							break;
						case '-':
							valueStack[++valueTop] = a - b;
							break;
						case '*':
							valueStack[++valueTop] = a * b;
							break;
						case '/':
							valueStack[++valueTop] = a / b;
							break;
					}
				}
				operatorTop--; // Pop the '('
			} else {
				fprintf(stderr, "Error: Invalid character in expression: %c\n", expression[i]);
				exit(1);
			}
		}

		// Evaluate any remaining operators
		while (operatorTop >= 0) {
			char op = operatorStack[operatorTop--];
			double b = valueStack[valueTop--];
			double a = valueStack[valueTop--];
			switch (op) {
				case '+':
					valueStack[++valueTop] = a + b;
					break;
				case '-':
					valueStack[++valueTop] = a - b;
					break;
				case '*':
					valueStack[++valueTop] = a * b;
					break;
				case '/':
					valueStack[++valueTop] = a / b;
					break;
			}
		}

		return valueStack[valueTop];
	}
}

int main() {
	// Retrieve the value of the "EXPR" environment variable
	char* exprEnv = getenv("EXPR");

	if (exprEnv == NULL) {
		printf("Error");
		return 1;
	}

	double result = evaluateExpression(exprEnv);
	printf("%lf", result);

	return 0;
}