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
		
		it("TestConstructorOverrideForBuiltinTypeWithInitMethod", function () {
			
			__log("TEST: TestConstructorOverrideForBuiltinTypeWithInitMethod");
			
			var initInvocationCount = 0;

			var MyDatePicker = android.widget.DatePicker.extend({
				init: function() {
					++initInvocationCount;
				}
			});
			
			var datePicker = new MyDatePicker(context);
			
			__log("datePicker=" + datePicker);
			
			var count1 = initInvocationCount;
			
			expect(count1).toBeGreaterThan(0);
			
			datePicker.init(2014, 3, 25, null);
			
			var count2 = initInvocationCount;
			
			expect(count2).toBeGreaterThan(count1);
		});
		
		it("TestBuiltinNestedClassCreation", function () {
			
			__log("TEST: TestBuiltinNestedClassCreation");
			
			var loader = new android.content.Loader(context);
			
			var observer = new loader.ForceLoadContentObserver();
			
			expect(observer).not.toEqual(null);
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
	});
};