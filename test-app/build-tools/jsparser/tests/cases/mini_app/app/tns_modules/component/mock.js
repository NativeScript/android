var buttonName = "a.B.C.ButtonThatIsExtendedComponent";

android.widget.Button.extend("a.B.C.ButtonThatIsExtendedComponent", {
    toString: function() {
        return buttonName;
    }
});