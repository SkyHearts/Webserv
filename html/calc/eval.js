#!/usr/bin/env node

function evaluateExpression(expression) {
	try {
		const result = eval(expression);
		return result.toString();
	} catch (error) {
		return 'Error';
	}
}

function main() {
	const expression = process.env.EXPR;

	if (expression) {
		const result = evaluateExpression(expression);
		console.log(result);
	} else {
		console.error('Error');
		process.exit(1);
	}
}

if (require.main === module) {
	main();
}