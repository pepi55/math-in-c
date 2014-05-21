var fs = require("fs");

require("http").createServer(function (req, res) {
	res.writeHead(200, {"Content-Type":"audio/ogg"});
	var rs = fs.createReadStream("public/mp3.ogg");

	rs.pipe(res);
	//res.end();
}).listen(1337);

//server.on('request', function (req, res) {
	//res.writeHead(200, {"Content-Type":"text/plain"});
	//res.write("Error: 404");
	//res.end();
//});

//server.listen(1336);
