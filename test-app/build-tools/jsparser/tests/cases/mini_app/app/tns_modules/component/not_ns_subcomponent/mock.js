var buttonName = "a.B.C.ButtonThatShouldNotBeExtendedComponent.Subcomponent";

android.widget.Button.extend("a.B.C.ButtonThatShouldNotBeExtendedComponent.Subcomponent", {
    toString: function() {
        return buttonName;
    }
});