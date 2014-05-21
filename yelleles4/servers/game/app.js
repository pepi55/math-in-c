var exp = require("express"),
		app = exp(),
		server = require("http").createServer(app),
		io = require("socket.io").listen(server);

server.listen(1337);

app.get("/", function(req, res) {
	res.sendfile(__dirname + "/index.html");
});

io.sockets.on("connection", function(socket) {
	console.log("=========> works <=========");
});
