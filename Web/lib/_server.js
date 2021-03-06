"use strict";


const WebSocketServer = require('ws').Server;
const Splitter        = require('stream-split');
const merge           = require('mout/object/merge');
const spawn     = require('child_process').spawn;

const NALseparator    = new Buffer([0,0,0,1]);//NAL break


class _Server {

  constructor(server, options) {

    this.options = merge({
        width : 640,
        height: 360,
    }, options);

    this.wss = new WebSocketServer({port: 8080, server });

    this.new_client = this.new_client.bind(this);
    this.start_feed = this.start_feed.bind(this);
    this.broadcast  = this.broadcast.bind(this);

    this.wss.on('connection', this.new_client);
  }
  

  start_feed() {
    var readStream = this.get_feed();
    this.readStream = readStream;

    readStream = readStream.pipe(new Splitter(NALseparator));
    readStream.on("data", this.broadcast);
  }

  get_feed() {
    throw new Error("to be implemented");//TODO
  }

  broadcast(data) {
    this.wss.clients.forEach(function(socket) {

      if(socket.buzy){
        return;
      }

      socket.buzy = true;
      socket.buzy = false;

      socket.send(Buffer.concat([NALseparator, data]), { binary: true}, function ack(error) {
        socket.buzy = false;
      });
    });
  }

  new_client(socket) {
  
    var self = this;
    console.log('New Video User');

    socket.send(JSON.stringify({
      action : 'init',
      width  : this.options.width,
      height : this.options.height,
    }));

    socket.on('message', function(data){
      const action = String(data).split(' ')[0];

      console.log("Incomming action '%s'", action);

      if(action === "REQUESTSTREAM"){
        self.start_feed();
      }
        
      if(action == "STOPSTREAM")
        self.readStream.pause();
    });

    socket.on('close', function() {
      try{
        console.log('stopping client interval');
          console.log('Killing raspvid');
          spawn('killall',['raspivid']);
        //self.readStream.end();
        self.readStream = null;
      }
      catch(e){
        
      }


    });
  }


};


module.exports = _Server;
