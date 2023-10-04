document.addEventListener('DOMContentLoaded', function() {
	const outputDisplay = document.getElementById('output');
	const buttons = document.getElementsByClassName('button');
	let currentExpression = '';

	for (const button of buttons) {
		button.addEventListener('click', () => {
			const buttonText = button.textContent;

			if (currentExpression === 'Error')
				currentExpression = '';

			if (buttonText === 'C')
				currentExpression = '0';
			else if (buttonText === '=')
				sendExpression(currentExpression);
			else {
				if (currentExpression === '0')
					currentExpression = buttonText;
				else
					currentExpression += buttonText;
			}

			outputDisplay.textContent = currentExpression;
		});
	}

	function sendExpression(expression) {
		const uri = '/calc';
		const requestBody = `EXPR=${expression}`;
		const headers = {
			'Content-Type': 'text/plain'
		};

		const requestOptions = {
			method: 'POST',
			headers: headers,
			body: requestBody,
		};

		fetch(uri, requestOptions)
			.then((response) => response.text())
			.then((result) => {
				updateOutput(result);
				currentExpression = result.replace(/\s+/g, '');
			})
			.catch((error) => {
				console.error('Error:', error);
			});
	}

	function updateOutput(result) {
		outputDisplay.textContent = result;
	}
});
