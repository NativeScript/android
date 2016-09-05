var buttonName = "a.B.C.ButtonThatShouldNotBeExtended";

android.widget.Button.extend("a.B.C.ButtonThatShouldNotBeExtended", {
    toString: function() {
        return buttonName;
    }
});