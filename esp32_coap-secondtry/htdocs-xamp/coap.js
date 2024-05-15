const coap = require('coap');
const server = coap.createServer();

const ledState = {
  value: 0
};

server.on('request', (req, res) => {
  const { url } = req;

  if (url === '/led') {
    handleLedRequest(req, res);
  } else {
    res.code = '4.04';
    res.end('Not Found');
  }
});

function handleLedRequest(req, res) {
  if (req.method === 'GET') {
    res.setOption('Content-Format', 'text/plain');
    res.end(ledState.value.toString());
  } else if (req.method === 'PUT') {
    let payload = '';
    req.on('data', (chunk) => {
      payload += chunk.toString();
    });
    req.on('end', () => {
      const newValue = parseInt(payload);
      if (newValue === 0 || newValue === 1) {
        ledState.value = newValue;
        console.log(`LED set to ${newValue}`);
        res.code = '2.04';
        res.end(payload);
      } else {
        res.code = '4.00';
        res.end('Bad Request');
      }
    });
  } else {
    res.code = '4.05';
    res.end('Method Not Allowed');
  }
}

server.listen(() => {
  console.log('CoAP server is listening');
});