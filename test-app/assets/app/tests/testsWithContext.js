var Assert = function(condition, failMessage) {
	if	(condition == false) {
		fail(failMessage);
	}
}

var TestConstructorOverrideForBuiltinType = function(context) {

	Log("TEST: TestConstructorOverrideForBuiltinType");
	
	var ctorCalled = false;
	var isConstructor = false;

	var MyButton = new android.widget.Button.extend({
		init : function() {
			ctorCalled = true;
			isConstructor = arguments[arguments.length - 1];
		}
	});
	
	var btn = new MyButton(context);
	
	Assert(ctorCalled == true, "TestConstructorOverrideForBuiltinType FAILED: constructor not called");
	Assert(isConstructor == true, "TestConstructorOverrideForBuiltinType FAILED: isConstructor should be 'true'");
}

var TestConstructorOverrideForBuiltinTypeWithInitMethod = function(context) {

	Log("TEST: TestConstructorOverrideForBuiltinTypeWithInitMethod");
	
	var initInvocationCount = 0;

	var MyDatePicker = android.widget.DatePicker.extend({
		init: function() {
			++initInvocationCount;
		}
	});
	
	var datePicker = new MyDatePicker(context);
	
	Log("datePicker=" + datePicker);
	
	var count1 = initInvocationCount;
	
	Assert(count1 > 0, "TestConstructorOverrideForBuiltinTypeWithInitMethod FAILED: initInvocationCount should be > 0");
	
	datePicker.init(2014, 3, 25, null);
	
	var count2 = initInvocationCount;
	
	Assert(count2 > count1, "TestConstructorOverrideForBuiltinTypeWithInitMethod FAILED: initInvocationCount should be increased");
}

var TestBuiltinNestedClassCreation = function(context) {
	
	Log("TEST: TestBuiltinNestedClassCreation");
	
	var loader = new android.content.Loader(context);
	
	var observer = new loader.ForceLoadContentObserver();
	
	Assert(observer != null, "TestBuiltinNestedClassCreation FAILED: Cannot instantiate android.content.Loader.ForceLoadContentObserver class");
}


var TestPublicWindowManagerImplWithoutMetadata = function(context) {

	Log("TEST: TestPublicWindowManagerImplWithoutMetadata");
	
	var windowManagerImpl = context.getSystemService(android.content.Context.WINDOW_SERVICE);
	
	var display = windowManagerImpl.getDefaultDisplay();
	
	//Log("display.isValid=" + display.isValid());
	
	var displayInfo = display.toString();
	
	Assert(displayInfo.length > 0, "TestPublicWindowManagerImplWithoutMetadata FAILED: Cannot obtain display info string from WindowManagerImpl instance");
}

var TestUsingClassFromAndroidSupportLibrary = function(context) {

	Log("TEST: TestUsingClassFromAndroidSupportLibrary");

	var layout = new android.support.v4.widget.DrawerLayout(context);
	
	Assert(layout != null, "TestUsingClassFromAndroidSupportLibrary FAILED: Cannot create an instance of class from Android Support Library");
}

var TestCanPassCharSequenceArray = function(context) {
	
	Log("TEST: TestCanPassCharSequenceArray");
	
	var alert = new android.app.AlertDialog.Builder(context);
    
    var builder = alert.setItems(["One", "Two" ], new android.content.DialogInterface.OnClickListener({
        onClick: function (dialog, which) {
            //
        }
    }));
    
    Assert(builder != null, "TestCanPassCharSequenceArray: builder should be != null");
}

exports.run = function(context)
{
	TestConstructorOverrideForBuiltinType(context);
	TestConstructorOverrideForBuiltinTypeWithInitMethod(context);
	TestBuiltinNestedClassCreation(context);
	TestPublicWindowManagerImplWithoutMetadata(context);
	TestUsingClassFromAndroidSupportLibrary(context);
	TestCanPassCharSequenceArray(context);
}