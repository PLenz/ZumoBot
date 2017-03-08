"use strict";

const http    = require('http');
const i2c = require('i2c');
const address = 0x08;
const wire = new i2c(address, {device: '/dev/i2c-1'});
const WebStreamerServer = require('./lib/raspivid');

const express = require('express');
const app = express();
const expressWs = require('express-ws')(app);

const server  = http.createServer(app);
const silence = new WebStreamerServer(server);

app.use(express.static(__dirname + '/public'));
app.use(express.static(__dirname + '/vendor/dist'));

app.use((req, res, next) => {
  console.log('middleware');
  req.testing = 'testing';
  return next();
});

app.get('/',(req, res) => {
   res.sendFile( __dirname + "/public/index.html" );
})

app.get('/key', function (req, res) {
   res.sendFile( __dirname + "/public/key.html" );
})

app.get('/manual', function (req, res) {
   res.sendFile( __dirname + "/public/manual.html" );
})

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

app.get('/index.html', function (req, res) {
   res.sendFile( __dirname + "/public/index.html" );
})

app.get('/js/backbone-min.js', function (req, res) {
   res.sendFile( __dirname + "/public/js/backbone-min.js" );
})

app.get('/js/joystick_view.js', function (req, res) {
   res.sendFile( __dirname + "/public/js/joystick_view.js" );
})

app.get('/js/jquery-1.11.2.min.js', function (req, res) {
   res.sendFile( __dirname + "/public/js/jquery-1.11.2.min.js" );
})

app.get('/js/underscore-min.js', function (req, res) {
   res.sendFile( __dirname + "/public/js/underscore-min.js" );
})

app.get('/img/button.png', function (req, res) {
   res.sendFile( __dirname + "/public/img/button.png" );
})

app.get('/img/canvas.png', function (req, res) {
   res.sendFile( __dirname + "/public/img/canvas.png" );
})

var server = app.listen(8081);
