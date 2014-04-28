require(["jquery", "backbone", "react"], function($, Backbone, React) {

    var Quote = Backbone.Model.extend({
        defaults: function() {
            return {
                text: null,
                author: null,
                image: null,
                template: null
            };
        },
    });

    var QuoteList = Backbone.Collection.extend({
        model: Quote,
        url: "http://localhost:8080/quoteService",
        comparator: "id"
    });

    var quotes = new QuoteList;

    var Router = Backbone.Router.extend({
        routes: {
            "quote/:quoteID": "getQuote",
            "*action": "default"
        }
    });

    var router = new Router;

    var QuoteView = React.createClass({
        getInitialState: function() {
            this.text = $("#text");
            this.author = $("#author");

            $("#text").bind("keypress", this.createQuote);
            $("#author").bind("keypress", this.createQuote);

            return null;
        },

        render: function() {
            if (!this.props.image)
                return <div> </div>;

            return <img src={this.props.image} alt={this.props.text+" -- "+this.props.author}></img>;
        },

        createQuote: function(key) {
            if (key.keyCode != 13) return;
            if (!this.text.val() || (!this.author.val())) return;

            var oldQuote = quotes.pop();
            if (oldQuote) oldQuote.trigger("destroy");

            quotes.create({text: this.text.val(), author: this.author.val(), template: 2, navigate: true});

            this.text.val("");
            this.author.val("");
        }

    });

    function quoteChanged(quote) {
        var id = quote.get("_id");

        if (id === undefined)
            return;

        React.renderComponent(<QuoteView image={quote.get("image")} text={quote.get("text")} author={quote.get("author")} />, $("#main").get(0));

        if (quote.get("navigate")) router.navigate("quote/"+id);
    }

    quotes.on("add", quoteChanged);
    quotes.on("change", quoteChanged);

    router.on("route:getQuote", function(quoteID) {
        var oldQuote = quotes.pop();
        if (oldQuote) oldQuote.trigger("destroy");

        quotes.fetch({data: {id: quoteID}});
    });

    router.on("route:default", function() {
        React.renderComponent(<QuoteView />, $("#main").get(0));
    });

    Backbone.history.start();
});
