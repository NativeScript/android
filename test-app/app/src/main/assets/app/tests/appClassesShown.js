// Whether the build showed the static binding generator the app module's own classes,
// which -PskipSbgAppClasses turns off. Only the generator writes a class for the call
// site below, which never runs, and it can only when it was shown them.
if (false) {
    new com.tns.tests.AppModuleShapes.StepProbe({
        describe: function () {
            return "never runs";
        }
    });
}

exports.appClassesShown = (function () {
    try {
        java.lang.Class.forName("com.tns.gen.com.tns.tests.AppModuleShapes_StepProbe", false, com.tns.tests.AppModuleShapes.class.getClassLoader());
        return true;
    } catch (e) {
        return false;
    }
}());
