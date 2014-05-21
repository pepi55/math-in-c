var express = require("express");
var app = express();
var server = require("http").createServer(app);
var io = require("socket.io").listen(server);

server.listen(1337);

app.get("/", function(req, res) {
	res.sendfile(__dirname + "/index.html");
});

io.sockets.on("connection", function(socket) {
	console.log("=========> works <=========");

	socket.on("send", function(data) {
		io.sockets.emit("new-msg", data);
	});
});
