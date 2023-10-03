import os

def evaluate():
	expression = os.environ.get('EXPR')

	if expression is not None:
		try:
			result = eval(expression)
		except Exception as e:
			result = "Error"
	else:
		result = "Error"
	return result

if __name__ == '__main__':
	print(evaluate())