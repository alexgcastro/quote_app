
// Includes.
var express = require('express');
var temp = require('temp');
var app = express();
var text_rendering = require('./Release/text_rendering');
var mongoose = require('mongoose');

// Defines.
var tempDirname = '/generated_images';
var uiDirname = '/ui';

mongoose.connect('mongodb://localhost/quoteapp');

var QuoteSchema = mongoose.Schema({
    author: String,
    text: String,
    template: Number,
    image: String
});

var Quote = mongoose.model('Quote', QuoteSchema);

var db = mongoose.connection;

db.on('error', console.error.bind(console, 'connection error:'));
db.once('open', function callback () {
    console.log('Connection to db opened correctly!');
});

function generateImage(res, model)
{
    var tempFilename = temp.path({dir: tempDirname, prefix: 'quote', suffix: '.png'});

    text_rendering.render(__dirname + tempFilename, model.text, model.author, model.template, function () {
        model.image = 'http://localhost:8080' + tempFilename;
        res.send(200, model);
    });

    var theQuote = new Quote({author: model.author, text: model.text, template: model.template, image: tempFilename});

    theQuote.save(function (err) {
        if (err) return console.error(err);
    });

    model.id = theQuote._id;
}

function respondGET(req, res, next)
{
    res.set('Content-Type', 'text/json');

    if (req.query.id === undefined) {
        res.send(200, {});
        return;
    }

    Quote.findById(req.query.id).exec(function(err, model) {
        if (err) {
            res.send(200, {});
            return;
        };

        res.send(200, model);
    });
}

function respondPOST(req, res, next)
{
    res.set('Content-Type', 'text/json');

    /* Never chunked. */
    req.on('data', function (data) {
        var model = JSON.parse(data);
        generateImage(res, model);
    });
}

app.post('/quoteService', respondPOST);
app.get('/quoteService', respondGET);

app.use(express.static(__dirname + uiDirname));
app.use(tempDirname, express.static(__dirname + tempDirname));

app.set('title', 'Quote app');

var server = app.listen(8080, function() {
    console.log('%s listening on %s', app.get('title'), server.address().port);
});
