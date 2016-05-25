var Kinet = {};

Kinet.setup = function(params) {
    $(document).ready(function() {
        // setup header with logo
        if (params.official) {
            $('body').prepend('<div class="gk-blue-bar"></div><header class="gk-official"><div class="gk-logo"></div></header>');
        }
        else {
            $('body').prepend('<header class="gk-internal"><div class="gk-logo"></div></header>');
        }

        // setup syntax highlighting
        hljs.initHighlightingOnLoad();

        // setup bubbles
        $('.bubble').each(function(i, e) {
            var element = $(e);
            var top = element.attr('top');
            var left = element.attr('left');
            var color = element.attr('color');
            var background = element.attr('background');
            var css = 'position: absolute; ';
            if (typeof top === 'string') {
                css += 'top: ' + top + ';';
            }

            if (typeof left === 'string') {
                css += 'left: ' + left + ';';
            }

            element.attr('style', css);
        });
    });
}
