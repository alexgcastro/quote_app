
var restify = require('restify');
var server = restify.createServer();
var exec = require('child_process').exec;
var Quotes = [];
var id = 1;

function addHeaders(res) {
  res.header('Access-Control-Allow-Origin ', '*');
  res.header('Access-Control-Allow-Method', 'POST, GET, PUT, DELETE, OPTIONS' );
  res.header('Access-Control-Allow-Headers', 'Origin, X-Requested-With, X-File-Name, Content-Type, Cache-Control' );
}

function removeById(id)
{
    for(var i=0; i < Quotes.length; i++)
    {
        if(Quotes[i].id == id) {
            Quotes.splice(i, 1);
            return;
        }
    }
}

function log_error(error, stdout, stderr)
{
  if (stdout) console.log('pango-view_stdout: ' + stdout);
  if (stderr) console.log('pango-view_stderr: ' + stderr);
  if (error !== null)
      console.log('node-pango-view error: ' + error);
}

function generateImage(res, model)
{
    var text = model.text+'\n\n\n'+model.author;
    var cmd = 'pango-view --background=#80A0C6 --align=center --font="sans bold 18" --width=350 --hinting=full --language=es --no-display --output=./generated_images/thetextimage.png --text="' + text + '"';

    var child = exec(cmd, log_error);

    child.on('exit', function (code, signal) {
        model.image = 'http://localhost:8080/generated_images/thetextimage.png';
        res.send(model);
    });
}

function respond(req, res, next)
{

  addHeaders(res);

  if ('HEAD' == req.method) {
      res.send(200, 'OK');
      return;
  }

  if ('OPTIONS' == req.method) {
      res.send(203, 'OK');
      return;
  }

  if ('DELETE' == req.method) {
      console.log('Deleting %s!', req.params.id);
      removeById(req.params.id);
      res.send(200, 'OK');
      return;
  }

  if (req.isUpload()) {
      req.on('data', function (data) {
          var model = JSON.parse(data);
          model.id = id++;
          Quotes.push(model);
          console.log('Received quote: ' + model.id + ' ' + model.text + ' ' + model.author);
          generateImage(res, model);
      });
      return;
  }

  if (req.params.id != undefined)
      res.send(JSON.stringify(Quotes[req.params.id]));
  else if (Quotes.length > 0)
      res.send(JSON.stringify(Quotes));

  res.send(200, '');
}

server.head('/quote', respond);
server.del('/quote/:id', respond);
server.post('/quote', respond);
server.get('/quote/:id', respond);
server.get('/quote', respond);

server.get(/\/generated_images\/?.*/, restify.serveStatic({
    directory: './generated_images',
    maxAge: 0
}));

server.listen(8080, function() {
  console.log('%s listening at %s', server.name, server.url);
});
