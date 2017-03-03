"use strict";
const i2c = require('i2c');
const address = 0x08;
const wire = new i2c(address, {device: '/dev/i2c-1'});

const http    = require('http');
const express = require('express');
const WebStreamerServer = require('./lib/raspivid');

const app  = express();
const expressWs = require('express-ws')(app);

app.use(express.static(__dirname + '/public'));
app.use(express.static(__dirname + '/vendor/dist'));

app.get('/ws', function(req, res, next){
  console.log('get route', req.testing);
  res.end();
});

app.ws('/ws', function(ws, req) {
  ws.on('message', function(msg) {
    console.log(msg);
    wire.writeBytes(0x01,JSON.parse(msg), function(err){});
  });
});

app.get('/mode', function(req, res, next){
  console.log('get route', req.testing);
  res.end();
});

app.ws('/mode', function(ws, req) {
  ws.on('message', function(msg) {
    console.log(msg);
    wire.writeBytes(0x00,JSON.parse(msg), function(err){});
  });
});

const server  = http.createServer(app);
const silence = new WebStreamerServer(server);

server.listen(8081, function () {
  console.log('App listening');
    wire.writeBytes(0x00,[1], function(err){});
});
