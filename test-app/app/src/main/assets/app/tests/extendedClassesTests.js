describe("Tests extended classes ", function () {

	it("Instance_with_no_extension_shouldnt_use_previously_defined_implementation_object", function () {
		var MyButton = com.tns.tests.Button1.extend({
			toString: function () {
				return "overriden toString method of chronometer instance";
			},
			getIMAGE_ID_PROP: function () {
				return "overriden getIMAGE_ID_PROP method on button";
			}
		});
		var button = new MyButton();
		var labelToString = button.toString();
		var labelgetIMAGE_ID_PROP = button.getIMAGE_ID_PROP();
		//
		
		var button1 = new com.tns.tests.Button1();
		var labelToString1 = button1.toString();
		var labelgetIMAGE_ID_PROP1 = button1.getIMAGE_ID_PROP();
		
		expect(labelToString).not.toBe(labelToString1);
		expect(labelgetIMAGE_ID_PROP).not.toBe(labelgetIMAGE_ID_PROP1);
	});
	
	it("Instance_with_extension_shouldnt_use_previously_defined_implementation_object", function () {
		
		var MyButton = com.tns.tests.Button1.extend({
			toString: function () {
				return "overriden toString method of button instance";
			},
			getIMAGE_ID_PROP: function () {
				return "overriden getIMAGE_ID_PROP method on button";
			}
		});
		var button = new MyButton();
		var labelToString = button.toString();
		var labelgetIMAGE_ID_PROP = button.getIMAGE_ID_PROP();
		//
		
		var MyButton1 = com.tns.tests.Button1.extend({
			toString: function () {
				return "overriden toString method of button1 instance ";
			},
			getIMAGE_ID_PROP: function () {
				return "overriden getIMAGE_ID_PROP method on button1";
			}
		});
		var button1 = new MyButton1();
		var labelToString1 = button1.toString();
		var labelgetIMAGE_ID_PROP1 = button1.getIMAGE_ID_PROP();

		expect(labelToString).not.toBe(labelToString1);
		expect(labelgetIMAGE_ID_PROP).not.toBe(labelgetIMAGE_ID_PROP1);
	});
	
	it("Newly_created_instances_should_behave_the_same_and_not_use_previously_defined_implementation_objects", function () {

		var button1 = new com.tns.tests.Button1();
		var labelgetIMAGE_ID_PROP1 = button1.getIMAGE_ID_PROP();
		
		//
		var MyButton = com.tns.tests.Button1.extend({
			getIMAGE_ID_PROP: function () {
				return "overriden getIMAGE_ID_PROP method on button";
			}
		});
		var button = new MyButton();
		var labelgetIMAGE_ID_PROP = button.getIMAGE_ID_PROP();
		//
		
		var button2 = new com.tns.tests.Button1();
		var labelgetIMAGE_ID_PROP2 = button2.getIMAGE_ID_PROP();
		
		expect(labelgetIMAGE_ID_PROP1).toBe(labelgetIMAGE_ID_PROP2);
	});

	it("should not crash with no exception when incorrectly calling extended class constructor", function () {
	    let MyObj = java.lang.Object.extend({
                        toString: () => { return "It's MyObj" }
                    });

        expect(() => { myObj() }).toThrow();
	});
});