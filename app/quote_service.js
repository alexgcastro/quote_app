
// Includes.
var express = require('express');
var temp = require('temp');
var app = express();
var exec = require('child_process').exec;
var text_rendering = require('./Release/text_rendering');

// Variables.
var Quotes = [];
var id = 1;

// Defines.
var tempDirname = '/generated_images';

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

    if ('DELETE' == req.method) {
        console.log('Deleting %s!', req.params.id);
        removeById(req.params.id);
        res.send(200, 'OK');
        return;
    }

    if (isUpload(req)) {
        /* Never chunked. */
        req.on('data', function (data) {
            var model = JSON.parse(data);
            model.id = id++;
            Quotes.push(model);
            console.log('Received quote: ' + model.id + ' ' + model.text + ' ' + model.author + ' ' + model.template);
            generateImage(res, model);
        });
        return;
    }

    if (req.params.id != undefined)
        res.send(200, JSON.stringify(Quotes[req.params.id]));

    res.send(200, JSON.stringify(Quotes));
}

var enableCORS = function(req, res, next) {
    res.header('Access-Control-Allow-Origin', '*');
    res.header('Access-Control-Allow-Methods', 'GET,PUT,POST,DELETE,OPTIONS');
    res.header('Access-Control-Allow-Headers', 'Content-Type, Authorization, Content-Length, X-Requested-With');

    if ('OPTIONS' == req.method)
        res.send(203, 'OK');
    else
        next();
};

app.use(enableCORS);

app.head('/quote', respond);
app.del('/quote/:id', respond);
app.post('/quote', respond);
app.get('/quote/:id', respond);
app.get('/quote', respond);

app.use(tempDirname, express.static(__dirname + tempDirname));

app.set('title', 'Quote app');

var server = app.listen(8080, function() {
    console.log('%s listening on %s', app.get('title'), server.address().port);
});
