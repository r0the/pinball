var jeda = jeda || {};

function executeFunctionByName(functionName, context, args) {
    var args = [].slice.call(arguments).splice(2);
    var namespaces = functionName.split(".");
    var func = namespaces.pop();
    for(var i = 0; i < namespaces.length; i++) {
        context = context[namespaces[i]];
    }

    return context[func].apply(context, args);
}

// ------------------------------------------------------------------------------------------------
// Log API
// ------------------------------------------------------------------------------------------------

var Log = {};

Log.d = function() {
    var i = 0;
    var msg = '';
    while (i < arguments.length) {
        msg = msg + arguments[i];
        ++i;
    }

    console.log(msg);
}

Log.e = function() {
    var i = 0;
    var msg = '';
    while (i < arguments.length) {
        msg = msg + arguments[i];
        ++i;
    }

    console.error(msg);
}

Log.i = function() {
    var i = 0;
    var msg = '';
    while (i < arguments.length) {
        msg = msg + arguments[i];
        ++i;
    }

    console.info(msg);
}

Log.w = function() {
    var i = 0;
    var msg = '';
    while (i < arguments.length) {
        msg = msg + arguments[i];
        ++i;
    }

    console.warn(msg);
}

// ------------------------------------------------------------------------------------------------
// Alignment API
// ------------------------------------------------------------------------------------------------

var Alignment = {};

Alignment.min = function(pos) {
    return pos;
}

Alignment.max = function(pos, size) {
    return pos - size;
}

Alignment.middle = function(pos, size) {
    return pos - size / 2;
}

Alignment.BOTTOM_CENTER = {x: Alignment.middle, y: Alignment.min};
Alignment.BOTTOM_LEFT = {x: Alignment.min, y: Alignment.min};
Alignment.BOTTOM_RIGHT = {x: Alignment.max, y: Alignment.min};
Alignment.CENTER = {x: Alignment.middle, y: Alignment.middle};
Alignment.LEFT = {x: Alignment.min, y: Alignment.middle};
Alignment.RIGHT = {x: Alignment.max, y: Alignment.middle};
Alignment.TOP_CENTER = {x: Alignment.middle, y: Alignment.max};
Alignment.TOP_LEFT = {x: Alignment.min, y: Alignment.max};
Alignment.TOP_RIGHT = {x: Alignment.max, y: Alignment.max};

// ------------------------------------------------------------------------------------------------
// Color API
// ------------------------------------------------------------------------------------------------

var Color = function() {
    if (arguments.length == 1) {
        this.value = arguments[0];
    }
}

Color.AMBER_50 = new Color("#fff8e1");
Color.AMBER_100 = new Color("#ffecb3");
Color.AMBER_200 = new Color("#ffe082");
Color.RED_200 = new Color("#ef9a9a");
Color.RED_A700 = new Color("#d50000");

// ------------------------------------------------------------------------------------------------
// Canvas API
// ------------------------------------------------------------------------------------------------

Canvas = function(canvas) {
    var fontHeightCache = [];

    determineFontHeight = function(fontStyle) {
        var result = fontHeightCache[fontStyle];
        if (!result) {
            var body = document.getElementsByTagName('body')[0];
            var dummy = document.createElement('div');
            var dummyText = document.createTextNode('M');
            dummy.appendChild(dummyText);
            dummy.setAttribute('style', fontStyle + ';position:absolute;top:0;left:0');
            body.appendChild(dummy);
            result = dummy.offsetHeight;
            fontHeightCache[fontStyle] = result;
            body.removeChild(dummy);
        }

        return result;
    };

    var _alignment = Alignment.BOTTOM_LEFT;
    var _element;
    if (typeof canvas === 'string') {
        _element = document.getElementById(canvas);
    }
    else {
        _element = canvas;
    }

    var _ctx = _element.getContext('2d');
    var _color = '#000000';
    var _textSize = '16px';
    var _fontName = 'sans-serif';
    _ctx.font = _textSize + ' ' + _fontName;

    var buildCircle = function(x, y, r) {
        if (r <= 0) {
            Log.d('Not drawing circle with non-positive radius.');
            return;
        }

        _ctx.beginPath();
        _ctx.arc(x, _element.height - y, r, 0, 2*Math.PI);
        _ctx.closePath();
    }

    var buildEllipse = function(x, y, rx, ry) {
        if (rx <= 0 || ry <= 0) {
            Log.d('Not drawing ellipse with non-positive radius.');
            return;
        }

        _ctx.save();
        _ctx.translate(x, _element.height - y);
        _ctx.scale(1, ry / rx);
        _ctx.beginPath();
        _ctx.arc(0, 0, rx, 0, 2*Math.PI);
        _ctx.closePath();
        _ctx.restore();
    }

    var buildPoints = function(args) {
        var result = [];
        var i = 0;
        while (i < args.length - 1) {
            result.push([args[i], args[i + 1]]);
            ++i;
        }

        return result;
    }

    var buildPolygon = function() {

    }

    this.clear = function() {
        _ctx.clearRect(0, 0, _element.width, _element.height);
    };

    this.drawCircle = function(x, y, r) {
        buildCircle(x, y, r);
        _ctx.stroke();
    }

    this.drawEllipse = function(x, y, rx, ry) {
        buildEllipse(x, y, rx, ry);
        _ctx.stroke();
    }

    this.drawPolyline = function(points) {
        if (points.length < 4) {
            Log.d('Not drawing polyline with less than four coordinates.');
            return;
        }

        _ctx.beginPath();
        _ctx.moveTo(points[0][0], _element.height - points[0][1]);
        var i = 1;
        while (i < points.length) {
            _ctx.lineTo(points[i][0], _element.height - points[i][1]);
            ++i;
        }

        //_ctx.closePath();
        _ctx.stroke();
    }

    this.drawRectangle = function(x, y, width, height) {
        x = _alignment.x(x, width);
        y = _alignment.y(y, height);
        _ctx.drawRect(x, y, width, height);
        _ctx.stroke();
    }

    this.drawText = function(x, y, text) {
        var m = _ctx.measureText(text);
        x = _alignment.x(x, m.width);
        //y = _alignment.y(y, );
        _ctx.fillText(text, x, _element.height - y);
    }

    this.fill = function() {
        _ctx.fillRect(0, 0, _element.width, _element.height);
    };

    this.fillCircle = function(x, y, r) {
        buildCircle(x, y, r);
        _ctx.fill();
    }

    this.fillEllipse = function(x, y, rx, ry) {
        buildEllipse(x, y, rx, ry);
        _ctx.fill();
    }

    this.getAlignment = function() {
        return _alignment;
    }

    this.getColor = function() {
        return _color;
    };

    this.getHeight = function() {
        return _element.height;
    }

    this.getLineWidth = function() {
        return _ctx.lineWidth;
    }

    this.getTextSize = function() {
        return _textSize;;
    }

    this.getWidth = function() {
        return _element.width;
    }

    this.setAlignment = function(alignment) {
        _alignment = alignment;
    }

    this.setColor = function(color) {
        if (typeof color == 'string') {
            _color = color;
        }
        else if (typeof color == 'object') {
            _color = color.value;
        }

        _ctx.fillStyle = _color;
        _ctx.strokeStyle = _color;
    }

    this.setLineWidth = function(width) {
        _ctx.lineWidth = width;
    }

    this.setTextSize = function(textSize) {
        if (typeof width != 'undefined') {
            _textSize = textSize;
            _ctx.font = _textSize + ' ' + _fontName;
        }
    }
}


if (window.jQuery) {
    $(document).ready(function() {
        if (typeof setup == 'function') {
            setup();
        }

        $('canvas').each(function(i, c) {
            var scriptName = $(c).attr('data-draw');
            var canvas = new Canvas(c);
            executeFunctionByName(scriptName, window, canvas);
        });
    });
}
