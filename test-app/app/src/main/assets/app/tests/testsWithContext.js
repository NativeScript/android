exports.run = function(cntxt)
{
	describe("Tests with context ", function () {

		var context = cntxt;
		var myCustomEquality = function(first, second) {
			return first == second;
		};

		beforeEach(function() {
			jasmine.addCustomEqualityTester(myCustomEquality);
		});

		it("TestConstructorOverrideForBuiltinType", function () {

			__log("TEST: TestConstructorOverrideForBuiltinType");

			var ctorCalled = false;
			var isConstructor = false;

			var MyButton = android.widget.Button.extend({
				init : function() {
					ctorCalled = true;
					isConstructor = arguments[arguments.length - 1];
				}
			});

			var btn = new MyButton(context);

			expect(ctorCalled).toEqual(true);
			expect(isConstructor).toEqual(true);
		});

//		it("TestConstructorOverrideForBuiltinTypeWithInitMethod", function () {
//
//			__log("TEST: TestConstructorOverrideForBuiltinTypeWithInitMethod");
//
//			var initInvocationCount = 0;
//
//			var MyDatePicker = android.widget.DatePicker.extend({
//				init: function() {
//					++initInvocationCount;
//				}
//			});
//
//			var datePicker = new MyDatePicker(context);
//
//			__log("datePicker=" + datePicker);
//
//			var count1 = initInvocationCount;
//
//			expect(count1).toBeGreaterThan(0);
//
//			datePicker.init(2014, 3, 25, null);
//
//			var count2 = initInvocationCount;
//
//			expect(count2).toBeGreaterThan(count1);
//		});

		it("TestBuiltinNestedClassCreation", function () {

			__log("TEST: TestBuiltinNestedClassCreation");

			var loader = new android.content.Loader(context);

			var observer = new android.content.Loader.ForceLoadContentObserver(loader);

			expect(observer).not.toEqual(null);
		});

		it("When instantiating inner class without passing outer `this` expect exception to be thrown", function () {
            __log("TEST: When instantiating inner class without passing outer `this` expect exception to be thrown");

            var exceptionCaught = false;
//            var loader = new android.content.Loader(context);
            try {
                var observer = new android.content.Loader.ForceLoadContentObserver();
            } catch (e) {
                exceptionCaught = true;
            }

            expect(exceptionCaught).toBe(true);
        });

		it("TestPublicWindowManagerImplWithoutMetadata", function () {

			__log("TEST: TestPublicWindowManagerImplWithoutMetadata");

			var windowManagerImpl = context.getSystemService(android.content.Context.WINDOW_SERVICE);

			var display = windowManagerImpl.getDefaultDisplay();

			//__log("display.isValid=" + display.isValid());

			var displayInfo = display.toString();

			expect(displayInfo.length).toBeGreaterThan(0);
		});

		it("TestCanPassCharSequenceArray", function () {

			__log("TEST: TestCanPassCharSequenceArray");

			var alert = new android.app.AlertDialog.Builder(context);

		    var builder = alert.setItems(["One", "Two" ], new android.content.DialogInterface.OnClickListener({
		        onClick: function (dialog, which) {
		            //
		        }
		    }));

			expect(builder).not.toEqual(null);
		});

		it("TestOldAPIForGettingMethodsListForMethodsWithParametersFromMissingType", function () {
            __log("TEST: TestOldAPIForGettingMethodsListForMethodsWithParametersFromMissingType");

            if(android.support.design && android.support.design.widget) {
                var til = new android.support.design.widget.TextInputLayout(context);
                var editText = new android.widget.EditText(context);
                var relativeLayout = new android.widget.RelativeLayout(context);
                var relativeLayoutParams = new android.widget.RelativeLayout.LayoutParams(android.widget.RelativeLayout.LayoutParams.MATCH_PARENT, android.widget.RelativeLayout.LayoutParams.MATCH_PARENT);
                relativeLayout.setLayoutParams(relativeLayoutParams);
                editText.setHint("TEST");
                til.addView(editText);
            }
        });

        it("TestPackagePrivateAccessForOtherPackages", function () {

                		      var ll = new android.widget.LinearLayout(context);
                               var didThrow = false;
                                try{
                                // https://android.googlesource.com/platform/frameworks/base/+/master/core/java/android/widget/LinearLayout.java#685
                                    ll.getVirtualChildCount();
                                }catch(e){
                                    // will throw since we don't have access to this
                                    didThrow = true;
                                }

                			expect(didThrow).toEqual(true);
                		});
	});
};