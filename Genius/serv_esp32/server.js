const express = require('express');
const app = express();
const http = require('http');
const WebSocket = require('ws');
const path = require('path');
const request = require('request');

const port = 3000;
const server = http.createServer(app);
const wss = new WebSocket.Server({ server });

//routers
app.use(express.static(path.join(__dirname, 'public')));
app.set('views', path.join(__dirname, 'public'));
app.engine('html', require('ejs').renderFile);
app.set('view engine', 'html');

app.use('/', (req, res) => {
    res.render('index.html');
});

let clients = [];
let ledNumber = 0;

wss.on('connection', (ws) => {
    clients.push(ws);
    ws.on('message', (message) => {
        ledNumber = message; 
        console.log(message);
        clients.forEach(function (client) {
                client.send(ledNumber);
        });
    })
});

server.listen(port);