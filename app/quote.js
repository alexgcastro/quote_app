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

    var Quotes = new QuoteList;

    var QuoteView = React.createClass({
        render: function () {
            if (!this.props.quote)
                return <div> </div>;

            var quote = this.props.quote;

            return <img src={quote.image} alt={quote.text+" -- "+quote.author}></img>;
        }
    });

    var AppView = Backbone.View.extend({

        el: $("#quoteapp"),

        events: {
            "keypress #text":  "createOnEnter",
            "keypress #author":  "createOnEnter",
        },

        initialize: function() {

            this.text = this.$("#text");
            this.author = this.$("#author");
            this.image = this.$("#image");

            this.listenTo(Quotes, 'add', this.renderQuote);
            this.listenTo(Quotes, 'change', this.renderQuote);

            this.main = $('#main');
        },

        renderQuote: function(quote) {
            React.renderComponent(<QuoteView quote={_.clone(quote.attributes)} />, this.main.get(0));
        },

        createOnEnter: function(e) {
            if (e.keyCode != 13) return;
            if (!this.text.val() || (!this.author.val())) return;

            var oldQuote = Quotes.pop();
            if (oldQuote) oldQuote.trigger('destroy');

            Quotes.create({text: this.text.val(), author: this.author.val(), template: 2});

            this.text.val('');
            this.author.val('');
        }
    });

    var App = new AppView;

});
