document.addEventListener('DOMContentLoaded', function() {
	const outputDisplay = document.getElementById('output');
	const buttons = document.getElementsByClassName('button');
	let currentExpression = '';

	for (const button of buttons) {
		button.addEventListener('click', () => {
			const buttonText = button.textContent;

			if (buttonText === 'C')
				currentExpression = '';
			else if (buttonText === '=')
				sendExpression(currentExpression);
			else
				currentExpression += buttonText;

			outputDisplay.textContent = currentExpression;
		});
	}

	function sendExpression(expression) {
		const uri = '/calc';
		const requestBody = `expr=${expression}`;
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
				console.log('Success:', result)
			})
			.catch((error) => {
				console.error('Error:', error);
			});
	}

	function updateOutput(result) {
		outputDisplay.textContent = result;
	}
});
