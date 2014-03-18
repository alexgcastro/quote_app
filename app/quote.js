$(function(){

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

        comparator: 'id',

        parse: function(resp, options) {
            return JSON.parse(resp);
        }

    });

    var Quotes = new QuoteList;

    var QuoteView = Backbone.View.extend({
        tagName:  "li",

        template: _.template($('#quote-template').html()),

        events: {
            "click a.destroy" : "clear",
        },

        initialize: function() {
            this.listenTo(this.model, 'change', this.render);
            this.listenTo(this.model, 'destroy', this.remove);
        },

        render: function() {
            this.$el.html(this.template(this.model.toJSON()));
            return this;
        },

        clear: function() {
            this.model.destroy();
        }

    });

    var AppView = Backbone.View.extend({

        el: $("#quoteapp"),

        events: {
            "keypress #text":  "createOnEnter",
            "keypress #author":  "createOnEnter",
            "click #clear-quotes": "clearQuotes",
        },

        initialize: function() {

            this.text = this.$("#text");
            this.author = this.$("#author");
            this.image = this.$("#image");

            this.listenTo(Quotes, 'add', this.addOne);
            this.listenTo(Quotes, 'all', this.render);

            this.footer = this.$('footer');
            this.main = $('#main');

            Quotes.fetch();
        },

        render: function() {
            if (Quotes.length) {
                this.main.show();
                this.footer.show();
            } else {
                this.main.hide();
                this.footer.hide();
            }
        },

        addOne: function(quote) {
            var view = new QuoteView({model: quote});
            this.$("#quote-list").append(view.render().el);
        },

        addAll: function() {
            Quotes.each(this.addOne, this);
        },

        createOnEnter: function(e) {
            if (e.keyCode != 13) return;
            if (!this.text.val() || (!this.author.val())) return;

            Quotes.create({text: this.text.val(), author: this.author.val(), template: 2});
            this.text.val('');
            this.author.val('');
        },

        clearQuotes: function() {
            _.invoke(_.clone(Quotes.models), 'destroy');
            return false;
        }
    });

    var App = new AppView;

});
