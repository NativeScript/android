android.widget.Button.extend({
    getHashCode: function() {
        return 15;
    },
    toString: function() {
        return "My button is anonymous and extended!";
    }
});

android.widget.Button.extend("A.B.MyButton", {
    toString: function() {
        return "My button is extended!";
    }
});

android.widget.Button.extend({
});

android.view.View.extend({
    onAttachedToWindow: function() {

    },
    onDetachedFromWindow: function() {

    }
});

java.lang.Object.extend("com.tns.MyObject", {
    getHashCode: function() {
        return 42;
    }
});