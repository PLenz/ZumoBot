"use strict";

const i2c = require('i2c');
const util = require('util');
const address = 0x08;
const wire = new i2c(address, {device: '/dev/i2c-1'});
const WebSocket = require('ws');
const http    = require('http');
const express = require('express');
const WebStreamerServer = require('./lib/raspivid');
const app  = express();
//const expressWs = require('express-ws')(app);
const wss = new WebSocket.Server({ port: 8082 });

process.on('uncaughtException', (err) => {
  console.log(1, `Caught exception: ${err}`);
});

app.use(express.static(__dirname + '/public'));
app.use(express.static(__dirname + '/vendor/dist'));


wss.on('connection', function connection(client) {
  console.log('New Operator Connected');
  client.on('message', function incoming(message) {
    console.log(util.inspect(message));
    const data = JSON.parse(message);
    if(data.mode == "0"){
      wire.writeBytes(0x00,data.data, function(err){});
    }
    else if (data.mode == "1"){
      wire.writeBytes(0x01,data.data, function(err){});
    }

  });

    client.on('close', function close() {
      try{
        console.log('Operator Disconnected');
        wire.writeBytes(0x00,[0], function(err){});
      }
      catch(e){

      }
    });

    client.on("error", function(error) {
    // Manage error here
    console.log(error);
});

  client.send('something');
});

const server  = http.createServer(app);
const silence = new WebStreamerServer(server);


server.listen(8081, function () {
  console.log('App listening');
    wire.writeBytes(0x00,[1], function(err){});
});
