require(['jquery', 'backbone', 'react'], function($, Backbone, React) {

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
        url: 'http://localhost:8080/quote',
        comparator: 'id'
    });

    var quotes = new QuoteList;

    var QuoteView = React.createClass({
        getInitialState: function() {
            this.text = $("#text");
            this.author = $("#author");
            this.main = $('#main');

            $("#text").bind("keypress", this.createQuote);
            $("#author").bind("keypress", this.createQuote);

            quotes.listenTo(quotes, 'add', this.doRender);
            quotes.listenTo(quotes, 'change', this.doRender);

            return {quote: null};
        },

        render: function() {
            if (!this.props.quote)
                return <div> </div>;

            var quote = this.props.quote;

            return <img src={quote.image} alt={quote.text+" -- "+quote.author}></img>;
        },

        doRender: function(quote) {
            React.renderComponent(<QuoteView quote={_.clone(quote.attributes)} />, this.main.get(0));
        },

        createQuote: function(key) {
            if (key.keyCode != 13) return;
            if (!this.text.val() || (!this.author.val())) return;

            var oldQuote = quotes.pop();
            if (oldQuote) oldQuote.trigger('destroy');

            quotes.create({text: this.text.val(), author: this.author.val(), template: 2});

            this.text.val('');
            this.author.val('');
        }

    });

    React.renderComponent(<QuoteView />, $('#main').get(0));
});
