var buttonName = "a.B.C.ButtonThatShouldNotBeExtended.Submodule";

android.widget.Button.extend("a.B.C.ButtonThatShouldNotBeExtended.Submodule", {
    toString: function() {
        return buttonName;
    }
});