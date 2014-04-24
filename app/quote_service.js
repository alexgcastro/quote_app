
// Includes.
var express = require('express');
var temp = require('temp');
var app = express();
var text_rendering = require('./Release/text_rendering');

// Variables.
var id = 1;

// Defines.
var tempDirname = '/generated_images';
var uiDirname = '/ui';

function generateImage(res, model)
{
    var tempFilename = temp.path({dir: tempDirname, prefix: 'quote', suffix: '.png'});

    text_rendering.render(__dirname + tempFilename, model.text, model.author, model.template, function () {
        model.image = 'http://localhost:8080' + tempFilename;
        res.send(200, model);
    });
}

function respond(req, res, next)
{
    res.set('Content-Type', 'text/json');

    /* Never chunked. */
    req.on('data', function (data) {
        var model = JSON.parse(data);
        model.id = id++;
        generateImage(res, model);
    });
}

app.post('/quote', respond);

app.use(express.static(__dirname + uiDirname));
app.use(tempDirname, express.static(__dirname + tempDirname));

app.set('title', 'Quote app');

var server = app.listen(8080, function() {
    console.log('%s listening on %s', app.get('title'), server.address().port);
});
