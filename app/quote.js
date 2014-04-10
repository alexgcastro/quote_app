require(['underscore', 'jquery', 'backbone'], function() {

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

    var QuoteView = Backbone.View.extend({
        tagName:  "div",

        className: "view",

        template: _.template($('#quote-template').html()),

        initialize: function() {
            this.listenTo(this.model, 'change', this.render);
            this.listenTo(this.model, 'destroy', this.remove);
        },

        render: function() {
            this.$el.html(this.template(this.model.toJSON()));
            return this;
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

            this.listenTo(Quotes, 'add', this.add);

            this.footer = this.$('footer');
            this.main = $('#main');
        },

        add: function(quote) {
            var view = new QuoteView({model: quote});
            this.$("#main").append(view.render().el);
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
