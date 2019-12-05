const express = require('express');
const app = express();
const path = require('path');
const router = express.Router();
var http = require('http').Server(app);
var io = require('/usr/local/lib/node_modules/socket.io')(http);

const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');

const port = new SerialPort('/dev/ttyACM1', { baudRate: 9600 });
const parser = port.pipe(new Readline({ delimiter: '\n' }));

app.get('/',function(req,res){
        res.sendFile(path.join(__dirname+'/public/index.html'));
  //__dirname : It will resolve to your project folder.
});


app.use(express.static(__dirname + '/public'));;
//add the router
app.use('/', router);
//app.listen(process.env.port || 3001);

io.on('connection', function(socket){
  console.log('an user connected');
});

http.listen(3000, function(){
  console.log('listening on *:3000');
});

io.on('connection',function(socket){
	socket.on('send', function(msg){
		port.write(msg, (err) => {
    		if (err) {
      			return console.log('Error on write: ', err.message);
    		}
    		console.log('message written');
  		});
	});
});

parser.on('data', data =>{
  console.log('got word from arduino:', data);
});

