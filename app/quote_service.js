
// Includes.
var express = require('express');
var temp = require('temp');
var app = express();
var exec = require('child_process').exec;
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

function isUpload(req)
{
    var method = req.method;

    return (method === 'PATH' || method === 'POST' || method === 'PUT');
}

function respond(req, res, next)
{
    if ('HEAD' == req.method) {
        res.send(200, 'OK');
        return;
    }

    res.set('Content-Type', 'text/json');
    if (isUpload(req)) {
        /* Never chunked. */
        req.on('data', function (data) {
            var model = JSON.parse(data);
            model.id = id++;
            generateImage(res, model);
        });
        return;
    }

    res.send(200, 'OK');
}

app.head('/quote', respond);
app.post('/quote', respond);

app.use(express.static(__dirname + uiDirname));
app.use(tempDirname, express.static(__dirname + tempDirname));

app.set('title', 'Quote app');

var server = app.listen(8080, function() {
    console.log('%s listening on %s', app.get('title'), server.address().port);
});
