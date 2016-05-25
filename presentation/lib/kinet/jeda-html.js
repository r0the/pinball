// don't let jQuery parse dynamically loaded content
$.ajaxSetup({
    mimeType: "text/plain"
});

var jeda = jeda || {};

jeda.html = function() {
    var _checkAnswerInput = function(element) {
        var target = $(element);
        var group = $(target.parent().parent());
        var expect = target.attr('solution');
        var got = target.val().toUpperCase();
        if (expect == got) {
            group.removeClass('has-danger');
            group.addClass('has-success');
            target.removeClass('form-control-danger');
            target.addClass('form-control-success');
        }
        else {
            group.removeClass('has-success');
            group.addClass('has-danger');
            target.removeClass('form-control-success');
            target.addClass('form-control-danger');
        }
    };

    var _initAnswerInputs = function() {
        $('input.answer').keyup(function(event) {
            _checkAnswerInput(event.target);
        });

        $('input.answer').each(function(index, element) {
            _checkAnswerInput(element);
        });
    };

    var _initOptionGroups = function() {
        $('.option-group button').click(function(event) {
            $(event.target).siblings().removeClass('active');
            $(event.target).addClass('active');
        });
    };

    var _createTable = function(data, thRows, thColumns) {
        var html = '';
        for (var r = 0; r < data.data.length; ++r) {
            html += '<tr>';
            for (c = 0; c < data.data[0].length; ++c) {
                if (r < thRows || c < thColumns) {
                    html += '<th>';
                }
                else {
                    html += '<td>';
                }

                html += data.data[r][c];
                if (r < thRows || c < thColumns) {
                    html += '</th>';
                }
                else {
                    html += '</td>';
                }
            }

            html += '</tr>';
        }

        return html;
    };

    var _loadContent = function(pattern, onSuccess) {
        $(pattern).each(function(i, e) {
            var element = $(e);
            var src = element.attr('data-src');
            $.ajax(src, {
                success: function(data) {
                    onSuccess(element, data);
                },
                error: function() {
                    console.error('Cannot load \'' + src + '\'.');
                }
            });
        });
    };

    var _loadCode = function() {
        _loadContent('.load-code', function(element, data) {
            element.text(data);
            if (typeof hljs != 'undefined') {
                hljs.highlightBlock(element.context);
            }
        });
    };

    var _loadHtml = function() {
        _loadContent('.load-html', function(element, data) {
            element.html(data);
            _initOptionGroups();
        });
    };

    var _loadTables = function() {
        _loadContent('.load-table', function(element, data) {
            element.append(_createTable(JSON.parse(data)));
        });
    }

    var _initialize = function() {
        _initAnswerInputs();
        _loadCode();
        _loadHtml();
        _loadTables();
    }

    return {
        initialize: _initialize
    };
}();

if (window.jQuery) {
    $(document).ready(function() {
        jeda.html.initialize();
        if (typeof katex != 'undefined') {
            $('.math').each(function(i, e) {
                var src = $(e).html();
                katex.render(src, e);
            });

            $('var').each(function(i, e) {
                var src = $(e).html();
                katex.render(src, e);
            });

            $('.math-display').each(function(i, e) {
                var src = $(e).html();
                katex.render(src, e, { displayMode: true});
            });
        }
    });
}
