const express = require('express');
const bodyParser = require('body-parser');

const app = express();
const port = 3000;

app.use(bodyParser.text({ type: 'text/plain' }));

app.post('/calc', (req, res) => {
  const expression = req.body;
  let result;

  try {
    result = eval(expression);
  } catch (e) {
    result = 'Error';
  }

  res.send(result.toString());
});

app.listen(port, () => {
  console.log(`Server is listening on port ${port}`);
});
