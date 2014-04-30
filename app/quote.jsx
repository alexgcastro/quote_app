/** @jsx React.DOM */
require(["jquery", "backbone", "react"], function($, Backbone, React) {

    /* React component that renders the quote. */
    var QuoteView = React.createClass({
        render: function() {
            return <img src={this.props.image} alt={this.props.text+" -- "+this.props.author}></img>;
        }
    });

    function renderQuote(quote) {
        if (quote._id === undefined)
            return;

        document.title = "Quote from "+quote.author;

        React.renderComponent(<QuoteView image={quote.image} text={quote.text} author={quote.author} />, $("#main").get(0));
    }


    /*  Quote persistence. */
    function generateQuote(text, author, template) {
        $.ajax({
            url: '/quoteService',
            async: true,
            data: JSON.stringify({text: text,
                                  author: author,
                                  template: template}),
            dataType: 'json',
            type: 'POST',
            success: function(attributes) {
                router.navigate("quote/"+attributes._id);
                renderQuote(attributes);
            }
        });
    }

    function fetchQuote(id) {
        $.ajax({
            url: '/quoteService/'+id,
            async: true,
            dataType: 'json',
            type: 'GET',
            success: function(attributes) {
                renderQuote(attributes);
            }
        });
    }

    /* App routes definition. */
    var Router = Backbone.Router.extend({
        routes: {
            "quote/:id": "getQuote",
            "*action": "default"
        }
    });

    var router = new Router();

    router.on("route:getQuote", function(id) {
        fetchQuote(id);
    });

    router.on("route:default", function() {
        renderQuote({});
    });

    Backbone.history.start({pushState: true});

    /* Form events handlers. */
    function createQuote(key) {
        if (key.keyCode != 13) return;
        if (!$("#text").val() || (!$("#author").val())) return;

        generateQuote($("#text").val(), $("#author").val(), 1);

        $("#text").val("");
        $("#author").val("");
    }

    $("#text").bind("keypress", createQuote);
    $("#author").bind("keypress", createQuote);
});
