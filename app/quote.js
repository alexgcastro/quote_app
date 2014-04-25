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

    var QuoteView = React.createClass({
        getInitialState: function() {
            this.text = $("#text");
            this.author = $("#author");

            this.quotes = new QuoteList;
            this.quotes.on("add", this.changeState);
            this.quotes.on("change", this.changeState);

            $("#text").bind("keypress", this.createQuote);
            $("#author").bind("keypress", this.createQuote);

            return {quote: null};
        },

        render: function() {
            var quote = this.state.quote;

            if (!quote)
                return <div> </div>;

            return <img src={quote.get("image")} alt={quote.get("text")+" -- "+quote.get("author")}></img>;
        },

        changeState: function(quote) {
            this.setState({quote: quote});
        },

        createQuote: function(key) {
            if (key.keyCode != 13) return;
            if (!this.text.val() || (!this.author.val())) return;

            var oldQuote = this.quotes.pop();
            if (oldQuote) oldQuote.trigger("destroy");

            this.quotes.create({text: this.text.val(), author: this.author.val(), template: 2});

            this.text.val("");
            this.author.val("");
        }

    });

    React.renderComponent(<QuoteView />, $("#main").get(0));
});
