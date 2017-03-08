var i2c = require('i2c');
var address = 0x08;
var wire = new i2c(address, {device: '/dev/i2c-1'});

var express = require('express');
var app = express();
var expressWs = require('express-ws')(app);
app.use(express.static('public'));

app.use(function (req, res, next) {
  console.log('middleware');
  req.testing = 'testing';
  return next();
});

app.get('/', function (req, res) {
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
